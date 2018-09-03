#include	"profiler.h"

#include	<stdlib.h>


#undef LOGSECTION
#define LOGSECTION LOGSECTION_INIT

namespace	Profiler
{

	U32	*	mpFrameRingBuffer;
	U32		mFrameRingBufferIndex;
	U32		mFrameRingBufferCount;
	U32		mFrameRingBufferVBLIndex;
	U32		mFrameRingBufferFrameCount;
	bool	mFrameRingBufferWrappedFlag;
	U32	*	mpCycleCounts = 0;
	bool	mProfilingFlag = true;
	U32		mAddressMin;
	U32		mAddressMax;
	U32		mAddressEntryCount = 1024 * 1024 * 4;

#define mENDIAN_U16_SWAPPED( _aValue )	( ( (_aValue&0xFF) << 8 ) | ( (_aValue>>8) & 0xFF ) )
#define mENDIAN_U32_SWAPPED( _aValue )	( ( (_aValue&0xFF) << 24 )| ( (_aValue&0xFF00) << 8 ) | ((_aValue>>8) & 0xFF00) | ((_aValue>>24) & 0xFF) )

#define mENDIAN_BIG_U16_READ( _apPtr )	mENDIAN_U16_SWAPPED( *(U16*)(_apPtr) )
#define mENDIAN_BIG_U32_READ( _apPtr )	mENDIAN_U32_SWAPPED( *(U32*)(_apPtr) )


#define mPROFILER_MALLOC( _aSize )	malloc( _aSize );
#define mPROFILER_DEALLOC( _aPtr )	if( _aPtr ) { free( _aPtr ); } _aPtr = 0

	char *	StringAlloc( const char * apSrc )
	{
		char * lpStr = 0;
		if( apSrc )
		{
			U32 lLen = 0;
			while( apSrc[ lLen ] )
			{
				lLen++;
			}
			lpStr = (char*)mPROFILER_MALLOC( lLen + 1 );
			if( lpStr )
			{
				for( U32 i=0; i<=lLen; i++ )
				{
					lpStr[ i ] = apSrc[ i ];
				}
			}
		}
		return( lpStr );
	}

	struct sSymbolDRI 
	{
		U8	mSymbolName[ 8 ];
		U16	mType;
		U8	mData[4];
	};

	struct sProgramHeader
	{
		U16	mMagic;
		U32	mTextLength;
		U32	mDataLength;
		U32	mBSSLength;
		U32	mSymbolLength;
		U32	mReserved;
		U32	mFlags;
		U16	mRelocationFlag;
	};

	struct sFrameInfo
	{
		U32	mFrameIndex;
		U32	mFrameNumber;
		U32	mCycleCount;
	};

#define dPROFILER_FRAMEINFO_COUNT	256

	sFrameInfo	mFrameInfos[ dPROFILER_FRAMEINFO_COUNT ];


	class cProcess
	{
	public:
		cProcess() : mpFileName(0), mSymbolCount(0),mSymbolCapacity(0),mpSymbols(0),mpNext(0) {}

		void	Init( const char * apFileName )
		{
			mpFileName = 0;
			mpFrameCycleCounts = 0;
			mSymbolCount = 0;
			mSymbolCapacity = 0;
			mpSymbols = 0;
			mpNext = 0;
			mpFileName = StringAlloc( apFileName );
		}

		void	DeInit( void )
		{
			mPROFILER_DEALLOC( mpFileName );
			mPROFILER_DEALLOC( mpSymbols );
			mPROFILER_DEALLOC( mppSymbolsSorted );
			mSymbolCapacity = 0;
			mSymbolCount = 0;
		}

		void	SetSymbolCapacity( U32 aCapacity )
		{
			mSymbolCapacity = aCapacity;
			mSymbolCount = 0;
			mpSymbols = (cSymbol*)mPROFILER_MALLOC( aCapacity * sizeof(cSymbol) );
			mppSymbolsSorted = (cSymbol **)mPROFILER_MALLOC( aCapacity * sizeof(cSymbol*) );
			for( U32 s=0; s<mSymbolCapacity; s++ )
			{
				mppSymbolsSorted[ s ] = &mpSymbols[ s ];
			}
		}

