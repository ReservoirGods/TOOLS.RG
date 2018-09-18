/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PACKER\GODPACK.H>
#include	<GODLIB\PACKER\PACKER.H>


/* ###################################################################################
#  INCLUDES
################################################################################### */

int main( int argc, char ** argv )
{
	int					lRes = 0;
	U8 *				lpSrcData;
	U32					lDstSize;
	U32					lSrcSize;
	sGodPackHeader *	lpHeader;
	U8 *				lpTemp;
	char *				lpFileNameSrc;
	char *				lpFileNameDst;

	printf( "GOD-PACKER\n" );
	printf( "(c) 2018 Reservoir Gods\n" );

	if( argc < 3 )
	{
		printf( "\nUSAGE: GODPACK srcfile dstfile" );
		return( -1 );
	}

	lpFileNameSrc = argv[ 1 ];
	lpFileNameDst = argv[ 2 ];

	if( !File_Exists( lpFileNameSrc ) )
	{
		printf( "GODPACK : ERROR : can't find file %s\n", lpFileNameSrc );
		return( -2 );
	}
	printf( "<GODPACK> %s -> %s\n", lpFileNameSrc, lpFileNameDst );

	lSrcSize = File_GetSize( argv[ 1 ] );
	if( !lSrcSize )
	{
		printf( "GODPACK : ERROR : can't pack 0 size file %s\n", lpFileNameSrc );
	}
	{
		lpSrcData = File_Load( argv[ 1 ] );

		lpHeader = GodPack_Pack( lpSrcData, lSrcSize );

		if( !lpHeader )
		{
			printf( "couldn't allocate memory for packed file\n" );
			lRes = -3;
		}
		else
		{
			lpTemp = mMEMCALLOC( lSrcSize + dGODPACK_OVERFLOW );
			GodPack_DePack( lpHeader, lpTemp );
			if( !Memory_IsEqual( lpSrcData, lpTemp, lSrcSize ) )
			{
				printf( "VALIDATE ERROR! Depacked data doesn't match original\n" );
				lRes = -4;
			}
			else
			{
				U32	lPerc;
				lDstSize = Packer_GetDepackSize( (sPackerHeader*)lpHeader );
				Endian_ReadBigU32( &lpHeader->mPackedSize, lDstSize );

				File_Save( lpFileNameDst, lpHeader, lDstSize + sizeof( sGodPackHeader ) );

				lPerc  = lDstSize * 100;
				lPerc /= lSrcSize;

				printf( "%d bytes -> %d bytes (%d%%)\n", lSrcSize, lDstSize, lPerc );
			}

			mMEMFREE( lpHeader );
		}

		File_UnLoad( lpSrcData );
	}

	return( lRes );
}