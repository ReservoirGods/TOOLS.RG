/* TODO: optional verbosity
 get PRJ from command line
 process multiple PRJs
 intercept bios and output text to debug log

 parses PBT file
 -A=<pure_c dir>
 -B=<log_output>
 .PRJ
*/

#include <GODLIB\BIOS\BIOS.H>
#include <GODLIB\DRIVE\DRIVE.H>
#include <GODLIB\FILE\FILE.H>
#include <GODLIB\GEMDOS\GEMDOS.H>
#include <GODLIB\PACKER\GODPACK.H>
#include <GODLIB\MEMORY\MEMORY.H>
#include <GODLIB\PROGRAM\PROGRAM.H>
#include <GODLIB\STRING\STRING.H>

#define dEMBEDDED_PUREC

#ifdef dEMBEDDED_PUREC
extern unsigned char PASM_Array[ 44572];
extern unsigned char PCC_Array[ 80521];
extern unsigned char PLINK_Array[ 16915];

typedef struct sEmbeddedProgram
{
	U32		mTextSize;
	U32		mDataSize;
	U32		mBssSize;
	void*	mpData;
} sEmbeddedProgram;

sEmbeddedProgram	gPureCompiler =
{
	0x19e38,
	0x3db4,
	0x72e0,
	PCC_Array
};

sEmbeddedProgram	gPureAssembler =
{
	0x0c838,
	0x330c,
	0x516a,
	PASM_Array
};

sEmbeddedProgram	gPureLinker =
{
	0x53aa,
	0x868,
	0x11cc,
	PLINK_Array
};
#endif


typedef struct sFileBackedBuffer
{
	S32		mFileHandle;
	U16		mOpenFlag;
	U16		mOffset;
	char	mBuffer[ 1024 ];
}sFileBackedBuffer;

typedef struct sFileNameSpace
{
	char mChars[ 512 ];
} sFileNameSpace;

typedef struct sProjectParser
{
	const char *		mpExecutable;
	const char *		mpFileName;
	const char *		mpPureC;
	char *				mpPRJ;
	U32					mSize;
	U32					mObjectCount;
	U32					mObjectDateTimeNewest;
	U16					mObjectBuiltCount;
	U16					mPathPRJLen;
	U16					mAssemblerOptionsLen;
	U16					mCompilerOptionsLen;
	U16					mLinkerOptionsLen;
	U16					mGather;
	char				mPathPRJ[512];
	char				mAssemblerOptions[128];
	char				mCompilerOptions[128];
	char				mLinkerOptions[128];
	sFileBackedBuffer	mLinkerScript;
	sFileBackedBuffer	mDebugLog;
} sProjectParser;

sGemDosDTA	gDTA;
sGemDosDTA	* gpOldDTA = 0;


void	FileBackedBuffer_Init( sFileBackedBuffer * apBuffer, const char * apFileName )
{
	apBuffer->mFileHandle = File_Create( apFileName);
	apBuffer->mOffset=0;
	apBuffer->mOpenFlag=1;
}

void	FileBackedBuffer_Write( sFileBackedBuffer * apBuffer )
{
	if( apBuffer->mOffset && apBuffer->mOpenFlag )
	{
		File_Write( apBuffer->mFileHandle, apBuffer->mOffset, &apBuffer->mBuffer[ 0 ] );
		apBuffer->mOffset = 0;
	}
}

void	FileBackedBuffer_DeInit( sFileBackedBuffer * apBuffer )
{
	FileBackedBuffer_Write(apBuffer);
	if( apBuffer->mOpenFlag )
		File_Close( apBuffer->mFileHandle );
	apBuffer->mFileHandle=0;
	apBuffer->mOffset=0;
	apBuffer->mOpenFlag=0;
}

void	FileBackedBuffer_Append( sFileBackedBuffer * apBuffer, char aChar )
{
	if( apBuffer->mOffset >= 1024 )
	{
		apBuffer->mOffset=1024;
		FileBackedBuffer_Write(apBuffer);
		apBuffer->mOffset=0;
	}
	apBuffer->mBuffer[ apBuffer->mOffset ] = aChar;
	apBuffer->mOffset++;
}

void	FileBackedBuffer_StringAppend( sFileBackedBuffer * apBuffer, const char * apString )
{
	while( *apString )
		FileBackedBuffer_Append( apBuffer, *apString++);
}

