/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"R_CUSTOM.H"

#include	"IMG_MENU.H"
#include	"EDITMENU.H"
#include	"TILEMAP.H"

#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\GRAPHIC\GRAPHIC.H>
#include	<GODLIB\GUI\GUI.H>
#include	<GODLIB\GUI\GUIDATA.H>
#include	<GODLIB\GUI\GUIEDIT.H>
#include	<GODLIB\GUI\R_GUI.H>
#include	<GODLIB\MEMORY\MEMORY.H>


/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	eRCUST_TYPE_NONE  = 0,
	eRCUST_TYPE_DEGAS = 1,
	eRCUST_TYPE_MAP   = 2,

	eRCUST_TYPE_LIMIT
};

#define	dRCUST_TILERP_LIMIT	8


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sRCustDegasTile
{
	U16	mTileIndex;
	U16	mRedrawFlag;
} sRCustDegasTile;

typedef	struct	sRCustMapTile
{
	S16	mMouseX;
	S16	mMouseY;
	U16	mRedrawFlag;
} sRCustMapTile;

typedef	struct	sRCustMapRePaintTile
{
	U16	mTileX;
	U16	mTileY;
	U16	mRePaintFlag;
} sRCustMapRePaintTile;

typedef	struct	sRenderCustomClass
{
	sGraphicCanvas			mCanvas;
	sRCustMapRePaintTile	mRPTile[ dRCUST_TILERP_LIMIT ];
	sRCustDegasTile			mDegasTiles[ 2 ];
	sRCustMapTile			mMapTiles[ 2 ];
	sRCustMapTile			mMapDrawTile;
	U16						mDegasTile;
	U16						mDegasTileRedrawFlag;
	U16						mTileIndex;
	U16						mColour;
	U16						mPaintEnable;
} sRenderCustomClass;


/* ###################################################################################
#  DATA
################################################################################### */

sRenderCustomClass	gRCustomClass;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	RenderCustom_ButtonDraw( sGraphicCanvas * apLogic, sGuiButton * apButton, const U16 aRedrawFlag );

void	RenderCustom_DegasDraw( sGraphicCanvas * apLogic, sGuiButton * apButton, const U16 aRedrawFlag );
void	RenderCustom_DegasTileDraw( sGraphicCanvas * apLogic, sGuiButton * apButton, const U16 aTileIndex );
void	RenderCustom_DegasTileUnDraw( sGraphicCanvas * apLogic, sGuiButton * apButton, const U16 aTileIndex );

void	RenderCustom_TileDraw( sGraphicCanvas * apLogic, const sTileMap * apMap, sGraphicPos * apPos, const U16 aTile );
void	RenderCustom_TileRepaint( sGraphicCanvas * apLogic, sGuiButton * apButton, sRCustMapRePaintTile * apRP );

void	RenderCustom_MapDraw( sGraphicCanvas * apLogic, sGuiButton * apButton, const U16 aRedrawFlag );
void	RenderCustom_MapTileDraw( sGraphicCanvas * apLogic, sGuiButton * apButton );
void	RenderCustom_MapTileUnDraw( sGraphicCanvas * apLogic, sGuiButton * apButton );

void	RenderCustom_Focus( sGuiButton * apButton, const sGuiMouse * apMouse );
void	RenderCustom_DeFocus( sGuiButton * apButton );

