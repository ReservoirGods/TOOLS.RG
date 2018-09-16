/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\LINKFILE\LINKFILE.H>


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( S16 argc, char * argv[] )
{
	printf( "< MAKELINK >\n\n" );

	if( argc < 3 )
	{
		printf( "usage : MAKELINK <directory> <linkfilename>\n" );
		GemDos_Cnecin();
		return( 0 );
	}

	printf( "%s -> %s\n", argv[1], argv[2] );
	LinkFile_BuildFromDirectory( argv[ 1 ], argv[ 2 ] );


	return( 1 );
}


