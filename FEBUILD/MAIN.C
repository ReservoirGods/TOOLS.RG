/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\FE\FEDPARSE.H>
#include	<GODLIB\FILE\FILE.H>


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( S16 argc, char * argv[] )
{
	char *	lpSrcFileName;
	char *	lpDstFileName;
	char *	lpText;
	S32		lSize;

	printf( "< FEBUILD >\n" );

	if( argc < 3 )
	{
		printf( "usage : FEBUILD <srcfile> <dstfile>\n" );
		GemDos_Cnecin();
		return( 0 );
	}

	lpSrcFileName = argv[1];
	lpDstFileName = argv[2];

	printf( "%s -> %s\n", argv[1], argv[2] );

	if( File_Exists( lpSrcFileName ) )
	{
		lSize  = File_GetSize( lpSrcFileName );
		lpText = File_Load( lpSrcFileName );
		FedParse_Init();
		FedParse_Text( lpText, lSize );
		File_Save( lpDstFileName, FedParse_GetpFed(), FedParse_GetSize() );
		File_UnLoad( lpText );
/*		Fed_Relocate( FedParse_GetpFed() );
		FedParse_ShowFedInfo();*/
		FedParse_DeInit();
	}
	else
	{
		printf( "ERROR : File Not Found %s\n", lpSrcFileName );
	}

	return( 1 );
}