		void	SymbolAdd( const char * apName, U32 aAddress )
		{
			if( mSymbolCount < mSymbolCapacity )
			{
				mpSymbols[ mSymbolCount ].mpName = StringAlloc( apName );
				mpSymbols[ mSymbolCount ].mAddress = aAddress;
				mpSymbols[ mSymbolCount ].mCycleCount = 0;
				mSymbolCount++;
			}
		}

		void	CycleCountsBuildFromTotal( void );
		void	CycleCountsBuildFromFrame( U32 aIndex );

		void	SymbolsSort( void );
		void	SymbolsAlphaSort( void );

		void	SymbolsDRIBuild( void );
		bool	SymbolsExternalBuild( void );

		char*		mpFileName;
		U32			mSymbolCount;
		U32			mSymbolCapacity;
		U32 *		mpFrameCycleCounts;
		cSymbol *	mpSymbols;
		cSymbol **	mppSymbolsSorted;
		cProcess *	mpNext;

		U32			mBasePageAdr;
		U32			mTextAdr;
		U32			mTextLength;
		U32			mDataAdr;
		U32			mDataLength;
		U32			mBSSAdr;
		U32			mBSSLength;

		U32			mProcessStartAdr;
		U32			mProcessEndAdr;
		U32			mProcessByteCount;
	};

	cProcess *	mpProcessStack = 0;


	void	Init( void )
	{
		U32 lSize = mAddressEntryCount * sizeof(U32);
		mpCycleCounts = (U32*)malloc( lSize );
		if( mpCycleCounts )
		{
			memset( mpCycleCounts, 0, lSize );
		}

		mFrameRingBufferCount = (4096 * 2024);

		lSize = mFrameRingBufferCount * sizeof(U32);
		mpFrameRingBuffer = (U32*)malloc( lSize );
		if( mpFrameRingBuffer )
		{
			memset( mpFrameRingBuffer, 0, lSize );
		}

		mFrameRingBufferWrappedFlag = false;
		mFrameRingBufferVBLIndex = 0;
	}


	void	DeInit( void )
	{
		free( mpCycleCounts );
		mpCycleCounts = 0;
	}


	void	Start( void )
	{
	}


	void	Stop( void )
	{
	}

	void	FrameRingBufferNext( void )
	{
		mFrameRingBufferIndex++;
		if( mFrameRingBufferIndex > mFrameRingBufferCount )
		{
			mFrameRingBufferWrappedFlag = 0;
			mFrameRingBufferIndex = 0;
			mFrameRingBufferWrappedFlag = true;
		}
	}

	void	FrameRingBufferAdd( U32 aAddress, U32 aCycleCount )
	{
		U32	lValue;

		lValue = aAddress & 0x00FFFFFF;
		lValue |= (aCycleCount & 0xFF) << 24;

		mpFrameRingBuffer[ mFrameRingBufferIndex ] = lValue;
		FrameRingBufferNext();
	}

	void	FrameRingBufferVBL()
	{
		mpFrameRingBuffer[ mFrameRingBufferIndex ] = (mFrameRingBufferVBLIndex << 1) | 1;
		mFrameRingBufferVBLIndex++;
		FrameRingBufferNext();
	}

	unsigned int	GetFrameCount( void )
	{
		return( mFrameRingBufferFrameCount );
	}

	unsigned int	GetFrameNumber( unsigned int aFrameIndex )
	{
		return( mFrameInfos[ aFrameIndex ].mFrameNumber );
	}

	unsigned int	GetFrameIndex( unsigned int aFrameIndex )
	{
		return( mFrameInfos[ aFrameIndex ].mFrameIndex );
	}


	unsigned int	GetFrameCycleCount( unsigned int aFrameIndex )
	{
		return( mFrameInfos[ aFrameIndex ].mCycleCount );
	}

	int	FrameInfo_CycleCompare( const void * ap0, const void * ap1 )
	{
		sFrameInfo * lpF0 = (sFrameInfo *)ap0;
		sFrameInfo * lpF1 = (sFrameInfo *)ap1;

		return( lpF1->mCycleCount - lpF0->mCycleCount );
	}

