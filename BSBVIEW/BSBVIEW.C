/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: MAIN.C
::
:: Some basic library testing stuff
::
:: [c] 2000 Reservoir Gods
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\GRAPHIC\GRAPHIC.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\FONT8X8\FONT8X8.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PICTYPES\DEGAS.H>
#include	<GODLIB\PLATFORM\PLATFORM.H>
#include	<GODLIB\SCREEN\SCREEN.H>
#include	<GODLIB\SPRITE\SPRITE.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\VBL\VBL.H>
#include	<GODLIB\VIDEO\VIDEO.H>

#include	<STDIO.H>
#include	<STRING.H>


/* ###################################################################################
#  DATA
################################################################################### */

U16 *			gpRScreenMem;
U16 *			gpRScreenBuffers[ 3 ];


U16				gRScreenPhysicIndex;
U16				gRScreenSwapMode;

U32				gRScreenLineOffsets[ 200 ];
U32				gRScreenLastVbl;
U32				gRScreenFrameRate = 0;
U8				gRScreenFirstTimeFlag = 0;

U16				gSpace[ 16000 ];

extern	U16 gGraphicBlitRout;


#define	dBLIT_ROUT_RIGHT		1
#define	dBLIT_ROUT_CENTRE		2
#define	dBLIT_ROUT_LEFT			4
#define	dBLIT_ROUT_ALIGNED		8
#define	dBLIT_ROUT_FXSR			16
#define	dBLIT_ROUT_NFSR			32


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	Main_Init( void );
void	Main_DeInit( void );
void	Test_Loop( char * apFileName );

void	BlitInfo_Build( void );


/* ###################################################################################
#  FUNCTIONS
################################################################################### */


S16	GodLib_Game_Main( S16 aArgCount, char * apArgs[] )
{
	if( aArgCount < 2 )
	{
		printf( "Usage : BSBVIEW <bsbfile>\n" );
		GemDos_Cnecin();
	}
	else
	{
		GemDos_Super( 0 );

		Platform_Init();

		Screen_Init( 320, 200, eGRAPHIC_COLOURMODE_4PLANE, eSCREEN_SCROLL_NONE );

		Test_Loop( apArgs[ 1 ] );

		Screen_DeInit();

		Platform_DeInit();

	}
	return( 0 );
}



void	Test_Loop( char * apFileName )
{
	char lDegasName[ 16 ];
	sDegasHeader lHeader;
	sFileHandle lDegasHandle;
	U16 lIndex = 0;
	U16 lRate = 4;
	U16 i;
	sSpriteBlock * lpSpriteBlock = File_Load( apFileName );
	Sprite_BlockRelocate( lpSpriteBlock );

	for( i = 0; i < 8 && '.' != apFileName[ i ]; i++ )
		lDegasName[ i ] = apFileName[ i ];
	lDegasName[ i++ ] = '.';
	lDegasName[ i++ ] = 'P';
	lDegasName[ i++ ] = 'I';
	lDegasName[ i++ ] = '1';
	lDegasName[ i++ ] = 0;

	lDegasHandle = File_Open( lDegasName );
	if( lDegasHandle )
	{
		File_Read( lDegasHandle, sizeof( sDegasHeader ), &lHeader );
		File_Close( lDegasHandle );
		Video_SetPalST( lHeader.mPalette );
	}

	while( !IKBD_GetKeyStatus( eIKBDSCAN_SPACE ) )
	{
		sSprite *		lpSprite = lpSpriteBlock->mpSprite[ lIndex ];
		sGraphicPos		lPos;

		Screen_Update();
		IKBD_Update();

		Screen_GetpLogicGraphic()->mpClipFuncs->ClearScreen( Screen_GetpLogicGraphic() );

		lPos.mX = 160 - ( lpSprite->mWidth >> 1 );
		lPos.mY = 100 - ( lpSprite->mHeight >> 1 );

		Screen_GetpLogicGraphic()->mpClipFuncs->DrawSprite(
			Screen_GetpLogicGraphic(),
			&lPos,
			lpSprite );

		if( --lRate <= 0 )
		{
			lIndex++;
			if( lIndex >= lpSpriteBlock->mHeader.mSpriteCount )
				lIndex = 0;
			lRate = 4;
		}
	}

	File_UnLoad( lpSpriteBlock );

#if 0
	sGraphicPos				lPos;
	sSpriteBlock	*	lpSprite;
	sGraphicCanvas			lCanvas;
	sDegas *				lpPic;
	U16						i;

	lpPic     = File_Load( apFileName );
	Video_SetPalST( &lpPic->mHeader.mPalette[ 0 ] );
	lpSprite  = File_Load( apFileName );
	Sprite_BlockRelocate( lpSprite );

	GraphicCanvas_Init( &lCanvas, eGRAPHIC_COLOURMODE_4BP, 320, 200 );
	GraphicCanvas_SetpVRAM( &lCanvas, gpRScreenBuffers[ 0 ] );

	for( i=0; i<lpSprite->Header.SpriteCount; i++ )
	{
		lPos.X = 160;
		lPos.Y = 100;

		lPos.X -= lpSprite->pSprite[ 0 ]->Width;
		lPos.Y -= lpSprite->pSprite[ 0 ]->Height;

		lCanvas.pFuncs->ClearScreen( &lCanvas );
		lCanvas.pFuncs->DrawSprite( &lCanvas, &lPos, lpSprite->pSprite[ i ] );

		while( *(U8*)0xFFFFFC02L != 0x39 );
		while( *(U8*)0xFFFFFC02L == 0x39 );

	}
	Memory_Release( lpSprite );
#endif
}