int	BuildExistingFilename( sProjectParser * apParser, const char * apShortName, sFileNameSpace * apFullName )
{
	S16 i,j;
	char * lpOpts = apParser->mCompilerOptions;
#ifndef dGODLIB_PLATFORM_ATARI
	char lPath[512];
	Drive_GetPath(0,lPath);
	printf( "curdir: %s\n",lPath);
#endif

	String_StrCpy( apFullName->mChars, apShortName );
/*	printf( "find: %s\n", apFullName->mChars); */
	if( File_Exists(apFullName->mChars ) ) 
		return 1;

	String_StrCpy( apFullName->mChars, apParser->mPathPRJ );
	String_StrCpy( &apFullName->mChars[ apParser->mPathPRJLen ], apShortName );
	/*	printf( "find: %s\n", apFullName->mChars); */
	if( File_Exists(apFullName->mChars ) ) 
		return 1;

	if( apParser->mpPureC )
	{
		String_StrCpy( apFullName->mChars, apParser->mpPureC );
		String_StrAppend2( apFullName->mChars, "\\", apShortName );
		/*	printf( "find: %s\n", apFullName->mChars); */
		if( File_Exists(apFullName->mChars ) ) 
			return 1;
	}

	Drive_GetPath( 0, apFullName->mChars );
	String_StrAppend2( apFullName->mChars, "\\", apShortName );
	/*	printf( "find: %s\n", apFullName->mChars); */
	if( File_Exists(apFullName->mChars ) ) 
		return 1;

	for( i=0; (lpOpts[i]); i++ )
	{
		if( i>1 && '-' ==lpOpts[i-2] && 'I'==lpOpts[i-1])
		{
			String_StrCpy( apFullName->mChars, apParser->mPathPRJ );
			do
			{
				S16 k=0;
				j=0;
				for( ;lpOpts && ' ' !=lpOpts[i] && ';'!=lpOpts[i]; apFullName->mChars[j++]=lpOpts[i++]);
				if( j && '\\' !=apFullName->mChars[j-1])
					apFullName->mChars[j++]='\\';
				apFullName->mChars[j]=0;
				String_StrAppend( apFullName->mChars, apShortName );
				/*	printf( "find: %s\n", apFullName->mChars); */
				if( File_Exists(apFullName->mChars))
					return 1;

				if( j )
				{
					j = String_StrLen( apFullName->mChars );
					for( k=0; k<=j; k++ )
						apFullName->mChars[ j+apParser->mPathPRJLen -k ] = apFullName->mChars[ j-k ];
					apFullName->mChars[ apParser->mPathPRJLen+j]=0;
					for( k=0; k<apParser->mPathPRJLen; k++ )
						apFullName->mChars[k]=apParser->mPathPRJ[k];
					/*	printf( "find: %s\n", apFullName->mChars); */
					if( File_Exists(apFullName->mChars))
						return 1;
				}

			} while(';'==lpOpts[i++]);
		}
	}

	return 0;
};

void	EmbeddedProgram_Execute( const sEmbeddedProgram * apProg, const char * apCommandLine )
{
	U32	lSize;
	U8 * lpData;

	lSize = apProg->mTextSize + apProg->mDataSize + apProg->mBssSize + sizeof(sBasePage);
	lpData = mMEMCALLOC(lSize);
	if( lpData )
	{
		U32 lOffset = sizeof(sBasePage) - sizeof(sProgramHeader);
		GodPack_DePack( apProg->mpData, lpData + lOffset );
		Program_Init( (sBasePage*)lpData);
		Program_Execute( (sBasePage*)lpData, apCommandLine );
		mMEMFREE(lpData);
	}
	else
	{
		printf( "couldn't allocate memory for prog\n");
	}
}


void	CopyOptions( const char * apSource, char * apDest )
{
	int i,j;
	for(i=0;'['!=apSource[i]&&apSource[i];i++);
	if('['==apSource[i]) i++;
	for(;' '==apSource[i]||'\t'==apSource[i];i++);
	for(j=0;']'!=apSource[i]&&apSource[i];apDest[j++]=apSource[i++]);
	for(;j>0&&(' '==apDest[j-1]||'\t'==apDest[j-1]);j--);
	apDest[j++]=' ';
	apDest[j]=0;
}

