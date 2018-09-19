/* ###################################################################################
#  INCLUDES
################################################################################### */


#include	<GODLIB\ASSERT\ASSERT.H>
#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PROGRAM\PROGRAM.H>
#include	<GODLIB\PROFILER\PROFILER.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>


/* ###################################################################################
#  DATA
################################################################################### */


/* ###################################################################################
#  PROTOTYPES
################################################################################### */


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( S16 argc, char * argv[] )
{
	char					lString[ 128 ];
	char					lName[ 10 ];
	char *					lpSrcProFileName;
	char *					lpSrcPrgFileName;
	char *					lpDstTxtFileName;
	sProgramHeader *		lpPrg;
	sProfilerHeader *		lpPro;
	sProfilerSymbolTable *	lpTable;
	sFileHandle				lHandle;
	FP32					lPerc;
	U32						i,j;

	printf( ":::::::::::::::::::::::::::::\n"   );
	printf( ":: PROST       ST Profiler ::\n"   );
	printf( ":: [c] 2001 Reservoir Gods ::\n"   );
	printf( ":::::::::::::::::::::::::::::\n\n" );

	if( argc < 4 )
	{
		printf( "usage : PROST <src program.prg> <src profile.pro> <dst profile.txt>\n" );
		GemDos_Cnecin();

		lpSrcPrgFileName = "C:\\SOURCE\\RG\\CHUCHU\\CHUCHU.TOS";
		lpSrcProFileName = "C:\\SOURCE\\RG\\CHUCHU\\PROFILE.PRO";
		lpDstTxtFileName = "C:\\SOURCE\\RG\\CHUCHU\\PROFILE.TXT";
	}
	else
	{
		lpSrcPrgFileName = argv[ 1 ];
		lpSrcProFileName = argv[ 2 ];
		lpDstTxtFileName = argv[ 3 ];
	}

	printf( "Loading Program : %s\n", lpSrcPrgFileName );

	if( !File_Exists( lpSrcPrgFileName ) )
	{
		printf( "error: couldn't find program file %s\n", lpSrcPrgFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	lpPrg = (sProgramHeader*)File_Load( lpSrcPrgFileName );
	if( !lpPrg )
	{
		printf( "error : couldn't load/allocate memory for %s", lpSrcPrgFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	if( !Program_IsValid( lpPrg ) )
	{
		printf( "error : invalid program file format %s", lpSrcPrgFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	if( !File_Exists( lpSrcProFileName ) )
	{
		printf( "error: couldn't find profile file %s\n", lpSrcProFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	lpPro = Profiler_LoadProfile( lpSrcProFileName );
	if( !lpPro )
	{
		printf( "error : couldn't load/allocate memory for %s", lpSrcProFileName );
		GemDos_Cnecin();
		return( 0 );
	}
	if( !Profiler_IsValid( lpPro ) )
	{
		printf( "error : invalid profiler file format %s", lpSrcProFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	lpTable = Profiler_BuildSymbolTable( lpPrg, lpPro );

	printf( "Saving : %s\n", lpDstTxtFileName );
	lHandle = File_Create( lpDstTxtFileName );

	sprintf( lString, "Build %d.%d %s %s\n", lpPro->mGameBuildHi, lpPro->mGameBuildLo, &lpPro->mDateText[0], &lpPro->mTimeText[0] );
	File_Write( lHandle, strlen(lString), &lString[ 0 ] );
	
	sprintf( lString, "Total Hits: %ld\n", lpTable->mHeader.mHitCount );
	File_Write( lHandle, strlen(lString), &lString[ 0 ] );

	for( i=0; i<lpTable->mHeader.mCount; i++ )
	{
		for( j=0; j<8; j++ )
		{
			lName[ j ] = lpTable->mSymbol[ i ].mSymbol.mName[ j ];
		}
		lName[ 8 ] = 0;
		if( lpTable->mSymbol[ i ].mCount )
		{
			lPerc  = (F32)lpTable->mSymbol[ i ].mCount;
			lPerc /= lpTable->mHeader.mHitCount;
			lPerc *= 100.f;
			sprintf( lString, "%s\t%ld\t%3.4f\t%lX\n", lName, lpTable->mSymbol[ i ].mCount, lPerc, lpTable->mSymbol[ i ].mSymbol.mValue );
			File_Write( lHandle, strlen(lString), &lString[ 0 ] );
		}
	}

	File_Close( lHandle );

	return( 0 );
}