	void	FrameCyclesSort( void )
	{
		qsort( &mFrameInfos[ 0 ], mFrameRingBufferFrameCount, sizeof(sFrameInfo), FrameInfo_CycleCompare );
	}


	void			FrameRingBufferSort( void )
	{
		S32	lIndex;
		U32	lCycleCount;


		mFrameRingBufferFrameCount = 0;

		lIndex = mFrameRingBufferIndex;
		lIndex--;

		lCycleCount = 0;

		while( lIndex != mFrameRingBufferIndex )
		{
			if( lIndex < 0 )
			{
				lIndex = mFrameRingBufferCount-1;
			}

			if( mpFrameRingBuffer[ lIndex ] & 1 )
			{
				if( mFrameRingBufferFrameCount < dPROFILER_FRAMEINFO_COUNT )
				{
					mFrameInfos[ mFrameRingBufferFrameCount ].mCycleCount = lCycleCount;
					mFrameInfos[ mFrameRingBufferFrameCount ].mFrameNumber = mFrameRingBufferVBLIndex - mFrameRingBufferFrameCount;
					mFrameInfos[ mFrameRingBufferFrameCount ].mFrameIndex = mFrameRingBufferFrameCount;
					mFrameRingBufferFrameCount++;
				}
				lCycleCount = 0;
			}

			lCycleCount += (mpFrameRingBuffer[ lIndex ] >> 24);

			lIndex--;
		}

/*
		U32 lMaxIndex = dPROFILER_FRAMEINFO_COUNT;


		if( mFrameRingBufferCount < lMaxIndex )
		{
			lMaxIndex = mFrameRingBufferFrameCount;
		}

		for( U32 i=0; i<lMaxIndex>>1; i++ )
		{
			sFrameInfo lTemp;

			lTemp = mFrameInfos[ i ];
			mFrameInfos[ i ] = mFrameInfos[ (lMaxIndex-1) - i ];
			mFrameInfos[ (lMaxIndex-1) - i ] = lTemp;
		}
*/
	}




	void	Add( unsigned long aAddress, int aCycleCount )
	{
		if( mProfilingFlag && (aAddress < mAddressEntryCount) )
		{
			if( aAddress > mAddressMax )
			{
				mAddressMax = aAddress;
			}
			if( aAddress < mAddressMin )
			{
				mAddressMin = aAddress;
			}
			mpCycleCounts[ aAddress ] += aCycleCount;

			FrameRingBufferAdd( aAddress, aCycleCount );
		}
	}

	void	OnVBL( void )
	{
		FrameRingBufferVBL();
	}


	bool	IsHexNumber( char aChar )
	{
		if( ( (aChar >='0') && (aChar<='9') ) ||
			( (aChar >='a') && (aChar<='f') ) ||
			( (aChar >='A') && (aChar<='F') ) )
		{
			return( true );
		}
		return( false );
	}


	int	SkipToLineEnd( unsigned char * apText, int aOffset, int aLength )
	{
		while( (aOffset < aLength) && ( 10 != apText[ aOffset ] ) && ( 13 != apText[ aOffset ] ) )
		{
			aOffset++;
		}
		return( aOffset );
	}


	int	SkipToLineNext( unsigned char * apText, int aOffset, int aLength )
	{
		while( (aOffset < aLength) && ( 10 != apText[ aOffset ] ) && ( 13 != apText[ aOffset ] ) )
		{
			aOffset++;
		}
		while( (aOffset < aLength) && ( ( 10 == apText[ aOffset ] ) || ( 13 == apText[ aOffset ] ) ) )
		{
			aOffset++;
		}
		return( aOffset );
	}


	void	OnPterm( void )
	{
		// let's dump profile to lofile

		log_write( "Pterm() : Dumping Profile:");
		Dump();

		cProcess * lpProc = mpProcessStack;
		if( lpProc )
		{
			U32 lSize = lpProc->mProcessEndAdr - lpProc->mProcessStartAdr;
			memset( &mpCycleCounts[ lpProc->mProcessStartAdr ], 0, lSize * sizeof(U32) );
			mpProcessStack = lpProc->mpNext;
			mPROFILER_DEALLOC( lpProc );
		}
	}


