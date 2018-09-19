/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PICTYPES\DEGAS.H>


/* ###################################################################################
#  DATA
################################################################################### */

sDegasElite *	gpSrcGfx;
sDegasElite *	gpDstGfx;

/* ###################################################################################
#  PROTOTYPES
################################################################################### */


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( S16 argc, char * argv[] )
{
	U16 *	lpSrc;
	U16	*	lpDst;
	char *	lpSrcFileName;
	char *	lpDstFileName;
	U16		x,y;
	U16		lMask;

	printf( "< MASKMAKE >\n\n" );


	if( argc < 3 )
	{
		printf( "usage : MASKMAKE <src gfx.pi1> <dst msk.pi1>\n" );
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

	gpSrcGfx = File_Load( lpSrcFileName );
	if( !gpSrcGfx )
	{
		printf( "error : couldn't load/allocate memory for %s", lpSrcFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	gpDstGfx = (sDegasElite*) mMEMCALLOC( sizeof(sDegasElite) );
	if( !gpDstGfx )
	{
		printf( "error : failed to allocate memory for mask file\n" );
		GemDos_Cnecin();
		return( 0 );
	}

	gpDstGfx->mHeader = gpSrcGfx->mHeader;

	lpSrc = &gpSrcGfx->mPixels[ 0 ];
	lpDst = &gpDstGfx->mPixels[ 0 ];

	for( y=0; y<200; y++ )
	{
		for( x=0; x< 320/16; x++ )
		{
			lMask  = *lpSrc++;
			lMask |= *lpSrc++;
			lMask |= *lpSrc++;
			lMask |= *lpSrc++;
			lMask  = ~lMask;

			*lpDst++ = lMask;
			*lpDst++ = lMask;
			*lpDst++ = lMask;
			*lpDst++ = lMask;
		}
	}

	printf( "Saving : %s\n", lpDstFileName );

	if( !File_Save( lpDstFileName, gpDstGfx, sizeof(sDegasElite) ) )
	{
		printf( "error : couldn't save file %s\n", lpDstFileName );
		GemDos_Cnecin();
		return( 0 );
	}

	mMEMFREE( gpDstGfx );
	File_UnLoad( gpSrcGfx );

	return( 1 );
}