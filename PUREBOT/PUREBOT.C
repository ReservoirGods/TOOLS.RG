/* TODO: optional verbosity
 get PRJ from command line
 process multiple PRJs
 intercept bios and output text to debug log

 parses PBT file
 -C=<pure_c dir>
 -O=<log_output>
 .PRJ
*/

#include <GODLIB\DRIVE\DRIVE.H>
#include <GODLIB\FILE\FILE.H>
#include <GODLIB\MEMORY\MEMORY.H>
#include <GODLIB\PROGRAM\PROGRAM.H>
#include <GODLIB\STRING\STRING.H>

typedef struct sProjectParser
{
	const char *	mpExecutable;
	const char *	mpFileName;
	const char *	mpPureC;
	const char *	mpDebugOutFile;
	char *			mpPRJ;
	U32				mSize;
	U32				mLinkScriptSize;
	U32				mLinkScriptOffset;
	U32				mObjectCount;
	U16				mPathPRJLen;
	U16				mAssemblerOptionsLen;
	U16				mCompilerOptionsLen;
	U16				mLinkerOptionsLen;
	U8				mGather;
	char **			mppObjs;
	char *			mpLinkScript;
	char			mPathPRJ[512];
	char			mAssemblerOptions[512];
	char			mCompilerOptions[512];
	char			mLinkerOptions[512];
} sProjectParser;

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

void	ParseLine(sProjectParser * apParser, char * apLine)
{
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
						apParser->mpExecutable = apLine;

						sprintf( &apParser->mpLinkScript[ apParser->mLinkScriptOffset ], "-V -O=%s%s", apParser->mPathPRJ, apParser->mpExecutable);
						for(;apParser->mpLinkScript[apParser->mLinkScriptOffset];apParser->mLinkScriptOffset++);
						apParser->mpLinkScript[apParser->mLinkScriptOffset++]=13;
						apParser->mpLinkScript[apParser->mLinkScriptOffset++]=10;
					}
					else if( '.' == *apLine)
					{
						if( 'C' == apLine[1] || 'c'==apLine[1])
						{
							CopyOptions( &apLine[2], &apParser->mCompilerOptions[apParser->mCompilerOptionsLen]);
							apParser->mCompilerOptionsLen = (U16)String_StrLen(&apParser->mCompilerOptions[0]);
						}
						else if( 'L' == apLine[1] || 's'==apLine[1])
						{
							CopyOptions( &apLine[2], &apParser->mLinkerOptions[0]);
							apParser->mLinkerOptionsLen = (U16)String_StrLen(&apParser->mLinkerOptions[0]);
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
				const char * lpFile=apLine;
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


				if( !File_Exists(apLine))
				{
					lpFile=&apParser->mPathPRJ[0];
					String_StrCpy( &apParser->mPathPRJ[ apParser->mPathPRJLen ], apLine );
					if( !File_Exists(lpFile))
					{
						printf("ERROR: Can't find file %s\n",apLine);
					}
				}


				if( 'C' == apLine[lOff] || 'c'==apLine[lOff])
				{
					sBasePage * lpH = 0;
					sprintf( &lCmdLine[0], "-V %s%s", &apParser->mCompilerOptions[0], lpFile);
					printf( "PCC.TTP %s\n",&lCmdLine[0]);
					lpH = Program_Load("PCC.TTP");
					Program_Execute(lpH,lCmdLine);
					Program_UnLoad(lpH);
				}
				else if( 'S' == apLine[lOff] || 's'==apLine[lOff])
				{
					sBasePage * lpH = 0;
					sprintf( &lCmdLine[0], "-V %s%s", &apParser->mAssemblerOptions[0], lpFile);
					lpH = Program_Load("PASM.TTP");
					printf( "PASM.TTP %s\n",&lCmdLine[0]);
					Program_Execute(lpH,lCmdLine);
					Program_UnLoad(lpH);
				}

				{
					U16 k;
					for( lOff=0; lpFile[lOff]; lOff++);
					for( ;lOff && '.'!=lpFile[lOff-1];lOff--);
					for( k=0; k<lOff; apParser->mpLinkScript[ apParser->mLinkScriptOffset++ ] = lpFile[ k++ ] );
					if( lpFile[lOff] == 'C' || lpFile[lOff] == 'c' || lpFile[lOff] == 'S' || lpFile[lOff] == 's')
					{
						apParser->mpLinkScript[apParser->mLinkScriptOffset++] = 'O';
					}
					else
					{
						for( ; lpFile[k]; apParser->mpLinkScript[ apParser->mLinkScriptOffset++]=lpFile[k++] );
					}
					apParser->mpLinkScript[ apParser->mLinkScriptOffset++ ] = 13;
					apParser->mpLinkScript[ apParser->mLinkScriptOffset++ ] = 10;
				}

				apParser->mAssemblerOptions[apParser->mAssemblerOptionsLen]=0;
				apParser->mCompilerOptions[apParser->mCompilerOptionsLen]=0;
				apParser->mLinkerOptions[apParser->mLinkerOptionsLen]=0;
				apParser->mPathPRJ[ apParser->mPathPRJLen ] = 0;
			}
		}
	}
}


void	ParsePRJ(sProjectParser * apParser)
{
	char * lpText = apParser->mpPRJ;
	apParser->mGather=0;
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
	sBasePage * lpPage;


	sprintf( lOut, "-V -c=%s%s", apParser->mPathPRJ, apParser->mpExecutable );
	for( i=0; lOut[i]; i++ );
	for( ; i && '.'!=lOut[i-1]; i--);
	lOut[i++]='P';
	lOut[i++]='L';
	lOut[i++]='K';
	lOut[i++]=0;

	File_Save( &lOut[6], apParser->mpLinkScript, apParser->mLinkScriptOffset );

	lpPage = Program_Load("PLINK.TTP");
	printf( "PLINK.TTP %s\n",&lOut[0]);
	Program_Execute(lpPage,lOut);
	Program_UnLoad(lpPage);
}