	void	OnPexec( const char * apFileName, unsigned int aBasePageAddress )
	{
		if( apFileName )
		{
			cProcess * lpProcess = (cProcess *)mPROFILER_MALLOC( sizeof(cProcess) );

			lpProcess->mpNext = mpProcessStack;
			mpProcessStack = lpProcess;

			lpProcess->Init( apFileName );

			lpProcess->mBasePageAdr = aBasePageAddress;
			lpProcess->mTextAdr = m68k_lpeek( aBasePageAddress + 0x8 );
			lpProcess->mTextLength = m68k_lpeek( aBasePageAddress + 0x0C );

			lpProcess->mDataAdr = m68k_lpeek( aBasePageAddress + 0x10 );
			lpProcess->mDataLength = m68k_lpeek( aBasePageAddress + 0x14 );

			lpProcess->mBSSAdr = m68k_lpeek( aBasePageAddress + 0x18 );
			lpProcess->mBSSLength = m68k_lpeek( aBasePageAddress + 0x1c );

			lpProcess->mProcessStartAdr = lpProcess->mTextAdr;
			lpProcess->mProcessEndAdr = lpProcess->mProcessStartAdr + lpProcess->mTextLength + lpProcess->mDataLength + lpProcess->mBSSLength;
			lpProcess->mProcessByteCount = lpProcess->mProcessEndAdr - lpProcess->mProcessStartAdr;

			if( !lpProcess->SymbolsExternalBuild() )
			{
				lpProcess->SymbolsDRIBuild();
			}
//			lpProcess->SymbolsExternalBuild();

			lpProcess->mpFrameCycleCounts = (U32*)mPROFILER_MALLOC( lpProcess->mProcessByteCount * sizeof(U32) );

			mFrameRingBufferVBLIndex = 0;
			mFrameRingBufferIndex = 0;
			mFrameRingBufferWrappedFlag = false;
			OnVBL();

		}
	}


	void	Reset( void )
	{
		if( mpCycleCounts )
		{
			U32 lSize = mAddressEntryCount * sizeof(U32);
			memset( mpCycleCounts, 0, lSize );
		}
	}


	void	Dump( void )
	{
		if( mpProcessStack )
		{
			mpProcessStack->SymbolsSort();

			for( U32 s=0; s<mpProcessStack->mSymbolCount; s++ )
			{
				cSymbol * lpS = mpProcessStack->mppSymbolsSorted[ s ];
				if( lpS->mCycleCount )
				{
					char	lCycleCount[ 32 ];
					sprintf( lCycleCount, "%16d,%08X,", lpS->mCycleCount, lpS->mAddress );
					log_write( EasyStr( lCycleCount ) + lpS->mpName );
				}
			}
		}
	}



	void	TotalCyclesSort( void )
	{
		if( mpProcessStack )
		{
			mpProcessStack->SymbolsSort();
		}
	}

	void	AlphaSort( void )
	{
		if( mpProcessStack )
		{
			mpProcessStack->SymbolsAlphaSort();
		}
	}

	void			SymbolCycleCountsBuildFromTotal( void )
	{
		if( mpProcessStack )
		{
			mpProcessStack->CycleCountsBuildFromTotal();
		}
	}

	void			SymbolCycleCountsBuildFromFrame( unsigned int aIndex )
	{
		if( mpProcessStack )
		{
			mpProcessStack->CycleCountsBuildFromFrame( aIndex );
		}
	}


	unsigned int	GetInstructionCycleCountTotal( unsigned int aAddress )
	{
		unsigned int lCycleCount = 0;
		if( mpCycleCounts && (aAddress < mAddressEntryCount) )
		{
			lCycleCount = mpCycleCounts[ aAddress ];
		}
		return( lCycleCount );
	}


