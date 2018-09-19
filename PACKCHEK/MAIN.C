/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\ASSERT\ASSERT.H>
#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\DRIVE\DRIVE.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PACKER\PACKER.H>
#include	<GODLIB\PICTYPES\DEGAS.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>


/* ###################################################################################
#  DATA
################################################################################### */


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

sGemDosDTA	gPackCheckDTA;


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( S16 argc, char * argv[] )
{
	char			lFileSpec[ 512 ];
	char			lString[ 128 ];
	char *			lpDirectory;
	U8				lPackedFlag;
	S32				lSize;
	S32				lUnpackedSize;
	sFileHandle		lLogHandle;
	sFileHandle		lHandle;
	sPackerHeader	lHeader;

	printf( "< PACKCHEK >\n\n" );

	if( argc < 2 )
	{
		printf( "usage : PACKCHEK <directory>\n" );
		GemDos_Cnecin();
		return( 0 );
	}

	lpDirectory = argv[ 1 ];

	printf( "Directory : %s\n", lpDirectory );

	Drive_SetPath( lpDirectory );

	strcpy( &lFileSpec[0], lpDirectory );
	if( lFileSpec[ strlen(lFileSpec)-1 ] != '\\' )
	{
		lFileSpec[ strlen(lFileSpec)   ]  = '\\';
	}
	strcat( lFileSpec, "*.*" );

	lLogHandle = File_Create( "PACKCHEK.LOG" );

	File_SetDTA( &gPackCheckDTA );
	if(	File_ReadFirst( lFileSpec, 1 ) )
	{
		printf( "no files found\n" );
		GemDos_Cnecin();
		return( 0 );
	}

	while( !File_ReadNext() )
	{
		lHandle = File_Open( gPackCheckDTA.mFileName );
		if( lHandle > 0 )
		{
			File_Read( lHandle, sizeof(sPackerHeader), &lHeader );
			File_Close( lHandle );
			lSize = File_GetSize( gPackCheckDTA.mFileName );
			if( Packer_IsPacked( &lHeader ) )
			{
				lPackedFlag   = 1;
				lUnpackedSize = Packer_GetDepackSize( &lHeader );
			}
			else
			{
				lPackedFlag   = 0;
				lUnpackedSize = lSize;
			}
			sprintf( lString, "%s\t%d\t%ld\t%ld\n", gPackCheckDTA.mFileName, lPackedFlag, lSize, lUnpackedSize );
			File_Write( lLogHandle, strlen(lString), lString );
		}
	}

	File_Close( lLogHandle );

	return( 1 );
}