void	ProcessPRJ(sProjectParser * apParser, const char * apFileName )
{
	printf( "Process: %s\n",apFileName);

/*
	for(i=0;apFileName[i];i++) 
		apParser->mPathPRJ[i]=apFileName[i];
	for(;i>0 && '\\'!=apFileName[i-1] && '/' !=apFileName[i-1];i--);
	apParser->mPathPRJ[i]=0;
	apParser->mPathPRJLen = i;
*/
	apParser->mAssemblerOptionsLen=0;
	apParser->mCompilerOptionsLen=0;
	apParser->mLinkerOptionsLen=0;

	apParser->mpFileName = apFileName;
	apParser->mpExecutable=0;

	String_StrCpy( &apParser->mCompilerOptions[0], "-I" );
/*	String_StrCat( &apParser->mCompilerOptions[0], &apParser->mCompilerOptions[0], &apParser->mPathPRJ[0] );*/
	String_StrCat( &apParser->mCompilerOptions[0], &apParser->mCompilerOptions[0], "INCLUDE " );
	apParser->mCompilerOptionsLen=(U16)String_StrLen(&apParser->mCompilerOptions[0]);

	if( File_Exists(apFileName))
	{
		apParser->mSize = File_GetSize(apFileName);
		apParser->mpPRJ = mMEMALLOC( apParser->mSize +1);
		if( apParser->mpPRJ )
		{
			U32 lLC;
			File_LoadAt(apFileName,apParser->mpPRJ);
			apParser->mpPRJ[apParser->mSize]=0;

			lLC = PRJCountLines(apParser);
			apParser->mLinkScriptSize = (lLC) * apParser->mPathPRJLen;
			apParser->mLinkScriptSize += apParser->mSize;
			apParser->mLinkScriptSize += 6;
			apParser->mpLinkScript = mMEMALLOC( apParser->mLinkScriptSize );
			apParser->mLinkScriptOffset = 0;

			apParser->mppObjs = mMEMALLOC( (lLC+1) * sizeof(char*));

			apParser->mObjectCount=0;
			ParsePRJ(apParser);
			Link(apParser);

			mMEMFREE( apParser->mpLinkScript);
			mMEMFREE( apParser->mppObjs);
			mMEMFREE(apParser->mpPRJ);
		}
	}
	else
	{
		printf( "%s does not exist", apFileName);
	}
}

void	ProcessPBT( sProjectParser * apParser, const char * apFileName )
{
	U32 lSize = File_GetSize(apFileName);
	U32 lOffset = 0;
	char * lpText;


	lpText = File_Load(apFileName);

	while( lOffset<lSize)
	{
		U32 lLineStart = 0;
		U32 lLineLen = 0;

		/* skip whitespace at start of line */
		for(;lOffset<lSize && (13==lpText[lOffset]||10==lpText[lOffset]||' '==lpText[lOffset]||'\t'==lpText[lOffset]||0==lpText[lOffset]);lOffset++);
		lLineStart = lOffset;
		for(;13!=lpText[lOffset] && 10 !=lpText[lOffset] && lpText[lOffset] && lOffset<lSize;lOffset++);
		for(;(lOffset>0)&&(' '==lpText[lOffset-1]||'\t'==lpText[lOffset-1]);lpText[--lOffset]=0);
		lpText[lOffset++]=0;

		lLineLen = lOffset-lLineStart;
		if( lLineLen > 1 )
		{
			printf( "doing %s\n", &lpText[ lLineStart]);
			if( lLineLen > 3 && ('-'==lpText[lLineStart] && '='==lpText[lLineStart+2]) )
			{
				U32 lFileOff=0;
				for( lFileOff=3; ' '==lpText[lFileOff] || '\t'==lpText[lFileOff]; lFileOff++ );
				if( 'A' == lpText[lLineStart+1])
				{
					apParser->mpPureC = &lpText[lFileOff];
					Drive_SetPath( &lpText[lFileOff] );
				}
				else if( 'B' == lpText[lLineStart+1])
				{
					apParser->mpDebugOutFile = &lpText[lFileOff];
				}
				else
				{
					printf( "Warning: unkown option %s\n", &lpText[lLineStart]);
				}
			}
			else
			{
				U32 i;
				for( i=lLineStart; i<=lOffset; apParser->mPathPRJ[ (i-lLineStart) + apParser->mPathPRJLen ] = lpText[ i ],i++);
				ProcessPRJ( apParser, apParser->mPathPRJ);
			}
		}

	}

	File_UnLoad(lpText);

}

int main( int argc, char **argv)
{
	if( argc > 1 )
	{
		sProjectParser	lParser;
		Memory_Clear( sizeof(sProjectParser), &lParser );
		lParser.mPathPRJ[0]=Drive_GetDrive() + 'A';
		lParser.mPathPRJ[1]=':';
		Drive_GetPath( 0, &lParser.mPathPRJ[2]);
		for(lParser.mPathPRJLen=0; lParser.mPathPRJ[ lParser.mPathPRJLen ]; lParser.mPathPRJLen++ );
		if( lParser.mPathPRJLen && '\\' != lParser.mPathPRJ[ lParser.mPathPRJLen-1])
			lParser.mPathPRJ[ lParser.mPathPRJLen++ ]= '\\';
		ProcessPBT( &lParser, argv[1]);
		Drive_SetPath( &lParser.mPathPRJ[0]);
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