	unsigned int	GetInstructionCycleCountFrame( unsigned int aAddress )
	{
		unsigned int lCycleCount = 0;
		cProcess * lpP = mpProcessStack;
		if( lpP && lpP->mpFrameCycleCounts && (aAddress >= lpP->mProcessStartAdr) && (aAddress < lpP->mProcessEndAdr) )
		{
			lCycleCount = lpP->mpFrameCycleCounts[ aAddress - lpP->mProcessStartAdr ];
		}
		return( lCycleCount );
	}


	const char *	GetSymbolName( unsigned int aAddress )
	{
		char * lpRes = "";
		cProcess * lpP = mpProcessStack;
		if( lpP && (aAddress < lpP->mProcessEndAdr) && (aAddress >= lpP->mProcessStartAdr) )
		{
			//U32	lAdr = aAddress - lpP->mProcessStartAdr;
			U32 lIndex = 0;
			while( (lIndex<lpP->mSymbolCount) && (aAddress >= lpP->mpSymbols[lIndex].mAddress) )
			{
				lpRes = lpP->mpSymbols[ lIndex ].mpName;
				lIndex++;
			}
		}
		return( lpRes );
	}


	unsigned int	GetSymbolCount( void )
	{
		unsigned int lCount = 0;

		if( mpProcessStack )
		{
			return( mpProcessStack->mSymbolCount );
		}

		return( lCount );
	}


	const cSymbol *	GetpTotalCycleSymbol( unsigned int aIndex )
	{
		const cSymbol * lpS = 0;

		if( mpProcessStack && (aIndex < mpProcessStack->mSymbolCount) )
		{
			return( mpProcessStack->mppSymbolsSorted[ aIndex ] );
		}

		return( lpS );
	}

	int	Symbol_AdressCompare( const void * ap0, const void * ap1 )
	{
		cSymbol * lppS0 = (cSymbol *)ap0;
		cSymbol * lppS1 = (cSymbol *)ap1;

		return( lppS0->mAddress - lppS1->mAddress );
	}


	// GST symbols allow 22 character names instead of 8
	// they use next DRI symbol to store remainding text of symbol name

	void	cProcess::SymbolsDRIBuild( void )
	{
		FILE * lpFile = fopen( mpFileName, "rb" );
		if( lpFile )
		{
			fseek( lpFile, 0, SEEK_END );
			int lSize = ftell( lpFile);
			fseek( lpFile, 2, SEEK_SET );

			U32	lLengths[ 4 ];
			fread( &lLengths[0], sizeof(lLengths), 1, lpFile );

			U32 lTextLength = mENDIAN_BIG_U32_READ( &lLengths[ 0 ] );
			U32 lDataLength = mENDIAN_BIG_U32_READ( &lLengths[ 1 ] );
			U32 lBSSLength = mENDIAN_BIG_U32_READ( &lLengths[ 2 ] );
			U32 lSymbolLength = mENDIAN_BIG_U32_READ( &lLengths[ 3 ] );

			U32	lOffset = (lTextLength+lDataLength) + 0x1C;

			fseek( lpFile, lOffset, SEEK_SET );

			sSymbolDRI * lpSymbols = (sSymbolDRI*)mPROFILER_MALLOC( lSymbolLength );
			fread( lpSymbols, lSymbolLength, 1, lpFile );

			U32 lDriSymCount = lSymbolLength / sizeof(sSymbolDRI);
			U32	lTotalSymCount = 0;

			for( U32 s=0; s<lDriSymCount; s++ )
			{
				U16	lType = mENDIAN_BIG_U16_READ( &lpSymbols[ s ].mType );
				// check for extended symbol
				if( lType & 0x48)
				{
					s++;
				}
				// check for TEXT (0x100) DATA (0x200) or BSS (0x400)
				if( lType & 0x700)
				{
					lTotalSymCount++;
				}
			}

			SetSymbolCapacity( lTotalSymCount );
			for( U32 s=0; s<lDriSymCount; s++ )
			{
				U16 lType = mENDIAN_BIG_U16_READ( &lpSymbols[ s ].mType );
				U32 lAdr = mENDIAN_BIG_U32_READ( &lpSymbols[ s ].mData[ 0 ] ) + mTextAdr;
				char	lName[ 23 ];
				lName[ 22 ] = 0;

				for( U32 i=0; i<8; i++ )
				{
					lName[ i ] = lpSymbols[ s ].mSymbolName[ i ];
				}
				lName[ 9 ] = 0;

				if( lType & 0x48)
				{
					s++;
					if( s<lDriSymCount )
					{
						char * lpSrc = (char*)&lpSymbols[ s ];
						for( U32 i=0; i<14; i++ )
						{
							lName[ 8 + i ] = lpSrc[ i ];
						}
					}
				}

				if( lType & 0x0700 )
				{
					SymbolAdd( &lName[ 0 ], lAdr );
				}
			}


			fclose( lpFile );

			qsort( mpSymbols, mSymbolCount, sizeof(cSymbol), Symbol_AdressCompare );

		}

	}


