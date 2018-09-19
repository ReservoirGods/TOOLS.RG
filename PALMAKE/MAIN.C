/* ###################################################################################
#  INCLUDES
################################################################################### */


#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\PICTYPES\DEGAS.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( S16 argc, char * argv[] )
{
	char *		lpSrcFileName;
	char *		lpDstFileName;
	sDegas *	lpDegas;


	printf( "< PALMAKE >\n\n" );


	if( argc < 3 )
	{
		printf( "usage : PALMAKE <src gfx.pi1> <dst pal.pal>\n" );
		GemDos_Cnecin();
		return( 0 );
	}

	lpSrcFileName = argv[ 1 ];
	lpDstFileName = argv[ 2 ];

	printf( "Loading : %s\n", lpSrcFileName );

	if( !File_Exists( lpSrcFileName ) )
	{
		printf( "error: couldn't find input file %s\n", lpSrcFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	lpDegas = File_Load( lpSrcFileName );
	if( !lpDegas )
	{
		printf( "error : couldn't load/allocate memory for %s", lpSrcFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	printf( "Saving : %s\n", lpDstFileName );

	if( !File_Save( lpDstFileName, &lpDegas->mHeader.mPalette[0], 32 ) )
	{
		printf( "error : couldn't save file %s\n", lpDstFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	return( 1 );
}