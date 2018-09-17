/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<STDLIB.H>
#include	<STDIO.H>
#include	<STRING.H>

/* ###################################################################################
#  DATA
################################################################################### */


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

int	Decibel_Build( char * apSrcFileName, char * apDstFileName );


/* ###################################################################################
#  CODE
################################################################################### */

int	main( int argc, char * argv[] )
{
	printf( "[ decibel ]\n\n" );

	if( argc < 3 )
	{
		printf( "usage : DECIBEL <input_file> <output_file>\n" );
		GemDos_Cnecin();
		return( 0 );
	}

	if( !Decibel_Build( argv[ 1 ], argv[ 2 ] ) )
	{
		GemDos_Cnecin();
	}

	return( 0 );
}


int	Decibel_Build( char * apSrcFileName, char * apDstFileName )
{
	FILE *	lpFile;
	unsigned char *	lpData;
	char	lString[ 1024 ];
	long	lStrLen;
	long	lSize;
	long	lIndex;

	lpFile = fopen( apSrcFileName, "rb" );

	if( !lpFile )
	{
		printf( "ERROR : File not found %s\n", apSrcFileName );
		return( 0 );
	}

	fseek( lpFile, 0, SEEK_END );
	lSize = ftell( lpFile );
	fclose( lpFile );

	lpData = malloc( lSize );

	if( !lpData )
	{
		printf( "ERROR : Memory alloc failed %ld bytes\n", lSize );
		return( 0 );
	}

	lpFile = fopen( apSrcFileName, "rb" );
	fread( lpData, lSize, 1, lpFile );
	fclose( lpFile );

	lpFile = fopen( apDstFileName, "w" );
	if( !lpFile )
	{
		printf( "ERROR : Couldn't create file %s\n", apDstFileName );
		return( 0 );
	}


	sprintf( lString, "char gData[%ld] =\n{\n", lSize );
	fputs( lString, lpFile );

	lIndex = 0;
	while( (lIndex+8) < lSize )
	{
		sprintf( lString, "0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", lpData[ lIndex+0 ], lpData[ lIndex+1 ], lpData[ lIndex+2 ], lpData[ lIndex+3 ], lpData[ lIndex+4 ], lpData[ lIndex+5 ], lpData[ lIndex+6 ], lpData[ lIndex+7 ] );
		fputs( lString, lpFile );
		lIndex += 8;
	}

	lString[ 0 ] = 0;
	for( ; lIndex<lSize; lIndex++ )
	{
		lStrLen = strlen( &lString[0] );
		sprintf( &lString[ lStrLen ], "0x%02X,", lpData[ lIndex ] );
	}

	lSize = strlen( lString );
	if( lSize )
	{
		lString[ lSize-1 ] = 0;
	}
	fputs( lString, lpFile );

	sprintf( lString, "\n};\n" );
	fputs( lString, lpFile );
	fclose( lpFile );

	free( lpData );

	return( 1 );
}