	bool	cProcess::SymbolsExternalBuild( void )
	{
		// load original symbol table
		bool lRes = false;
		int lLen = 0;
		int lIndex = 0;
		while( mpFileName[ lLen ] )	lLen++;

		char * lpSymFileName = (char*)malloc( lLen + 5 );
		if( lpSymFileName )
		{
			for( int i=0; i<= lLen; i++ )
			{
				lpSymFileName[ i ] = mpFileName[ i ];
			}

			lIndex = lLen;
			while( lIndex && ('.' != lpSymFileName[ lIndex-1 ] ) )
			{
				lIndex--;
			}
			lpSymFileName[ lIndex+0 ] = 'S';
			lpSymFileName[ lIndex+1 ] = 'Y';
			lpSymFileName[ lIndex+2 ] = 'M';

			log( EasyStr("Profiler SYM file: ") + lpSymFileName );

			FILE * lpFile = fopen( lpSymFileName, "rb" );
			if( lpFile )
			{

				int lPosOld = ftell( lpFile );

				fseek( lpFile, 0, SEEK_END );
				int lSize = ftell( lpFile);
				fseek( lpFile, lPosOld, SEEK_SET );

				unsigned int lAdr = 0;
				unsigned char * lpSymData = (unsigned char*)malloc( lSize );
				if( lpSymData )
				{
					fread( lpSymData, lSize, 1, lpFile );

					// pass 1 over data, count valid name entries

					int lIndex = 0;
					int lEntryCount = 0;
					while( lIndex < lSize )
					{
						if( IsHexNumber( lpSymData[ lIndex ] ) )
						{
							lEntryCount++;
						}
						lIndex = SkipToLineNext( lpSymData, lIndex, lSize );
					}

					if( lEntryCount )
					{
						lRes = true;
						SetSymbolCapacity( lEntryCount );

						lIndex = 0;
						while( lIndex < lSize )
						{
							if( !IsHexNumber( lpSymData[ lIndex ] ) )
							{
								lIndex = SkipToLineNext( lpSymData, lIndex, lSize );
							}
							else
							{
								int lNumStartIndex = lIndex;
								while( (lIndex<lSize) && (lpSymData[lIndex]!=' '))
								{
									lIndex++;
								}
								if( lIndex < lSize )
								{
									lpSymData[ lIndex ] = 0;
									lAdr = strtol( (char*)&lpSymData[ lNumStartIndex ], 0, 16 );
									lIndex++;
								}
								while( (lIndex<lSize) && (lpSymData[lIndex]==' '))
								{
									lIndex++;
								}
								if( 't' == lpSymData[lIndex] || 'T' == lpSymData[lIndex] )
								{
									lIndex++;
									while( (lIndex<lSize) && (lpSymData[lIndex]==' '))
									{
										lIndex++;
									}
									int lNameStartIndex = lIndex;
									while( (lIndex<lSize) && (lpSymData[lIndex] > 13) )
									{
										lIndex++;
									}
									if( lIndex < lSize )
									{
										lpSymData[ lIndex ] = 0;
									}
									SymbolAdd( (const char*)&lpSymData[ lNameStartIndex ], lAdr + mTextAdr );
								}
								else
								{
									lIndex = SkipToLineNext( lpSymData, lIndex, lSize );
								}
							}
						}
					}
					free( lpSymData );
					qsort( mpSymbols, mSymbolCount, sizeof(cSymbol), Symbol_AdressCompare );
				}

				fclose( lpFile );
			}

		}

		free( lpSymFileName );

		return( lRes );
	}