void	DebugLog_Update( sProjectParser * apParser )
{
	apParser->mDebugLog.mOffset = Bios_GetPipeOffset();
	if( apParser->mDebugLog.mOffset && apParser->mDebugLog.mOpenFlag )
	{
		if( apParser->mDebugLog.mOffset > 1024 )
			apParser->mDebugLog.mOffset = 1024;
		FileBackedBuffer_Write( &apParser->mDebugLog );
		Bios_ClearPipeOffset();
	}
}

void	ShowDTA( sGemDosDTA * apDTA )
{
	U16 lYear;
	U16	lMonth;
	U16 lDay;

	lYear = (apDTA->mDate >> 9) + 1980;
	lMonth = ( (apDTA->mDate >> 5) & 15);
	lDay = (apDTA->mDate & 31);
	printf( "%s %d-%d-%d\n", apDTA->mFileName, lYear, lMonth, lDay );
}

void	ParseLine(sProjectParser * apParser, char * apLine)
{
	sFileNameSpace	lFileName;

	if(';' != *apLine && *apLine)
	{
		if( '=' == *apLine )
		{
			apParser->mGather=1;
		}
		else
		{
			if( !apParser->mGather )
			{
				if( *apLine )
				{
					if( !apParser->mpExecutable && *apLine )
					{
						char lOut[256];
						U32 i;
						apParser->mpExecutable = apLine;

						String_StrCpy( lOut, apParser->mPathPRJ );
						String_StrCpy( &lOut[ apParser->mPathPRJLen ], apParser->mpExecutable );
						for( i=0; lOut[i]; i++ );
						for( ; i && '.'!=lOut[i-1]; i--);
						lOut[i++]='P';
						lOut[i++]='L';
						lOut[i++]='K';
						lOut[i++]=0;

						apParser->mObjectCount=0;

						FileBackedBuffer_Init( &apParser->mLinkerScript, lOut );

						FileBackedBuffer_StringAppend( &apParser->mLinkerScript, "-V -O=");
						String_StrCpy( lOut, apParser->mPathPRJ );
						String_StrCpy( &lOut[ apParser->mPathPRJLen ], apParser->mpExecutable );
						FileBackedBuffer_StringAppend( &apParser->mLinkerScript, lOut);

						FileBackedBuffer_Append( &apParser->mLinkerScript, 13 );
						FileBackedBuffer_Append( &apParser->mLinkerScript, 10 );
					}
					else if( '.' == *apLine)
					{
						if( 'C' == apLine[1] || 'c'==apLine[1])
						{
							CopyOptions( &apLine[2], &apParser->mCompilerOptions[0]);
							apParser->mCompilerOptionsLen = (U16)String_StrLen(&apParser->mCompilerOptions[0]);
						}
						else if( 'L' == apLine[1] || 's'==apLine[1])
						{
							CopyOptions( &apLine[2], &apParser->mLinkerOptions[0]);
							apParser->mLinkerOptionsLen = (U16)String_StrLen(&apParser->mLinkerOptions[0]);
							FileBackedBuffer_StringAppend( &apParser->mLinkerScript, apParser->mLinkerOptions);

							FileBackedBuffer_Append( &apParser->mLinkerScript, 13 );
							FileBackedBuffer_Append( &apParser->mLinkerScript, 10 );
						}
						else if( 'S' == apLine[1] || 's'==apLine[1])
						{
							CopyOptions( &apLine[2], &apParser->mAssemblerOptions[0]);
							apParser->mAssemblerOptionsLen = (U16)String_StrLen(&apParser->mAssemblerOptions[0]);
						}
					}
				}
			}
			else
			{
				char lCmdLine[ 256 ];
/*				char lPath[256];*/
				char * lpFile=apLine;
				char * lpOpts = 0;

				U32 lOff=0;
				for(;' '!=apLine[lOff]&&'\t'!=apLine[lOff]&&0!=apLine[lOff];lOff++);
				if( apLine[lOff] )
				{
					lpOpts = &apLine[lOff];
					*lpOpts=0;
				}
				while(lOff>0 && '.'!=apLine[lOff-1])lOff--;

				if( lpOpts )
				{
					if( 'C' == apLine[lOff] || 'c'==apLine[lOff])
					{
						CopyOptions( &apLine[lOff+1], &apParser->mCompilerOptions[apParser->mCompilerOptionsLen]);
					}
					else if( 'O' == apLine[lOff] || 'o'==apLine[lOff] || 'L'==apLine[lOff] || 'l' == apLine[lOff])
					{
						CopyOptions( &apLine[lOff+1], &apParser->mLinkerOptions[apParser->mLinkerOptionsLen]);
					}
					else if( 'S' == apLine[lOff] || 's'==apLine[lOff])
					{
						CopyOptions( &apLine[lOff+1], &apParser->mAssemblerOptions[apParser->mAssemblerOptionsLen]);
					}
				}

				if( !BuildExistingFilename(apParser,apLine,&lFileName))
				{
					printf("ERROR: Can't find file %s\n",apLine);
				}
				else
				{


/*
				lpFile = apLine;
				if( !File_Exists(apLine))
				{
					lpFile=&apParser->mPathPRJ[0];
					String_StrCpy( &apParser->mPathPRJ[ apParser->mPathPRJLen ], apLine );
					if( !File_Exists(lpFile))
					{
						String_StrCat( lPath, apParser->mpPureC, "\\" );
						String_StrCpy( lPath+String_StrLen(lPath), apLine);
						lpFile = lPath;
					}
					for(;' '!=lpFile[lOff]&&'\t'!=lpFile[lOff]&&0!=lpFile[lOff];lOff++);
					while(lOff>0 && '.'!=lpFile[lOff-1])lOff--;
					if( !File_Exists(lpFile))
					{
						printf("ERROR: Can't find file %s\n",apLine);
					}
				}
*/
					lpFile=lFileName.mChars;

					for(;' '!=lpFile[lOff]&&'\t'!=lpFile[lOff]&&0!=lpFile[lOff];lOff++);
					while(lOff>0 && '.'!=lpFile[lOff-1])lOff--;
					if( lpFile[lOff] == 'C' || lpFile[lOff] == 'c' || lpFile[lOff] == 'S' || lpFile[lOff] == 's')
					{
						if( 0 == GemDos_Fsfirst( lpFile, dGEMDOS_FA_READONLY | dGEMDOS_FA_ARCHIVE ) )
						{
							U32 lSrcDateTime;
							U16 lBuildFlag = 1;
							char lOld[2];

							lSrcDateTime = gDTA.mDate;
							lSrcDateTime <<= 16;
							lSrcDateTime |= gDTA.mTime;

							lOld[0]=lpFile[lOff];
							lOld[1]=lpFile[lOff+1];

							lpFile[lOff+0]='O';
							lpFile[lOff+1]=0;
							FileBackedBuffer_StringAppend( &apParser->mLinkerScript, lpFile );

							/*printf( "%d - %d -%s\n", gDTA.mDate, gDTA.mTime, gDTA.mFileName);*/
							if( 0 == GemDos_Fsfirst( lpFile, dGEMDOS_FA_READONLY | dGEMDOS_FA_ARCHIVE ) )
							{
								U32 lObjDateTime;
								lObjDateTime = gDTA.mDate;
								lObjDateTime <<= 16;
								lObjDateTime |= gDTA.mTime;

								if( lObjDateTime > apParser->mObjectDateTimeNewest )
									apParser->mObjectDateTimeNewest = lObjDateTime;

								lBuildFlag = 0;
								/*printf( "%d - %d -%s\n", gDTA.mDate, gDTA.mTime, gDTA.mFileName);*/
								if( lSrcDateTime >= lObjDateTime )
								{
									printf( "newer so building\n");
									lBuildFlag = 1;
								}
							}

							lpFile[lOff+0] = lOld[0];
							lpFile[lOff+1] = lOld[1];

/*							lBuildFlag=1;*/
							if( lBuildFlag )
							{
								apParser->mObjectBuiltCount++;

								if( 'C' == lpFile[lOff] || 'c'==lpFile[lOff])
								{
									sBasePage * lpH = 0;
									sprintf( &lCmdLine[0], "%s%s", &apParser->mCompilerOptions[0], lpFile);
									printf( "PCC.TTP %s\n",&lCmdLine[0]);
	#ifdef dEMBEDDED_PUREC
									EmbeddedProgram_Execute( &gPureCompiler, lCmdLine );
	#else
									lpH = Program_Load("PCC.TTP");
									Program_Execute(lpH,lCmdLine);
									Program_UnLoad(lpH);
	#endif
								}
								else if( 'S' == lpFile[lOff] || 's'==lpFile[lOff])
								{
									sBasePage * lpH = 0;
									sprintf( &lCmdLine[0], "%s%s", &apParser->mAssemblerOptions[0], lpFile);
									printf( "PASM.TTP %s\n",&lCmdLine[0]);
	#ifdef dEMBEDDED_PUREC
									EmbeddedProgram_Execute( &gPureAssembler, lCmdLine );
	#else
									lpH = Program_Load("PASM.TTP");
									Program_Execute(lpH,lCmdLine);
									Program_UnLoad(lpH);
	#endif
								}
							}
						}
						else
						{
							printf( "File not found: %s\n", lpFile );
						}

					}
					else
					{
						FileBackedBuffer_StringAppend( &apParser->mLinkerScript, lpFile);
					}

					FileBackedBuffer_Append( &apParser->mLinkerScript, 13 );
					FileBackedBuffer_Append( &apParser->mLinkerScript, 10 );
		
					DebugLog_Update( apParser );

					apParser->mAssemblerOptions[apParser->mAssemblerOptionsLen]=0;
					apParser->mCompilerOptions[apParser->mCompilerOptionsLen]=0;
					apParser->mLinkerOptions[apParser->mLinkerOptionsLen]=0;
					apParser->mPathPRJ[ apParser->mPathPRJLen ] = 0;
				}
			}
		}
	}
}


