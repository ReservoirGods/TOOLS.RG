/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"LAYER.H"

#include	"BUILD.H"

#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\SCREEN\SCREEN.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\VIDEO\VIDEO.H>


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

extern void	Layer_Draw_2BP_Col1( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_2BP_Col2( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_2BP_Col3( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );

extern void	Layer_Draw_4BP_Col1(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col2(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col3(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col4(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col5(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col6(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col7(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col8(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col9(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col10( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col11( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col12( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col13( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col14( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );
extern void	Layer_Draw_4BP_Col15( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : Layer_Init( sLayer * apLayer )
* ACTION   : Layer_Init
* CREATION : 04.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Layer_Init( sLayer * apLayer )
{
	apLayer->mActiveFlag = 0;
	apLayer->mpFrame     = 0;
	apLayer->mColour     = 1;
	apLayer->mFlipX      = 0;
	apLayer->mFlipY      = 0;
	apLayer->mScrollX.l  = 0;
	apLayer->mScrollY.l  = 0;
	apLayer->mTileX      = 0;
	apLayer->mTileY      = 0;
	apLayer->mX.l        = 0;
	apLayer->mY.l        = 0;	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Layer_Update( sLayer * apLayer )
* ACTION   : Layer_Update
* CREATION : 23.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Layer_Update( sLayer * apLayer )
{
	apLayer->mX.l += apLayer->mScrollX.l;
	apLayer->mY.l += apLayer->mScrollY.l;
}


#if	0

/*-----------------------------------------------------------------------------------*
* FUNCTION : Layer_Render( sLayer * apLayers,U16 * apScreen )
* ACTION   : Layer_Render
* CREATION : 23.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Layer_Render( sLayer * apLayer,U16 * apScreen, sFont * apFont )
{
	S16		lRow, lCol;
	S16		lColBase;
	S16		lRowAdd, lColAdd;
	U16		i,j,k;
	U16		lOff;
	U32		lTextOff;
	U8 *	lpScreenRow;
	U8 *	lpScreenCol;
	U8 *	lpScreenItem;
	U8 *	lpSrc;

	U32	lCycles;

	lCycles = System_GetCycleCounter();
	
	if( apLayer->mFlipY )
	{
		lRowAdd = -1;
		lRow    = apLayer->mpFrame->mHeight-1;
		lRow   -= apLayer->mY.w.w1;
	}
	else
	{
		lRow = apLayer->mY.w.w1;
		lRowAdd = 1;
	}
	if( apLayer->mFlipX )
	{
		lColAdd   = -1;
		lColBase  = apLayer->mpFrame->mWidth-1;
		lColBase -= apLayer->mX.w.w1;
	}
	else
	{
		lColAdd  = 1;
		lColBase = apLayer->mX.w.w1;
	}

	lpScreenRow = (U8*)apScreen;

/*	Build_SteemPrintfLine1( "Layer_Render() : X : %d" , apLayer->mX.w.w1 );*/

	for( i=0; i<25; i++ )
	{
		lpScreenCol = lpScreenRow;
		lCol        = lColBase;

		if( (lRow >= 0) && (lRow<apLayer->mpFrame->mHeight) )
		{
			for( j=0; j<80; j++ )
			{
				if( (lCol >= 0) && (lCol<apLayer->mpFrame->mWidth) )
				{
					lTextOff  = lRow;
					lTextOff *= apLayer->mpFrame->mWidth;
					lTextOff += lCol;

					lOff         = apLayer->mpFrame->mpText[ lTextOff ];
					if( lOff != ' ' )
					{
						lpScreenItem = lpScreenCol;
						lpSrc        = apFont->mpChar[ lOff ];

						switch( apLayer->mColour )
						{
						case	1:
							for( k=0; k<8; k++ )
							{
								lpScreenItem[ 0 ]  = lpSrc[ 0 ];
								lpScreenItem[ 2 ]  = 0;
								lpScreenItem      += 160;
								lpSrc             += 160;
							}
							break;
						case	2:
							for( k=0; k<8; k++ )
							{
								lpScreenItem[ 0 ]  = 0;
								lpScreenItem[ 2 ]  = lpSrc[ 0 ];
								lpScreenItem      += 160;
								lpSrc             += 160;
							}
							break;
						case	3:
							for( k=0; k<8; k++ )
							{
								lpScreenItem[ 0 ]  = lpSrc[ 0 ];
								lpScreenItem[ 2 ]  = lpSrc[ 0 ];
								lpScreenItem      += 160;
								lpSrc             += 160;
							}
							break;
						}
					}
				}
				lCol += lColAdd;

				if( j&1 )
				{
					lpScreenCol += 3;
				}
				else
				{
					lpScreenCol += 1;
				}

			}
		}
		lRow += lRowAdd;

		lpScreenRow += (160*8);
	}

	lCycles = System_GetCycleCounter() - lCycles;

	Build_SteemPrintfLine1( "Layer Render C Cycles : %ld", lCycles );
}

#else


/*-----------------------------------------------------------------------------------*
* FUNCTION : void Layer_Render(sLayer * apLayer,U16 * apScreen,sFontAscot * apFont)
* ACTION   : Layer_Render
* CREATION : 01.04.2004 PNK
*-----------------------------------------------------------------------------------*/

void Layer_Render(sLayer * apLayer,U16 * apScreen, sFontAscot * apFont)
{
	S16	lMaxX;
	S16	lX0,lX1,lY0,lY1;
	S16	lCX,lCY;
	S16	lWidth;
	S16	lHeight;
	S16	lTextWidth;
	S16	lTextStepX;
	S32	lOff;
	U16	lMode;
	U8 *	lpScreen;

	U32	lCycles;

	lCycles = System_GetCycleCounter();
	
	lX0 = apLayer->mX.w.w1;
	lY0 = apLayer->mY.w.w1;

	lX1 = lX0 + (apLayer->mpFrame->mWidth);
	lY1 = lY0 + (apLayer->mpFrame->mHeight);

	Build_SteemPrintfLine1( "frame height %d", apLayer->mpFrame->mHeight );

	lCX = 0;
	lCY = 0;

	lTextWidth = apLayer->mpFrame->mWidth;

/*	lMode = Screen_GetMode(); */
	lMode = Video_GetMode();

	if( lX0 < 0 )
	{
		lCX = -lX0;
		lX0 = 0;
	}
	if( lY0 < 0 )
	{
		lCY = -lY0;
		lY0 = 0;
	}

	if( apLayer->mFlipX )
	{
		lCX = (apLayer->mpFrame->mWidth-1) - lCX;
		lTextStepX = -1;
	}
	else
	{
		lTextStepX = 1;
	}

	if( apLayer->mFlipY )
	{
		lCY = (apLayer->mpFrame->mHeight-1) - lCY;
		lTextWidth = -lTextWidth;
	}


	if( lMode == eVIDEO_MODE_2PLANE )
	{
		lMaxX = 80;
	}
	else
	{
		lMaxX = 40;
	}

	if( lX1 > lMaxX )
	{
		lX1 = lMaxX;
	}
	if( lY1 > 25 )
	{
		lY1 = 25;
	}

	lWidth  = lX1 - lX0;
	lHeight = lY1 - lY0;

	if( (lWidth>0) && (lHeight>0) && (lY0<25) && (lX0<80) )
	{
		lpScreen  =  (U8*)apScreen;
		lpScreen += (lY0*(160*8));
		if( lMode == eVIDEO_MODE_2PLANE )
		{
			lpScreen += ((lX0>>1)*4);
		}
		else
		{
			lpScreen += ((lX0>>1)*8);
		}

		if( lX0 & 1 )
		{
			lpScreen++;
		}

		lOff  = (lCY * apLayer->mpFrame->mWidth );
		lOff += lCX;

		Build_SteemPrintfLine2( "Layer Render cy %d framew %d", lCy, apLayer->mpFrame->mWidth );
		switch( lMode )
		{
		case	eVIDEO_MODE_4PLANE:
			switch( apLayer->mColour )
			{
			case	1:
				Layer_Draw_4BP_Col1( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	2:
				Layer_Draw_4BP_Col2( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	3:
				Layer_Draw_4BP_Col3( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	4:
				Layer_Draw_4BP_Col4( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	5:
				Layer_Draw_4BP_Col5( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	6:
				Layer_Draw_4BP_Col6( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	7:
				Layer_Draw_4BP_Col7( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	8:
				Layer_Draw_4BP_Col8( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	9:
				Layer_Draw_4BP_Col9( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	10:
				Layer_Draw_4BP_Col10( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	11:
				Layer_Draw_4BP_Col11( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	12:
				Layer_Draw_4BP_Col12( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	13:
				Layer_Draw_4BP_Col13( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	14:
				Layer_Draw_4BP_Col14( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	15:
				Layer_Draw_4BP_Col15( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			}
			break;

		case	eVIDEO_MODE_2PLANE:
			switch( apLayer->mColour )
			{
			case	1:
				Layer_Draw_2BP_Col1( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	2:
				Layer_Draw_2BP_Col2( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			case	3:
				Layer_Draw_2BP_Col3( &apLayer->mpFrame->mpText[ lOff ], lpScreen, apFont, lWidth, lHeight, lTextWidth, lTextStepX );
				break;
			}
			break;
		}

	}

	lCycles = System_GetCycleCounter() - lCycles;

	Build_SteemPrintfLine1( "Layer Render ASM Cycles : %ld", lCycles );
}

#endif



/*-----------------------------------------------------------------------------------*
* FUNCTION : Layer_RenderTiled( sLayer * apLayers,U16 * apScreen )
* ACTION   : Layer_RenderTiled
* CREATION : 23.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Layer_RenderTiled( sLayer * apLayers,U16 * apScreen )
{
	(void)apLayers;
	(void)apScreen;
}

#ifdef	WIN32

void	Layer_Draw_2BP_Base( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX, U16 aColour )
{
	S16 tx, ty;
	U8 * lpText = apText;
	U8 * lpScreen = apScreen;

	for( ty = 0; ty < aHeight; ty++ )
	{
		lpText = apText;
		lpScreen = apScreen;
		for( tx = 0; tx < aWidth; tx++ )
		{
			U8 * lpGfx = apFont->mpChar[ *lpText ];
			U16 b, y;

			for( y = 0; y < 8; y++ )
			{
				U8 g = *lpGfx++;
				for( b = 0; b < 2; b++ )
				{
					U8 lFill = ( aColour & ( 1 << b ) ) ? 0 : g;
					lpScreen[ b ] = lFill;
				}
			}

			if( (U32)( lpScreen ) & 1 )
				lpScreen += 3;
			else
				lpScreen++;

			lpText += aTextStepX;
		}
		apScreen += ( 160 * 8 );
		apText += aTextWidth;
	}
}

void	Layer_Draw_2BP_Col1( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX ){
	(void)apText; (void)apScreen; (void)apFont; (void)aWidth; (void)aHeight; (void)aTextWidth; (void)aTextStepX;
}
void	Layer_Draw_2BP_Col2( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX ){
	(void)apText; (void)apScreen; (void)apFont; (void)aWidth; (void)aHeight; (void)aTextWidth; (void)aTextStepX;
}
void	Layer_Draw_2BP_Col3( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX ){
	(void)apText; (void)apScreen; (void)apFont; (void)aWidth; (void)aHeight; (void)aTextWidth; (void)aTextStepX;
}

void	Layer_Draw_4BP_Base( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX, U16 aColour )
{
	S16 tx,ty;
	U8 * lpText = apText;
	U8 * lpScreen = apScreen;

	for( ty = 0; ty < aHeight; ty++ )
	{
		lpText = apText;
		lpScreen = apScreen;
		for( tx = 0; tx < aWidth; tx++ )
		{
			U8 * lpGfx = apFont->mpChar[ *lpText ];
			U16 b,y;

			for( y = 0; y < 8; y++ )
			{
				U8 g = *lpGfx++;
				for( b = 0; b < 4; b++ )
				{
					U8 lFill = ( aColour & ( 1 << b ) ) ? 0 : g;
					lpScreen[ b ] = lFill;
				}
			}

			if( (U32)( lpScreen ) & 1 )
				lpScreen += 7;
			else
				lpScreen++;

			lpText += aTextStepX;
		}
		apScreen += ( 160 * 8 );
		apText += aTextWidth;
	}
}

void	Layer_Draw_4BP_Col1(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 1 );
}
void	Layer_Draw_4BP_Col2(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 2 );
}
void	Layer_Draw_4BP_Col3(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 3 );
}
void	Layer_Draw_4BP_Col4(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 4 );
}
void	Layer_Draw_4BP_Col5(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 5 );
}
void	Layer_Draw_4BP_Col6(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 6 );
}
void	Layer_Draw_4BP_Col7(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 7 );
}
void	Layer_Draw_4BP_Col8(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 8 );
}
void	Layer_Draw_4BP_Col9(  U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 9 );
}
void	Layer_Draw_4BP_Col10( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 10 );
}
void	Layer_Draw_4BP_Col11( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 11 );
}
void	Layer_Draw_4BP_Col12( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 12 );
}
void	Layer_Draw_4BP_Col13( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 13 );
}
void	Layer_Draw_4BP_Col14( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 14 );
}
void	Layer_Draw_4BP_Col15( U8 * apText, U8 * apScreen, sFontAscot * apFont, S16 aWidth, S16 aHeight, S16 aTextWidth, S16 aTextStepX )
{
	Layer_Draw_4BP_Base( apText, apScreen, apFont, aWidth, aHeight, aTextWidth, aTextStepX, 15 );
}

void	Layer_DrawPic( U16 * apScreen, const U16 * apGfx ) 
{ 
	Memory_Copy( 32000, apGfx, apScreen );
}

#endif

/* ################################################################################ */