	int	Symbol_AlphaCompare( const void * ap0, const void * ap1 )
	{
		cSymbol ** lppS0 = (cSymbol **)ap0;
		cSymbol ** lppS1 = (cSymbol **)ap1;

		return( strcmp( (*lppS0)->mpName, (*lppS1)->mpName ) );
	}


	int	Symbol_CycleCountCompare( const void * ap0, const void * ap1 )
	{
		cSymbol ** lppS0 = (cSymbol **)ap0;
		cSymbol ** lppS1 = (cSymbol **)ap1;

		return( (*lppS1)->mCycleCount - (*lppS0)->mCycleCount );
	}


	void	cProcess::SymbolsSort( void )
	{
/*
		for( U32 s=0; s<mSymbolCount; s++ )
		{
			U32	lStartAdr = mpSymbols[ s ].mAddress;
			U32	lNextAdr;
			if( s+1 < mSymbolCount )
			{
				lNextAdr = mpSymbols[ s+1 ].mAddress;
			}
			else
			{
				lNextAdr = mProcessEndAdr;
			}
			mpSymbols[ s ].mCycleCount = 0;
			while( lStartAdr < lNextAdr )
			{
				mpSymbols[ s ].mCycleCount += mpCycleCounts[ lStartAdr ];
				lStartAdr++;
			}
		}*/
		qsort( mppSymbolsSorted, mSymbolCount, sizeof(cSymbol*), Symbol_CycleCountCompare );
	}


	void	cProcess::SymbolsAlphaSort( void )
	{
		qsort( mppSymbolsSorted, mSymbolCount, sizeof(cSymbol*), Symbol_AlphaCompare );

	}


	void	cProcess::CycleCountsBuildFromTotal( void )
	{
		for( U32 s=0; (s+1)<mSymbolCount; s++ )
		{
			U32	lStartAdr = mpSymbols[ s ].mAddress;
			mpSymbols[ s ].mCycleCount = 0;
			while( lStartAdr < mpSymbols[ s+1 ].mAddress )
			{
				mpSymbols[ s ].mCycleCount += mpCycleCounts[ lStartAdr ];
				lStartAdr++;
			}
		}
	}

	void	cProcess::CycleCountsBuildFromFrame( U32 aIndex )
	{
		if( mpFrameCycleCounts )
		{
			memset( mpFrameCycleCounts, 0, sizeof(U32)*mProcessByteCount );
		}

		S32	lOffset;
		U32	lIndex = 0;

		lOffset = mFrameRingBufferIndex;
		do
		{
			lOffset--;
			if( lOffset < 0 )
			{
				lOffset = mFrameRingBufferCount - 1;
			}

			if( mpFrameRingBuffer[ lOffset ] & 1 )
			{
				lIndex++;
			}
			else if( aIndex == lIndex )
			{
				U32	lAdr;
				U32 lCount;

				lAdr   = mpFrameRingBuffer[ lOffset ] & 0x00FFFFFF;
				lCount = mpFrameRingBuffer[ lOffset ] >> 24;

				lAdr -= mProcessStartAdr;
				if( lAdr < mProcessByteCount )
				{
					mpFrameCycleCounts[ lAdr ] += lCount;
				}
			}

		} while( lOffset != mFrameRingBufferIndex );


		for( U32 s=0; (s+1)<mSymbolCount; s++ )
		{
			U32	lStartAdr = mpSymbols[ s ].mAddress;
			mpSymbols[ s ].mCycleCount = 0;
			while( lStartAdr < mpSymbols[ s+1 ].mAddress )
			{
				if( lStartAdr > mProcessStartAdr )
				{
					mpSymbols[ s ].mCycleCount += mpFrameCycleCounts[ lStartAdr-mProcessStartAdr ];
				}
				lStartAdr++;
			}
		}


	}


}

#undef LOGSECTION