void	ParsePRJ(sProjectParser * apParser)
{
	char * lpText = apParser->mpPRJ;
	apParser->mGather=0;

	apParser->mAssemblerOptionsLen = 0;
	apParser->mCompilerOptionsLen = 0;
	apParser->mLinkerOptionsLen = 0;

	if( lpText )
	{
		U32 lSize=apParser->mSize;
		U32 lOffset = 0;
		U32 lLineStart = 0;
		/* parse a line */

		while( lOffset<lSize)
		{
			/* skip whitespace at start of line */
			for(;lOffset<lSize && (13==lpText[lOffset]||10==lpText[lOffset]||' '==lpText[lOffset]||'\t'==lpText[lOffset]||0==lpText[lOffset]);lOffset++);
			lLineStart = lOffset;
			for(;13!=lpText[lOffset] && 10 !=lpText[lOffset] && lpText[lOffset] && lOffset<lSize;lOffset++);
			for(;(lOffset>0)&&(' '==lpText[lOffset-1]||'\t'==lpText[lOffset-1]);lpText[--lOffset]=0);
			lpText[lOffset++]=0;
			ParseLine(apParser,&lpText[lLineStart]);
		}
	}
}

U32		PRJCountLines(sProjectParser * apParser)
{
	U32 lCount = 0;
	U8 lLast = 0;
	U32 i=0;
	for( i=0; i<apParser->mSize; i++ )
	{
		U8 c = apParser->mpPRJ[i];
		if( (13==c) || (10 ==c) )
		{
			if( (lLast==c) || ((13!=lLast) && (10!=lLast)) )
			{
				lCount++;
			}
			else
			{
				c=0;
			}
		}
		lLast=c;
	}
	return(lCount);
}

