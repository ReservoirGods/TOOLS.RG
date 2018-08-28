/* 
Special character ;: is extension point for PRJ format

;:-A=<pure_c dir>
;:-B=<log_output>
;:-K=Continuous Building mode
;:-V

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
extern unsigned char PASM_Array[];
extern unsigned char PCC_Array[];
extern unsigned char PLINK_Array[];

typedef struct sEmbeddedProgram
{
	char *	mpName;
	U32		mTextSize;
	U32		mDataSize;
	U32		mBssSize;
	void*	mpData;
} sEmbeddedProgram;

sEmbeddedProgram	gPureCompiler =
{
	"PCC.TTP",
	0x19e38,
	0x3db4,
	0x72e0,
	PCC_Array
};

sEmbeddedProgram	gPureAssembler =
{
	"PASM.TTP",
	0x0c838,
	0x330c,
	0x516a,
	PASM_Array
};

sEmbeddedProgram	gPureLinker =
{
	"PLINK.TTP",
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
	const char *				mpExecutable;
	const char *				mpFileName;
	const char *				mpPureC;
	char *						mpPRJ;
	U32							mSize;
	U32							mObjectCount;
	U32							mObjectDateTimeNewest;
	U16							mObjectBuiltCount;
	U16							mPathPRJLen;
	U16							mAssemblerOptionsLen;
	U16							mCompilerOptionsLen;
	U16							mLinkerOptionsLen;
	U8							mGather;
	U8							mVerboseFlag;
	U8							mContinuousBuildFlag;
	U8							mPadFlag;
	char						mPathPRJ[512];
	char						mAssemblerOptions[128];
	char						mCompilerOptions[128];
	char						mLinkerOptions[128];
	sFileBackedBuffer			mLinkerScript;
	sFileBackedBuffer			mDebugLog;
	struct sProjectParser	*	mpParent;
} sProjectParser;

sGemDosDTA	gDTA;
sGemDosDTA	* gpOldDTA = 0;

void	ProcessPRJ(sProjectParser * apParser, const char * apFileName );

char *	FileName_GetpExt( char * apFileName )
{
	U16 lLen = String_StrLen(apFileName);
	while( lLen > 0 && '.' != apFileName[ lLen-1] )
		lLen--;
	return( lLen ? &apFileName[ lLen-1 ] : 0 );
}

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


int		BuildExistingFilename( sProjectParser * apParser, const char * apShortName, sFileNameSpace * apFullName )
{
	S16 i,j;
	char * lpOpts = apParser->mCompilerOptions;
#ifndef dGODLIB_PLATFORM_ATARI
	char lPath[512];
	Drive_GetPath(0,lPath);
	printf( "curdir: %s\n",lPath);
#endif

	String_StrCpy( apFullName->mChars, apShortName );
	if( apParser->mVerboseFlag )
		printf( "find: %s\n", apFullName->mChars);
	if( File_Exists(apFullName->mChars ) ) 
		return 1;

	String_StrCpy( apFullName->mChars, apParser->mPathPRJ );
	String_StrCpy( &apFullName->mChars[ apParser->mPathPRJLen ], apShortName );
	if( apParser->mVerboseFlag )
		printf( "find: %s\n", apFullName->mChars);
	if( File_Exists(apFullName->mChars ) ) 
		return 1;

	if( apParser->mpPureC )
	{
		String_StrCpy( apFullName->mChars, apParser->mpPureC );
		String_StrAppend2( apFullName->mChars, "\\", apShortName );
		if( apParser->mVerboseFlag )
			printf( "find: %s\n", apFullName->mChars);
		if( File_Exists(apFullName->mChars ) ) 
			return 1;
	}

	Drive_GetPath( 0, apFullName->mChars );
	String_StrAppend2( apFullName->mChars, "\\", apShortName );
	if( apParser->mVerboseFlag )
		printf( "find: %s\n", apFullName->mChars);
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
				if( apParser->mVerboseFlag )
					printf( "find: %s\n", apFullName->mChars);
				if( File_Exists(apFullName->mChars))
					return 1;

				if( j )
				{
					j = (S16)String_StrLen( apFullName->mChars );
					for( k=0; k<=j; k++ )
						apFullName->mChars[ j+apParser->mPathPRJLen -k ] = apFullName->mChars[ j-k ];
					apFullName->mChars[ apParser->mPathPRJLen+j]=0;
					for( k=0; k<apParser->mPathPRJLen; k++ )
						apFullName->mChars[k]=apParser->mPathPRJ[k];
					if( apParser->mVerboseFlag )
						printf( "find: %s\n", apFullName->mChars);
					if( File_Exists(apFullName->mChars))
						return 1;
				}

			} while(';'==lpOpts[i++]);
		}
	}

	return 0;
};


void	EmbeddedProgram_Execute( sProjectParser * apParser, const sEmbeddedProgram * apProg, const char * apCommandLine )
{
	U32	lSize;
	U8 * lpData;

	if( apParser->mVerboseFlag )
		printf( "Executing: %s %s\n", apProg->mpName, apCommandLine);

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


void	OptionsCopy( const char * apSource, char * apDest, U16 aDstLen )
{
	U16 i,j;
	for(i=0;'['!=apSource[i]&&apSource[i];i++);
	if('['==apSource[i]) i++;
	for(;' '==apSource[i]||'\t'==apSource[i];i++);
	for(j=0;']'!=apSource[i]&&apSource[i] && (j<aDstLen-1);apDest[j++]=apSource[i++]);
	for(;j>0&&(' '==apDest[j-1]||'\t'==apDest[j-1]);j--);
	apDest[j++]=' ';
	apDest[j]=0;
}

void	DebugLog_InternalUpdate( sProjectParser * apParser )
{
	if( apParser )
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
}

void	DebugLog_Update( sProjectParser * apParser )
{
	sProjectParser * lpParser = apParser;

	for( ; lpParser && !lpParser->mDebugLog.mOpenFlag; lpParser = lpParser->mpParent );
	DebugLog_InternalUpdate( lpParser );
}

void	DebugLog_Complete( sProjectParser * apParser )
{
	DebugLog_Update( apParser );
	if( apParser->mDebugLog.mOpenFlag )
	{
		sProjectParser * lpParser = apParser->mpParent;

		for( ; lpParser && !lpParser->mDebugLog.mOpenFlag; lpParser = lpParser->mpParent );
		if( lpParser )
		{
			U32 lBytes = 0;
			char lBuffer[ 256 ];
			File_SeekFromStart( apParser->mDebugLog.mFileHandle, 0 );
			do
			{
				lBytes = File_Read( apParser->mDebugLog.mFileHandle, 256, lBuffer );
				File_Write( lpParser->mDebugLog.mFileHandle, lBytes, lBuffer );
			} while( 256 == lBytes );
			File_SeekFromEnd( apParser->mDebugLog.mFileHandle, 0 );
		}
		FileBackedBuffer_DeInit( &apParser->mDebugLog );
		Bios_UnPipeConsole();
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

	/* check for our cheeky PRJ extension */
	if( ';'== *apLine && ':' == apLine[1])
	{
		U16 lOff = 0;
		for( lOff=2; apLine[lOff] && '-'!=apLine[lOff]; lOff++);
		if( '-' == apLine[lOff])
		{
			lOff++;
			if( 'B'==apLine[lOff] && '=' == apLine[lOff+1])
			{
				if( apParser->mDebugLog.mOpenFlag ) 
				{
					DebugLog_Complete( apParser );
				}

				String_StrCpy( &apParser->mPathPRJ[ apParser->mPathPRJLen ], &apLine[lOff+2] );
				FileBackedBuffer_Init( &apParser->mDebugLog, &apParser->mPathPRJ[0] );
				Bios_PipeConsole( &apParser->mDebugLog.mBuffer[0], sizeof(apParser->mDebugLog.mBuffer) );
			}
			else if( 'K'==apLine[lOff])
			{
				/* only allow continuous build if no parents are currently continuously building */
				sProjectParser * lpP = apParser->mpParent;
				for( ; lpP && !lpP->mContinuousBuildFlag; lpP=lpP->mpParent );
				if( !lpP )
					apParser->mContinuousBuildFlag=1;
			}
			else if( 'V'==apLine[lOff])
			{
				apParser->mVerboseFlag=1;
			}

		}
	}
	else if(';' != *apLine && *apLine)
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
						char * lpExt;
						char lOut[256];
	
						String_StrCpy( lOut, apParser->mPathPRJ );
						String_StrCpy( &lOut[ apParser->mPathPRJLen ], apLine );
						lpExt = FileName_GetpExt(lOut);
						if( !String_StrCmpi(lpExt,".PRJ"))
						{
							ProcessPRJ( apParser, apLine );
						}
						else
						{
							apParser->mpExecutable = apLine;
							String_StrCpy(lpExt,".PLK");

							apParser->mObjectCount=0;

							FileBackedBuffer_Init( &apParser->mLinkerScript, lOut );

							FileBackedBuffer_StringAppend( &apParser->mLinkerScript, "-V -O=");
							String_StrCpy( lOut, apParser->mPathPRJ );
							String_StrCpy( &lOut[ apParser->mPathPRJLen ], apParser->mpExecutable );
							FileBackedBuffer_StringAppend( &apParser->mLinkerScript, lOut);

							FileBackedBuffer_Append( &apParser->mLinkerScript, 13 );
							FileBackedBuffer_Append( &apParser->mLinkerScript, 10 );
						}
					}
					else if( '.' == *apLine)
					{
						if( 'C' == apLine[1] || 'c'==apLine[1])
						{
							OptionsCopy( &apLine[2], apParser->mCompilerOptions,sizeof(apParser->mCompilerOptions));
							apParser->mCompilerOptionsLen = (U16)String_StrLen(&apParser->mCompilerOptions[0]);
						}
						else if( 'L' == apLine[1] || 's'==apLine[1])
						{
							OptionsCopy( &apLine[2], apParser->mLinkerOptions, sizeof(apParser->mLinkerOptions ));
							apParser->mLinkerOptionsLen = (U16)String_StrLen(&apParser->mLinkerOptions[0]);
							FileBackedBuffer_StringAppend( &apParser->mLinkerScript, apParser->mLinkerOptions);

							FileBackedBuffer_Append( &apParser->mLinkerScript, 13 );
							FileBackedBuffer_Append( &apParser->mLinkerScript, 10 );
						}
						else if( 'S' == apLine[1] || 's'==apLine[1])
						{
							OptionsCopy( &apLine[2], apParser->mAssemblerOptions, sizeof(apParser->mAssemblerOptions) );
							apParser->mAssemblerOptionsLen = (U16)String_StrLen(&apParser->mAssemblerOptions[0]);
						}
					}
				}
			}
			else
			{
				char lCmdLine[ 256 ];
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
						OptionsCopy( &apLine[lOff+1], &apParser->mCompilerOptions[apParser->mCompilerOptionsLen], sizeof(apParser->mCompilerOptions) - apParser->mCompilerOptionsLen);
					}
					else if( 'O' == apLine[lOff] || 'o'==apLine[lOff] || 'L'==apLine[lOff] || 'l' == apLine[lOff])
					{
						OptionsCopy( &apLine[lOff+1], &apParser->mLinkerOptions[apParser->mLinkerOptionsLen], sizeof(apParser->mLinkerOptions)-apParser->mLinkerOptionsLen);
					}
					else if( 'S' == apLine[lOff] || 's'==apLine[lOff])
					{
						OptionsCopy( &apLine[lOff+1], &apParser->mAssemblerOptions[apParser->mAssemblerOptionsLen], sizeof(apParser->mAssemblerOptions)-apParser->mAssemblerOptionsLen);
					}
				}

				if( !BuildExistingFilename(apParser,apLine,&lFileName))
				{
					printf("ERROR: Can't find file %s\n",apLine);
				}
				else
				{

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

								/* printf( "%lx %s\n", lObjDateTime, gDTA.mFileName);*/
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
									EmbeddedProgram_Execute( apParser, &gPureCompiler, lCmdLine );
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
									EmbeddedProgram_Execute( apParser, &gPureAssembler, lCmdLine );
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


void	Link( sProjectParser * apParser )
{
	char lOut[ 256 ];
	U32 i;
	U16 lLinkFlag = 0;

	if( !apParser->mpExecutable )
	{
		return;
	}

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
		EmbeddedProgram_Execute( apParser, &gPureLinker, lOut );
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
	char * lpFileName;
	char lPath[ 512 ];
	char lOld;
	sProjectParser	lParser;

	printf( "Process: %s\n",apFileName);

	Drive_GetPath( 0, lPath );

	do
	{
		Memory_Clear( sizeof(sProjectParser), &lParser );
		lParser.mpParent = apParser;
		if( ':' == apFileName[1])
		{
			String_StrCpy( lParser.mPathPRJ, apFileName );
		}
		else
		{
			String_StrCpy( lParser.mPathPRJ, apParser->mPathPRJ );
			String_StrCpy( &lParser.mPathPRJ[ apParser->mPathPRJLen ], apFileName );
		}

		lParser.mPathPRJLen = String_StrLen( lParser.mPathPRJ );
		for(; (lParser.mPathPRJLen) > 0 && ('\\' != lParser.mPathPRJ[ lParser.mPathPRJLen-1 ]) && ('/' != lParser.mPathPRJ[ lParser.mPathPRJLen-1 ]) ;lParser.mPathPRJLen-- );

		lOld = lParser.mPathPRJ[ lParser.mPathPRJLen ];
		lParser.mPathPRJ[ lParser.mPathPRJLen ] = 0;
		Drive_SetPath( lParser.mPathPRJ);
		lParser.mPathPRJ[ lParser.mPathPRJLen ] = lOld;

		lParser.mAssemblerOptionsLen=0;
		lParser.mCompilerOptionsLen=0;
		lParser.mLinkerOptionsLen=0;

		lParser.mpFileName = apFileName;
		lParser.mpExecutable=0;

		lParser.mObjectBuiltCount = 0;
		lParser.mObjectDateTimeNewest = 0;

		lParser.mCompilerOptionsLen=(U16)String_StrLen(&lParser.mCompilerOptions[0]);

		lpFileName = &lParser.mPathPRJ[ lParser.mPathPRJLen ];
		if( File_Exists(lpFileName))
		{
			lParser.mSize = File_GetSize(lpFileName);
			lParser.mpPRJ = mMEMALLOC( lParser.mSize +1);
			if( lParser.mpPRJ )
			{
				File_LoadAt(lpFileName,lParser.mpPRJ);
				lParser.mpPRJ[lParser.mSize]=0;
				ParsePRJ(&lParser);
				FileBackedBuffer_DeInit( &lParser.mLinkerScript );
				Link(&lParser);

				mMEMFREE(lParser.mpPRJ);
			}
		}
		else
		{
			printf( "error\n");
			printf( "file %s does not exist", apFileName);
		}
		DebugLog_Complete( &lParser );
		if( apParser && apParser->mDebugLog.mOpenFlag )
		{
			Bios_PipeConsole( &apParser->mDebugLog.mBuffer[0], sizeof(apParser->mDebugLog.mBuffer) );
		}
	} while( lParser.mContinuousBuildFlag );

	Drive_SetPath(lPath);
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
		ProcessPRJ( &lParser, argv[1]);
		Drive_SetPath( &lParser.mPathPRJ[0]);
		GemDos_Fsetdta(gpOldDTA);
	}
	else
	{
		printf( "PUREBOT\n");
		printf( "[c] 2018 Reservoir Gods\n");
		printf( "usage: purebot <file.pbt>\n");
	}


	printf( "\nDone.");
	GemDos_Cnecin();

	return( 0 );
}