void	RenderCustom_DegasFocus( sGuiButton * apButton, const sGuiMouse * apMouse );
void	RenderCustom_DegasDeFocus( sGuiButton * apButton );
void	RenderCustom_MapFocus( sGuiButton * apButton, const sGuiMouse * apMouse );
void	RenderCustom_MapDeFocus( sGuiButton * apButton );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : void RenderCustom_Init(void)
* ACTION   : RenderCustom_Init
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void RenderCustom_Init(void)
{
	Memory_Clear( sizeof(gRCustomClass), &gRCustomClass );

	GraphicCanvas_Init( &gRCustomClass.mCanvas, eGRAPHIC_COLOURMODE_4PLANE, 320, 200 );
	RenderGui_SetCustomRender( RenderCustom_ButtonDraw );

	Gui_SetCustomFocus( RenderCustom_Focus );
	Gui_SetCustomDeFocus( RenderCustom_DeFocus );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void RenderCustom_DeInit(void)
* ACTION   : RenderCustom_DeInit
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void RenderCustom_DeInit(void)
{
	RenderGui_SetCustomRender( 0 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void RenderCustom_ButtonDraw(sGraphicCanvas * apLogic,sGuiButton * apButton, const U16 aRedrawFlag )
* ACTION   : RenderCustom_ButtonDraw
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void RenderCustom_ButtonDraw(sGraphicCanvas * apLogic,sGuiButton * apButton, const U16 aRedrawFlag )
{
	DebugLog_Printf0( "RenderCustom_ButtonDraw()" );

	switch( apButton->mSubType )
	{
	case	eRCUST_TYPE_NONE:
		break;
	case	eRCUST_TYPE_DEGAS:
		RenderCustom_DegasDraw( apLogic, apButton, aRedrawFlag );
		break;
	case	eRCUST_TYPE_MAP:
		RenderCustom_MapDraw( apLogic, apButton, aRedrawFlag );
		break;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void RenderCustom_DegasDraw(sGraphicCanvas * apLogic,sGuiButton * apButton, const U16 aRedrawFlag )
* ACTION   : RenderCustom_DegasDraw
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void RenderCustom_DegasDraw(sGraphicCanvas * apLogic,sGuiButton * apButton, const U16 aRedrawFlag )
{
	sDegas *		lpDegas;
	sGraphicPos		lPos;
	sGraphicRect	lRect;

	DebugLog_Printf0( "RenderCustom_DegasDraw()" );

	RenderCustom_DegasTileUnDraw( apLogic, apButton, 0 );

	if( aRedrawFlag )
	{
		lpDegas = ImageMenu_GetpImageCurrent();

		if( lpDegas )
		{
			gRCustomClass.mCanvas.mpVRAM = &lpDegas->mPixels[ 0 ];
			lPos.mX = apButton->mInfo.mRectPair.mWorld.mX;
			lPos.mY = apButton->mInfo.mRectPair.mWorld.mY;

			lRect.mX = 0;
			lRect.mY = 0;
			lRect.mWidth = 320;
			lRect.mHeight = 200;

			apLogic->mpClipFuncs->Blit( apLogic, &lPos, &lRect, &gRCustomClass.mCanvas );
		}
		else
		{
			lRect.mX      = apButton->mInfo.mRectPair.mWorld.mX;
			lRect.mY      = apButton->mInfo.mRectPair.mWorld.mY;
			lRect.mWidth  = apButton->mInfo.mRectPair.mWorld.mWidth;
			lRect.mHeight = apButton->mInfo.mRectPair.mWorld.mHeight;

			apLogic->mpClipFuncs->DrawBox( apLogic, &lRect, 1 );
		}
	}
	if( gRCustomClass.mDegasTileRedrawFlag )
	{
		RenderCustom_DegasTileDraw( apLogic, apButton, gRCustomClass.mDegasTile );
		gRCustomClass.mDegasTileRedrawFlag = 0;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_Focus( sGuiButton * apButton,const sGuiMouse * apMouse )
* ACTION   : RenderCustom_Focus
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_Focus( sGuiButton * apButton,const sGuiMouse * apMouse )
{
	DebugLog_Printf0( "RenderCustom_Focus()" );

	switch( apButton->mSubType )
	{
	case	eRCUST_TYPE_NONE:
		break;
	case	eRCUST_TYPE_DEGAS:
		RenderCustom_DegasFocus( apButton, apMouse );
		break;
	case	eRCUST_TYPE_MAP:
		RenderCustom_MapFocus( apButton, apMouse );
		break;
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_DeFocus( sGuiButton * apButton )
* ACTION   : RenderCustom_DeFocus
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_DeFocus( sGuiButton * apButton )
{
	DebugLog_Printf0( "RenderCustom_DeFocus()" );

	switch( apButton->mSubType )
	{
	case	eRCUST_TYPE_NONE:
		break;
	case	eRCUST_TYPE_DEGAS:
		RenderCustom_DegasDeFocus( apButton );
		break;
	case	eRCUST_TYPE_MAP:
		RenderCustom_MapDeFocus( apButton );
		break;
	}	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_DegasFocus( sGuiButton * apButton,const sGuiMouse * apMouse )
* ACTION   : RenderCustom_DegasFocus
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_DegasFocus( sGuiButton * apButton,const sGuiMouse * apMouse )
{
	(void)apButton;
	gRCustomClass.mDegasTile  = ( (apMouse->mY>>4) * 20 );
	gRCustomClass.mDegasTile += ( apMouse->mX >> 4);
	gRCustomClass.mDegasTileRedrawFlag = 1;

	gRCustomClass.mTileIndex = gRCustomClass.mDegasTile;
	if( gRCustomClass.mTileIndex > 240 )
	{
		gRCustomClass.mTileIndex = 239;
	}
	gRCustomClass.mTileIndex += ImageMenu_GetImageIndex() * 240;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_DegasDeFocus( sGuiButton * apButton )
* ACTION   : RenderCustom_DegasDeFocus
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_DegasDeFocus( sGuiButton * apButton )
{
	(void)apButton;
	gRCustomClass.mDegasTileRedrawFlag = 0;	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_DegasTileDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const U16 aTileIndex )
* ACTION   : RenderCustom_DegasTileDraw
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_DegasTileDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const U16 aTileIndex )
{
	sGraphicRect	lRect;
	U16				lColour;
	U16				lIndex;
	S16				lX,lY;

	lColour = gRCustomClass.mColour;
	lColour++;
	lColour &= 15;
	gRCustomClass.mColour = lColour;

	lIndex  = RenderGui_GetScreenIndex();
	gRCustomClass.mDegasTiles[ lIndex ].mRedrawFlag = 1;
	gRCustomClass.mDegasTiles[ lIndex ].mTileIndex  = aTileIndex;

	lY = aTileIndex / 20;
	lX = aTileIndex - (lY*20);

	lX *= 16;
	lY *= 16;

	lX += apButton->mInfo.mRectPair.mWorld.mX;
	lY += apButton->mInfo.mRectPair.mWorld.mY;

	DebugLog_Printf3( "RenderCustom_DegasTileDraw() x:%d y:%d t:%d", lX, lY, aTileIndex );

	lRect.mX      = lX;
	lRect.mY      = lY;
	lRect.mWidth  = 16;
	lRect.mHeight = 1;
	apLogic->mpClipFuncs->DrawBox( apLogic, &lRect, lColour );

	lRect.mX      = lX;
	lRect.mY      = lY;
	lRect.mWidth  = 1;
	lRect.mHeight = 16;
	apLogic->mpClipFuncs->DrawBox( apLogic, &lRect, lColour );

	lRect.mX      = lX+15;
	lRect.mY      = lY;
	lRect.mWidth  = 1;
	lRect.mHeight = 16;
	apLogic->mpClipFuncs->DrawBox( apLogic, &lRect, lColour );

	lRect.mX      = lX;
	lRect.mY      = lY+15;
	lRect.mWidth  = 16;
	lRect.mHeight = 1;
	apLogic->mpClipFuncs->DrawBox( apLogic, &lRect, lColour );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_DegasTileUnDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const U16 aTileIndex )
* ACTION   : RenderCustom_DegasTileUnDraw
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_DegasTileUnDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const U16 aTileIndex )
{
	sDegas *		lpDegas;
	sGraphicPos		lPos;
	sGraphicRect	lRect;
	U16				lIndex;
	U16				lTileIndex;
	S16				lX,lY;

	(void)aTileIndex;
	lIndex = RenderGui_GetScreenIndex();
	if( gRCustomClass.mDegasTiles[ lIndex ].mRedrawFlag )
	{
		gRCustomClass.mDegasTiles[ lIndex ].mRedrawFlag--;

		lTileIndex = gRCustomClass.mDegasTiles[ lIndex ].mTileIndex;

		lY = lTileIndex / 20;
		lX = lTileIndex - (lY*20);

		lX *= 16;
		lY *= 16;

		lpDegas = ImageMenu_GetpImageCurrent();
		if( lpDegas )
		{


			lPos.mX = lX;
			lPos.mY = lY;

			lPos.mX += apButton->mInfo.mRectPair.mWorld.mX;
			lPos.mY += apButton->mInfo.mRectPair.mWorld.mY;


			lRect.mX = lX;
			lRect.mY = lY;
			lRect.mWidth = 16;
			lRect.mHeight = 16;

			gRCustomClass.mCanvas.mpVRAM = &lpDegas->mPixels[ 0 ];
			apLogic->mpClipFuncs->Blit( apLogic, &lPos, &lRect, &gRCustomClass.mCanvas );
		}
		else
		{
			lRect.mX = lX + apButton->mInfo.mRectPair.mWorld.mX;;
			lRect.mY = lY + apButton->mInfo.mRectPair.mWorld.mY;;
			lRect.mWidth = 16;
			lRect.mHeight = 16;
			apLogic->mpClipFuncs->DrawBox( apLogic, &lRect, 1 );
		}
	}
	
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_MapDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const U16 aRedrawFlag )
* ACTION   : RenderCustom_MapDraw
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_MapDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const U16 aRedrawFlag )
{
	sTileMap *		lpMap;
	U16				lTile;
	U16				i,j;
	U16				lMapX,lMapY;
	sGraphicPos		lPos;

	lpMap   = EditMenu_GetpTileMap();

	if( apLogic && apButton && lpMap )
	{
		RenderCustom_MapTileUnDraw( apLogic, apButton );

		for( i=0; i<dRCUST_TILERP_LIMIT; i++ )
		{
			RenderCustom_TileRepaint( apLogic, apButton, &gRCustomClass.mRPTile[ i ] );
		}

		if( aRedrawFlag )
		{
			lPos.mY = apButton->mInfo.mRectPair.mWorld.mY;

			lMapX = (U16)EditMenu_GetMapX();
			lMapY = (U16)EditMenu_GetMapY();
			for( i=0; i<lpMap->mBlocksY; i++ )
			{
				lPos.mX = apButton->mInfo.mRectPair.mWorld.mX;
				for( j=0; j<lpMap->mBlocksX; j++ )
				{
					lTile = TileMap_GetTile( lpMap, (U16)(j+lMapX), (U16)(i+lMapY) );
					RenderCustom_TileDraw( apLogic, lpMap, &lPos, lTile );
					lPos.mX += 16;
				}
				lPos.mY += 16;
			}
		}
		RenderCustom_MapTileDraw( apLogic, apButton );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_MapTileDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const S16 aTileX,const S16 aTileY )
* ACTION   : RenderCustom_MapTileDraw
* CREATION : 04.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_MapTileDraw( sGraphicCanvas * apLogic,sGuiButton * apButton )
{
	sGraphicPos	lPos;

	if( gRCustomClass.mMapDrawTile.mRedrawFlag )
	{
		gRCustomClass.mMapTiles[ RenderGui_GetScreenIndex() ] = gRCustomClass.mMapDrawTile;

		lPos.mX = (gRCustomClass.mMapDrawTile.mMouseX>>4)*16;
		lPos.mY = (gRCustomClass.mMapDrawTile.mMouseY>>4)*16;

		lPos.mX += apButton->mInfo.mRectPair.mWorld.mX;
		lPos.mY += apButton->mInfo.mRectPair.mWorld.mY;

		RenderCustom_TileDraw( apLogic, EditMenu_GetpTileMap(), &lPos, gRCustomClass.mTileIndex );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_MapTileUnDraw( sGraphicCanvas * apLogic,sGuiButton * apButton,const U16 aTileX,const U16 aTileY )
* ACTION   : RenderCustom_MapTileUnDraw
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_MapTileUnDraw( sGraphicCanvas * apLogic,sGuiButton * apButton )
{
	sRCustMapTile *	lpTile;
	U16				lBlockIndex;
	U16				lMapX,lMapY;
	sGraphicPos		lPos;
	
	lpTile = &gRCustomClass.mMapTiles[ RenderGui_GetScreenIndex() ];
	if( lpTile->mRedrawFlag )
	{
		lPos.mX = (lpTile->mMouseX>>4)*16;
		lPos.mY = (lpTile->mMouseY>>4)*16;

		lPos.mX += apButton->mInfo.mRectPair.mWorld.mX;
		lPos.mY += apButton->mInfo.mRectPair.mWorld.mY;

		lMapX = (U16)EditMenu_GetMapX();
		lMapY = (U16)EditMenu_GetMapY();
		lBlockIndex = TileMap_GetTile( EditMenu_GetpTileMap(), (U16)((lpTile->mMouseX>>4)+lMapX), (U16)((lpTile->mMouseY>>4)+lMapY) );

		RenderCustom_TileDraw( apLogic, EditMenu_GetpTileMap(), &lPos, lBlockIndex );
		lpTile->mRedrawFlag = 0;
	}
}



/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_MapFocus( sGuiButton * apButton,const sGuiMouse * apMouse )
* ACTION   : RenderCustom_MapFocus
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_MapFocus( sGuiButton * apButton,const sGuiMouse * apMouse )
{
	sTileMap *	lpMap;
	U16			i;
	S16			lX,lY;

	(void)apButton;
	gRCustomClass.mMapDrawTile.mMouseX = apMouse->mX;	
	gRCustomClass.mMapDrawTile.mMouseY = apMouse->mY;
	gRCustomClass.mMapDrawTile.mRedrawFlag = 1;

	switch( apMouse->mButtonLeft )
	{
	case	eGUI_MOUSEBUTTON_CLICKED:
		gRCustomClass.mPaintEnable = 1;
		break;
	case	eGUI_MOUSEBUTTON_HELD:
		if( gRCustomClass.mPaintEnable )
		{
			lpMap = EditMenu_GetpTileMap();
			lX    = (apMouse->mX >> 4);
			lY    = (apMouse->mY >> 4);
			lX   += (U16)EditMenu_GetMapX();
			lY   += (U16)EditMenu_GetMapY();
			TileMap_Insert( lpMap, lX, lY, gRCustomClass.mTileIndex );

			i = 0;
			while( (i<dRCUST_TILERP_LIMIT) && (gRCustomClass.mRPTile[ i ].mRePaintFlag) )
			{
				i++;
			}
			DebugLog_Printf3( "RenderCustom_MapFocus() set rp[%d] %d %d", i, lX, lY );
			if( i<dRCUST_TILERP_LIMIT )
			{
				gRCustomClass.mRPTile[ i ].mTileX = lX;
				gRCustomClass.mRPTile[ i ].mTileY = lY;
				gRCustomClass.mRPTile[ i ].mRePaintFlag = 2;
			}
		}
		break;
	case	eGUI_MOUSEBUTTON_NONE:
	case	eGUI_MOUSEBUTTON_RELEASED:
		gRCustomClass.mPaintEnable = 0;
		break;
	}
	switch( apMouse->mButtonRight )
	{
	case	eGUI_MOUSEBUTTON_CLICKED:
		lpMap = EditMenu_GetpTileMap();
		lX    = (apMouse->mX >> 4);
		lY    = (apMouse->mY >> 4);
		lX   += (U16)EditMenu_GetMapX();
		lY   += (U16)EditMenu_GetMapY();
		gRCustomClass.mTileIndex = TileMap_GetTile( lpMap, lX, lY );
		break;
	default:
		break;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_MapDeFocus( sGuiButton * apButton )
* ACTION   : RenderCustom_MapDeFocus
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_MapDeFocus( sGuiButton * apButton )
{
	(void)apButton;
	gRCustomClass.mMapDrawTile.mRedrawFlag = 0;	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderCustom_TileDraw( sGraphicCanvas * apLogic,const sTileMap * apMap,const sGraphicPos * apPos,const U16 aTile, )
* ACTION   : RenderCustom_TileDraw
* CREATION : 03.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderCustom_TileDraw( sGraphicCanvas * apLogic, const sTileMap * apMap, sGraphicPos * apPos, const U16 aTile )
{
	sDegas *		lpDegas;
	U16				lImgIndex;
	sGraphicRect	lRect;

	(void)apMap;
	lImgIndex = TileMap_GetImageIndex( aTile );
	lpDegas   = ImageMenu_GetpImage( lImgIndex );

	if( lpDegas )
	{
		lRect.mX = TileMap_GetX( aTile );
		lRect.mY = TileMap_GetY( aTile );
		lRect.mWidth = 16;
		lRect.mHeight = 16;
		gRCustomClass.mCanvas.mpVRAM = &lpDegas->mPixels[ 0 ];
		apLogic->mpClipFuncs->Blit( apLogic, apPos, &lRect, &gRCustomClass.mCanvas );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void RenderCustom_TileRepaint(sGraphicCanvas * apLogic,sGuiButton * apButton,sRCustMapRePaintTile * apRP)
* ACTION   : RenderCustom_TileRepaint
* CREATION : 05.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void RenderCustom_TileRepaint(sGraphicCanvas * apLogic,sGuiButton * apButton,sRCustMapRePaintTile * apRP)
{
	U16			lBlockIndex;
	sGraphicPos	lPos;

	if( apLogic && apButton && apRP )
	{
		if( apRP->mRePaintFlag )
		{
			DebugLog_Printf2( "RenderCustom_TileRepaint() %d %d", apRP->mTileX, apRP->mTileY );
			lPos.mX  = (S16)((apRP->mTileX - EditMenu_GetMapX())<<4);
			lPos.mY  = (S16)((apRP->mTileY - EditMenu_GetMapY())<<4);
			lPos.mX += apButton->mInfo.mRectPair.mWorld.mX;
			lPos.mY += apButton->mInfo.mRectPair.mWorld.mY;
			lBlockIndex = TileMap_GetTile( EditMenu_GetpTileMap(), apRP->mTileX, apRP->mTileY );

			RenderCustom_TileDraw( apLogic, EditMenu_GetpTileMap(), &lPos, lBlockIndex );
			apRP->mRePaintFlag--;
		}
	}
}