void	Link( sProjectParser * apParser )
{
	char lOut[ 256 ];
	U32 i;
	U16 lLinkFlag = 0;

	sprintf( lOut, "-V -c=%s%s", apParser->mPathPRJ, apParser->mpExecutable );
	if( apParser->mObjectBuiltCount || GemDos_Fsfirst( &lOut[6], dGEMDOS_FA_READONLY | dGEMDOS_FA_ARCHIVE ) )
	{
		lLinkFlag = 1;
	}
	else
	{
		U32 lExeDateTime;

		lLinkFlag = 0;
		lExeDateTime = gDTA.mDate;
		lExeDateTime <<= 16;
		lExeDateTime |= gDTA.mTime;

		if( lExeDateTime < apParser->mObjectDateTimeNewest )
		{
			printf( "objs %lx newer than exe %lx so linking\n", apParser->mObjectDateTimeNewest, lExeDateTime);
			lLinkFlag = 1;
		}
	}
	if( lLinkFlag )
	{

		for( i=0; lOut[i]; i++ );
		for( ; i && '.'!=lOut[i-1]; i--);
		lOut[i++]='P';
		lOut[i++]='L';
		lOut[i++]='K';
		lOut[i++]=0;


		printf( "PLINK.TTP %s\n",&lOut[0]);
#ifdef dEMBEDDED_PUREC
		EmbeddedProgram_Execute( &gPureLinker, lOut );
#else
		{
			sBasePage * lpPage;
			lpPage = Program_Load("PLINK.TTP");
			Program_Execute(lpPage,lOut);
			Program_UnLoad(lpPage);
		}
#endif
	}
	else
	{
		printf( "Up to date, no linking.\n");
	}
	DebugLog_Update( apParser );
}


