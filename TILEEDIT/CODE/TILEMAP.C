/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"TILEMAP.H"

#include	<GODLIB\MEMORY\MEMORY.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

#define	dTM_IMG_BLOCKSX		20
#define	dTM_IMG_BLOCKSY		12
#define	dTM_TILES_PER_IMAGE	(dTM_IMG_BLOCKSX*dTM_IMG_BLOCKSY)

/* ###################################################################################
#  PROTOTYPES
################################################################################### */


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_Init( sTileMap * apMap,const U16 aBlocksX,const U16 aBlocksY )
* ACTION   : TileMap_Init
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_Init( sTileMap * apMap,const U16 aBlocksX,const U16 aBlocksY )
{
	U32	lSize;

	if( apMap )
	{
		lSize  = aBlocksX;
		lSize *= aBlocksY;
		lSize *= 2;

		apMap->mID      = dTILEMAP_ID;
		apMap->mVersion = dTILEMAP_VERSION;
		apMap->mBlocksX = aBlocksX;
		apMap->mBlocksY = aBlocksY;
		apMap->mpBlocks = mMEMCALLOC( lSize );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_DeInit( sTileMap * apMap )
* ACTION   : TileMap_DeInit
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_DeInit( sTileMap * apMap )
{
	if( apMap )
	{
		if( apMap->mpBlocks )
		{
			mMEMFREE( apMap->mpBlocks );
			apMap->mpBlocks = 0;
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_GetTile( const sTileMap * apMap,const U16 aX,const aY )
* ACTION   : TileMap_GetTile
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

U16	TileMap_GetTile( const sTileMap * apMap,const U16 aX,const U16 aY )
{
	U32	lOffset;
	U16	lTile;
	
	lTile = 0;
	if( apMap )
	{
		if( (aX<apMap->mBlocksX) && (aY<apMap->mBlocksY) )
		{
			lOffset  = aY;
			lOffset *= apMap->mBlocksX;
			lOffset += aX;
			lTile    = apMap->mpBlocks[ lOffset ];
		}
	}

	return( lTile );
}



/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_GetSize( const sTileMap * apMap )
* ACTION   : TileMap_GetSize
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	TileMap_GetSize( const sTileMap * apMap )
{
	U32	lSize;

	if( apMap )
	{
		lSize  = apMap->mBlocksX;
		lSize *= apMap->mBlocksY;
		lSize *= 2;
		lSize += sizeof(sTileMap);
	}
	else
	{
		lSize = 0;
	}

	return( lSize );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_Insert( sTileMap * apMap,const U16 aCol,const U16 aRow,const U16 aBlockIndex )
* ACTION   : TileMap_Insert
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_Insert( sTileMap * apMap,const U16 aCol,const U16 aRow,const U16 aBlockIndex )
{
	U32	lOffset;

	if( apMap )
	{
		if( (aCol < apMap->mBlocksX) && (aRow < apMap->mBlocksY) )
		{
			lOffset  = aRow;
			lOffset *= apMap->mBlocksX;
			lOffset += aCol;

			if( apMap->mpBlocks )
			{
				apMap->mpBlocks[ lOffset ] = aBlockIndex;
			}
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_RowInsert( sTileMap * apMap,const U16 aRowIndex )
* ACTION   : TileMap_RowInsert
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_RowInsert( sTileMap * apMap,const U16 aRowIndex )
{
	U32		lSize;
	U16 *	lpSrc;
	U16 *	lpDst;
	U16 *	lpBlocks;
	U16		i,j;;

	if( apMap )
	{
		apMap->mBlocksY++;

		lSize  = apMap->mBlocksX;
		lSize *= apMap->mBlocksY;
		lSize *= 2;

		lpBlocks = mMEMCALLOC( lSize );

		if( apMap->mpBlocks )
		{
			lpSrc = apMap->mpBlocks;
			lpDst = lpBlocks;

			for( i=0; i<aRowIndex; i++ )
			{
				for( j=0; j<apMap->mBlocksX; j++ )
				{
					*lpDst++ = *lpSrc++;
				}
			}
			for( j=0; j<apMap->mBlocksX; j++ )
			{
				*lpDst++ = 0;
			}
			for( i=aRowIndex; i<apMap->mBlocksY; i++ )
			{
				for( j=0; j<apMap->mBlocksX; j++ )
				{
					*lpDst++ = *lpSrc++;
				}
			}
			mMEMFREE( apMap->mpBlocks );
		}
		apMap->mpBlocks = lpBlocks;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_RowDelete( sTileMap * apMap,const U16 aRowIndex )
* ACTION   : TileMap_RowDelete
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_RowDelete( sTileMap * apMap,const U16 aRowIndex )
{
	U32		lSize;
	U16 *	lpSrc;
	U16 *	lpDst;
	U16 *	lpBlocks;
	U16		i,j;;

	if( apMap )
	{
		if( apMap->mBlocksY > 1 )
		{
			apMap->mBlocksY--;

			lSize  = apMap->mBlocksX;
			lSize *= apMap->mBlocksY;
			lSize *= 2;

			lpBlocks = mMEMCALLOC( lSize );

			if( apMap->mpBlocks )
			{
				lpSrc = apMap->mpBlocks;
				lpDst = lpBlocks;

				for( i=0; i<aRowIndex; i++ )
				{
					for( j=0; j<apMap->mBlocksX; j++ )
					{
						*lpDst++ = *lpSrc++;
					}
				}
				lpSrc += apMap->mBlocksX;
				for( i=(aRowIndex+1); i<apMap->mBlocksY; i++ )
				{
					for( j=0; j<apMap->mBlocksX; j++ )
					{
						*lpDst++ = *lpSrc++;
					}
				}
				mMEMFREE( apMap->mpBlocks );
			}
			apMap->mpBlocks = lpBlocks;
		}
	}	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_ColInsert( sTileMap * apMap,const U16 aColIndex )
* ACTION   : TileMap_ColInsert
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_ColInsert( sTileMap * apMap,const U16 aColIndex )
{
	U32		lSize;
	U16 *	lpSrc;
	U16 *	lpDst;
	U16 *	lpBlocks;
	U16		i,j;;

	if( apMap )
	{
		apMap->mBlocksX++;

		lSize  = apMap->mBlocksX;
		lSize *= apMap->mBlocksY;
		lSize *= 2;

		lpBlocks = mMEMCALLOC( lSize );

		if( apMap->mpBlocks )
		{
			lpSrc = apMap->mpBlocks;
			lpDst = lpBlocks;
			for( i=0; i<apMap->mBlocksY; i++ )
			{
				for( j=0; j<aColIndex; j++ )
				{
					*lpDst++ = *lpSrc++;
				}
				*lpDst++ = 0;
				for( j=(aColIndex+1); j<apMap->mBlocksX; j++ )
				{
					*lpDst++ = *lpSrc++;
				}

			}
			mMEMFREE( apMap->mpBlocks );
		}

		apMap->mpBlocks = lpBlocks;
	}	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_ColDelete( sTileMap * apMap,const U16 aColIndex )
* ACTION   : TileMap_ColDelete
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_ColDelete( sTileMap * apMap,const U16 aColIndex )
{
	U32		lSize;
	U16 *	lpSrc;
	U16 *	lpDst;
	U16 *	lpBlocks;
	U16		i,j;;

	if( apMap )
	{
		if( apMap->mBlocksX > 1 )
		{
			apMap->mBlocksX--;

			lSize  = apMap->mBlocksX;
			lSize *= apMap->mBlocksY;
			lSize *= 2;

			lpBlocks = mMEMCALLOC( lSize );

			if( apMap->mpBlocks )
			{
				lpSrc = apMap->mpBlocks;
				lpDst = lpBlocks;
				for( i=0; i<apMap->mBlocksY; i++ )
				{
					for( j=0; j<aColIndex; j++ )
					{
						*lpDst++ = *lpSrc++;
					}
					lpSrc++;
					for( j=(aColIndex+1); j<=apMap->mBlocksX; j++ )
					{
						*lpDst++ = *lpSrc++;
					}

				}
				mMEMFREE( apMap->mpBlocks );
			}

			apMap->mpBlocks = lpBlocks;
		}
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_Validate( sTileMap * apMap )
* ACTION   : TileMap_Validate
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

U8	TileMap_Validate( sTileMap * apMap )
{
	U8	lRes;

	lRes = 0;
	if( apMap )
	{
		if( (apMap->mID == dTILEMAP_ID) &&
			(apMap->mVersion == dTILEMAP_VERSION) )
		{
			lRes = 1;
		}
	}
	
	return( lRes );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_Relocate( sTileMap * apMap )
* ACTION   : TileMap_Relocate
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_Relocate( sTileMap * apMap )
{
	if( TileMap_Validate(apMap) )
	{
		*(U32*)&apMap->mpBlocks += (U32)apMap;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_Delocate( sTileMap * apMap )
* ACTION   : TileMap_Delocate
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileMap_Delocate( sTileMap * apMap )
{
	if( TileMap_Validate(apMap) )
	{
		*(U32*)&apMap->mpBlocks -= (U32)apMap;
	}		
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : sTileMap * TileMap_Serialise( const sTileMap * apMap)
* ACTION   : TileMap_Serialise
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

sTileMap * TileMap_Serialise( const sTileMap * apMap )
{
	sTileMap *	lpMap;
	U32			lSize;
	U8 *		lpMem;

	if( apMap )
	{
		lSize = TileMap_GetSize( apMap );
		lpMap = mMEMCALLOC( lSize );
		if( lpMap )
		{
			*lpMap = *apMap;
			lSize -= sizeof(sTileMap);
			if( lSize )
			{
				lpMem  = (U8*)lpMap;
				lpMem += sizeof(sTileMap);
				lpMap->mpBlocks = (U16*)lpMem;
				Memory_Copy( lSize, apMap->mpBlocks, lpMap->mpBlocks );
			}
		}
	}
	else
	{
		lpMap = 0;
	}

	return( lpMap );
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_GetX( const U16 aTileIndex )
* ACTION   : TileMap_GetX
* CREATION : 04.03.2004 PNK
*-----------------------------------------------------------------------------------*/

U16	TileMap_GetX( const U16 aTileIndex )
{
	U16	lX,lB;
	
	lB  = aTileIndex / dTM_TILES_PER_IMAGE;
	lB  = aTileIndex - ( lB * dTM_TILES_PER_IMAGE );

	lX  = lB / dTM_IMG_BLOCKSX;
	lX  = lB - (lX * dTM_IMG_BLOCKSX);
	lX *= 16;

	return( lX );
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_GetY( const U16 aTileIndex )
* ACTION   : TileMap_GetY
* CREATION : 04.03.2004 PNK
*-----------------------------------------------------------------------------------*/

U16	TileMap_GetY( const U16 aTileIndex )
{
	U16	lY;
	
	lY  = aTileIndex / dTM_TILES_PER_IMAGE;
	lY  = aTileIndex - ( lY * dTM_TILES_PER_IMAGE );

	lY  = lY / dTM_IMG_BLOCKSX;
	lY *= 16;
	return( lY );
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileMap_GetImageIndex( const U16 aTileIndex )
* ACTION   : TileMap_GetImageIndex
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

U16	TileMap_GetImageIndex( const U16 aTileIndex )
{
	U16	lIndex;
	
	lIndex = aTileIndex / dTM_TILES_PER_IMAGE;

	return( lIndex );
}



/* ################################################################################ */
