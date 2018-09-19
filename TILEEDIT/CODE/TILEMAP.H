#ifndef	INCLUDED_TILEMAP_H
#define	INCLUDED_TILEMAP_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

#define	dTILEMAP_ID			mSTRING_TO_U32( 'T', 'M', 'A', 'P' )
#define	dTILEMAP_VERSION	0


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sTileMap
{
	U32		mID;
	U32		mVersion;
	U16		mBlocksX;
	U16		mBlocksY;
	U16 *	mpBlocks;
} sTileMap;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void		TileMap_Init( sTileMap * apMap, const U16 aBlocksX, const U16 aBlocksY );
void		TileMap_DeInit( sTileMap * apMap );

U16			TileMap_GetTile( const sTileMap * apMap, const U16 aX, const U16 aY );
U32			TileMap_GetSize( const sTileMap * apMap );
void		TileMap_Insert( sTileMap * apMap, const U16 aCol, const U16 aRow, const U16 aBlockIndex );
void		TileMap_RowInsert( sTileMap * apMap, const U16 aRowIndex );
void		TileMap_RowDelete( sTileMap * apMap, const U16 aRowIndex );
void		TileMap_ColInsert( sTileMap * apMap, const U16 aColIndex );
void		TileMap_ColDelete( sTileMap * apMap, const U16 aColIndex );

U16			TileMap_GetImageIndex( const U16 aTileIndex );
U16			TileMap_GetX( const U16 aTileIndex );
U16			TileMap_GetY( const U16 aTileIndex );

U8			TileMap_Validate( sTileMap * apMap );
void		TileMap_Relocate( sTileMap * apMap );
void		TileMap_Delocate( sTileMap * apMap );

sTileMap *	TileMap_Serialise( const sTileMap * apMap );


/* ################################################################################ */

#endif	/* INCLUDED_TILEMAP_H */