void	ProcessPRJ(sProjectParser * apParser, const char * apFileName )
{
	char lPath[ 512 ];
	char lOld;
	printf( "Process: %s\n",apFileName);

	Drive_GetPath( 0, lPath );

	for(apParser->mPathPRJLen=0; apFileName[ apParser->mPathPRJLen ]; apParser->mPathPRJ[ apParser->mPathPRJLen ] = apFileName[ apParser->mPathPRJLen ],apParser->mPathPRJLen++);
	for(; (apParser->mPathPRJLen) > 0 && ('\\' != apParser->mPathPRJ[ apParser->mPathPRJLen-1 ]) && ('/' != apParser->mPathPRJ[ apParser->mPathPRJLen-1 ]) ;apParser->mPathPRJLen-- );

	lOld = apParser->mPathPRJ[ apParser->mPathPRJLen ];
	apParser->mPathPRJ[ apParser->mPathPRJLen ] = 0;
	Drive_SetPath( apParser->mPathPRJ);
	apParser->mPathPRJ[ apParser->mPathPRJLen ] = lOld;

	apParser->mAssemblerOptionsLen=0;
	apParser->mCompilerOptionsLen=0;
	apParser->mLinkerOptionsLen=0;

	apParser->mpFileName = apFileName;
	apParser->mpExecutable=0;

	apParser->mObjectBuiltCount = 0;
	apParser->mObjectDateTimeNewest = 0;

/*
	String_StrCpy( &apParser->mCompilerOptions[0], "-I" );
	String_StrCat( &apParser->mCompilerOptions[0], &apParser->mCompilerOptions[0], "INCLUDE " );
*/
	apParser->mCompilerOptionsLen=(U16)String_StrLen(&apParser->mCompilerOptions[0]);

	if( File_Exists(apFileName))
	{
		apParser->mSize = File_GetSize(apFileName);
		apParser->mpPRJ = mMEMALLOC( apParser->mSize +1);
		if( apParser->mpPRJ )
		{
			char lOut[ 256 ];
			File_LoadAt(apFileName,apParser->mpPRJ);
			apParser->mpPRJ[apParser->mSize]=0;

			FileBackedBuffer_Init( &apParser->mLinkerScript, lOut );
			ParsePRJ(apParser);
			FileBackedBuffer_DeInit( &apParser->mLinkerScript );
			Link(apParser);

			mMEMFREE(apParser->mpPRJ);
		}
	}
	else
	{
		printf( "error\n");
		printf( "file %s does not exist", apFileName);
	}
	Drive_SetPath(lPath);
}

void	ProcessPBT( sProjectParser * apParser, const char * apFileName )
{
	U32 lSize = File_GetSize(apFileName);
	U32 lOffset = 0;
	char * lpText;


	lpText = mMEMCALLOC( lSize+1 );
	File_LoadAt(apFileName,lpText);
	while( lOffset<lSize)
	{
		U32 lLineStart = 0;
		U32 lLineLen = 0;

		/* skip whitespace at start of line */
		for(;lOffset<lSize && (13==lpText[lOffset]||10==lpText[lOffset]||' '==lpText[lOffset]||'\t'==lpText[lOffset]||0==lpText[lOffset]);lOffset++);
		lLineStart = lOffset;
		for(;13!=lpText[lOffset] && 10 !=lpText[lOffset] && lpText[lOffset] && lOffset<lSize;lOffset++);
		for(;(lOffset>0)&&(' '==lpText[lOffset-1]||'\t'==lpText[lOffset-1]);lpText[--lOffset]=0);
		if( lOffset<=lSize)
		{
			lpText[lOffset++]=0;
		}
		else
		{

			printf("end");
		}

		lLineLen = lOffset-lLineStart;
		if( (lLineLen > 1) && ';' !=lpText[lLineStart] )
		{
			if( lLineLen > 3 && ('-'==lpText[lLineStart] && '='==lpText[lLineStart+2]) )
			{
				U32 i=0;
				if( 'A' == lpText[lLineStart+1])
				{
					for( i=lLineStart+2; i<lOffset && '='!=lpText[i]; i++ );
					if( '=' == lpText[i])
					{
						for( i++; i<lOffset && (' '==lpText[i] || '\t' ==lpText[i]); i++);
						apParser->mpPureC = &lpText[i];
#ifndef dEMBEDDED_PUREC
						Drive_SetPath( &lpText[i] );
#endif
					}
				}
				else if( 'B' == lpText[lLineStart+1])
				{
					if( apParser->mDebugLog.mOpenFlag ) 
					{
						FileBackedBuffer_DeInit( &apParser->mDebugLog );
						Bios_UnPipeConsole();
					}
					for( i=lLineStart+2; i<lOffset && '='!=lpText[i]; i++ );
					if( '=' == lpText[i])
					{
						for( i++; i<lOffset && (' '==lpText[i] || '\t' ==lpText[i]); i++);
						String_StrCpy( &apParser->mPathPRJ[ apParser->mPathPRJLen ], &lpText[i] );
						FileBackedBuffer_Init( &apParser->mDebugLog, &apParser->mPathPRJ[0] );
						Bios_PipeConsole( &apParser->mDebugLog.mBuffer[0], sizeof(apParser->mDebugLog.mBuffer) );
					}

				}
				else
				{
					printf( "Warning: unknown option %s\n", &lpText[lLineStart]);
				}
			}
			else
			{
				U32 i;
				U32 lOldLen = apParser->mPathPRJLen;
				
				for( i=lLineStart; i<=lOffset; apParser->mPathPRJ[ (i-lLineStart) + apParser->mPathPRJLen ] = lpText[ i ],i++);
				
				for( i=apParser->mPathPRJLen; apParser->mPathPRJ[i]; i++);
				for( ;i>0 && '\\' != apParser->mPathPRJ[ i-1] && '/' != apParser->mPathPRJ[i-1]; i-- );
				apParser->mPathPRJLen=i;

				ProcessPRJ( apParser, apParser->mPathPRJ);

				apParser->mPathPRJLen =lOldLen;
			}
		}

	}

	if( apParser->mDebugLog.mOpenFlag ) 
	{
		FileBackedBuffer_DeInit( &apParser->mDebugLog );
		Bios_UnPipeConsole();
	}

	mMEMFREE(lpText);

}

int		main( int argc, char **argv)
{
	if( argc > 1 )
	{
		sProjectParser	lParser;
		gpOldDTA = GemDos_Fgetdta();
		GemDos_Fsetdta(&gDTA);
		Memory_Clear( sizeof(sProjectParser), &lParser );
		lParser.mPathPRJ[0]=Drive_GetDrive() + 'A';
		lParser.mPathPRJ[1]=':';
		Drive_GetPath( 0, &lParser.mPathPRJ[2]);
		for(lParser.mPathPRJLen=0; lParser.mPathPRJ[ lParser.mPathPRJLen ]; lParser.mPathPRJLen++ );
		if( lParser.mPathPRJLen && '\\' != lParser.mPathPRJ[ lParser.mPathPRJLen-1])
			lParser.mPathPRJ[ lParser.mPathPRJLen++ ]= '\\';
		ProcessPBT( &lParser, argv[1]);
		Drive_SetPath( &lParser.mPathPRJ[0]);
		GemDos_Fsetdta(gpOldDTA);
	}
	else
	{
		printf( "PUREBOT\n");
		printf( "[c] 2018 Reservoir Gods\n");
		printf( "usage: purebot <file.pbt>\n");
	}


#if 0
	Drive_GetPath( 0, &lPath[0]);

#if defined(dGODLIB_PLATFORM_WIN)
	Drive_SetPath("C:\\ATARI_HD\\TOOLS\\CODE\\PURE_C");
	lpH = Program_Load("PCC.TTP");
	/*Program_Execute( lpH, "hello world");*/

	ProcessPRJ(&lParser,"C:\\ATARI_HD\\SOURCE\\GITHUB\\GODLIB.SPL\\BOX\\BOX.PRJ");


#else
	Drive_SetPath("C:\\HD\\TOOLS\\CODE\\PURE_C");
	/* lpH = Program_Load("PCC.TTP");
	Program_Execute( lpH, "-V -IINCLUDE INCLUDE\\GODLIB\\AUDIO\\AUDIO.C"); */

	ProcessPRJ(&lParser,"C:\\HD\\SOURCE\\GITHUB\\GODLIB.SPL\\BOX\\BOX.PRJ");
#endif
	Drive_SetPath( &lPath[0]);

#endif

	return( 0 );
}