/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB/AUDIO/AUDIO.H>
#include	<GODLIB/AUDIO/AMIXER.H>
#include	<GODLIB/CLI/CLI.H>
#include	<GODLIB/DEBUGLOG/DEBUGLOG.H>
#include	<GODLIB/FADE/FADE.H>
#include	<GODLIB/GEMDOS/GEMDOS.H>
#include	<GODLIB/IKBD/IKBD.H>
#include	<GODLIB/HASHTREE/HASHTREE.H>
#include	<GODLIB/KERNEL/KERNEL.H>
#include	<GODLIB/MEMORY/MEMORY.H>
#include	<GODLIB/PICTYPES/GOD.H>
#include	<GODLIB/PLATFORM/PLATFORM.H>
#include	<GODLIB/SCREEN/SCREEN.H>
#include	<GODLIB/SYSTEM/SYSTEM.H>
#include	<GODLIB/VIDEO/VIDEO.H>


/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	eGODVIEW_TASK_SHOW,

	eGODVIEW_TASK_LIMIT
};

/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef struct	sGodViewClass
{
	sGod *	mpGod;
	U16		mX;
	U16		mY;
	U16		mRedrawFlag;
} sGodViewClass;

/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	GodViewTask_Show_Init( void );
void	GodViewTask_Show_DeInit( void );
U16		GodViewTask_Show_Update( sInput * apInput );

void	GodView_GodRender( sGraphicCanvas * apCanvas, sGod * apGod, U16 aX, U16 aY );


/* ###################################################################################
#  DATA
################################################################################### */

sGodViewClass	gGodViewClass;

sKernelTask		gGodViewTasks[ eGODVIEW_TASK_LIMIT ] =
{
	{	eGODVIEW_TASK_SHOW,	0,	GodViewTask_Show_Init,	GodViewTask_Show_DeInit,		GodViewTask_Show_Update	},
};


/* ###################################################################################
#  CODE
################################################################################### */


/*-----------------------------------------------------------------------------------*
* FUNCTION : main( void )
* ACTION   : it begins  ...
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	GodLib_Game_Main( S16 aArgCount, char * apArgs[] )
{
	Memory_Clear( sizeof(sGodViewClass), &gGodViewClass );

	GemDos_Super( 0 );

	Platform_Init();

	if( ( System_GetEMU() == EMU_STEEM ) && ( System_GetEmuVersion() < 0x0240 ) )
	{
		printf( "This game requires STEEM version 2.4\n\n" );
		printf( "Please download new version from:\n" );
		printf( "http://steem.atari.org\n" );
		GemDos_Cnecin();
	}
	else
	{
		if( aArgCount <= 1 )
		{
			printf( "no filename specified\n" );
			GemDos_Cnecin();
		}
		else
		{
			gGodViewClass.mpGod = File_Load( apArgs[1] );

			if( gGodViewClass.mpGod )
			{
				Endian_FromBigU16( &gGodViewClass.mpGod->mHeader.mHeight );
				Endian_FromBigU16( &gGodViewClass.mpGod->mHeader.mWidth  );

				Kernel_Init( gGodViewTasks, eGODVIEW_TASK_LIMIT, 0, 0, 0 );

				Screen_Init( 320, 200, eGRAPHIC_COLOURMODE_16BPP, 0 );
				IKBD_EnableMouse();
				IKBD_SetMousePosition( 0, 0 );
				Kernel_InputsEnumerate();

				Kernel_Main();

				Screen_DeInit();

				Kernel_DeInit();
			}
			else
			{
				printf( "couldn't load file %s\n", apArgs[1] );
			}
		}
	}
	Platform_DeInit();	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : GodViewTask_Show_Init( void )
* ACTION   : GodViewTask_Show_Init
* CREATION : 11.04.2005 PNK
*-----------------------------------------------------------------------------------*/

void	GodViewTask_Show_Init( void )
{
	gGodViewClass.mRedrawFlag = 2;	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : GodViewTask_Show_DeInit( void )
* ACTION   : GodViewTask_Show_DeInit
* CREATION : 11.04.2005 PNK
*-----------------------------------------------------------------------------------*/

void	GodViewTask_Show_DeInit( void )
{
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : GodViewTask_Show_Update( sInput * apInput )
* ACTION   : GodViewTask_Show_Update
* CREATION : 11.04.2005 PNK
*-----------------------------------------------------------------------------------*/

U16	GodViewTask_Show_Update( sInput * apInput )
{
	U16	lRes;
	U16	lClip;
	S16	lX,lY;
	
	lRes = eGODVIEW_TASK_SHOW;

	lX = IKBD_GetMouseX();
	lY = IKBD_GetMouseY();

	lClip = 0;

	if( apInput->mKeyStatus[ eINPUTKEY_UP ] & eINPUTKEYSTATUS_HELD )
	{
		lY    = (S16)gGodViewClass.mY - 1;
		lClip = 1;
	}
	if( apInput->mKeyStatus[ eINPUTKEY_DOWN ] & eINPUTKEYSTATUS_HELD )
	{
		lY = (S16)gGodViewClass.mY + 1;
		lClip = 1;
	}
	if( apInput->mKeyStatus[ eINPUTKEY_LEFT ] & eINPUTKEYSTATUS_HELD )
	{
		lX = (S16)gGodViewClass.mX + 1;
		lClip = 1;
	}
	if( apInput->mKeyStatus[ eINPUTKEY_RIGHT ] & eINPUTKEYSTATUS_HELD )
	{
		lX = (S16)gGodViewClass.mX - 1;
		lClip = 1;
	}


	if( (lX+320) > gGodViewClass.mpGod->mHeader.mWidth )
	{
		lX    = gGodViewClass.mpGod->mHeader.mWidth - 320;
		lClip = 1;
	}

	if( (lY+200) > gGodViewClass.mpGod->mHeader.mHeight )
	{
		lY    = gGodViewClass.mpGod->mHeader.mHeight - 200;
		lClip = 1;
	}

	if( lX < 0 )
	{
		lX    = 0;
		lClip = 1;
	}

	if( lY < 0 )
	{
		lY    = 0;
		lClip = 1;
	}

	if( lClip )
	{
		IKBD_SetMousePosition( lX, lY );
	}



	if( (lX != gGodViewClass.mX) || (lY != gGodViewClass.mY) )
	{
		gGodViewClass.mX = lX;
		gGodViewClass.mY = lY;
		gGodViewClass.mRedrawFlag = 2;
	}

	if( gGodViewClass.mRedrawFlag )
	{
		Screen_GetpLogicGraphic()->mpFuncs->ClearScreen( Screen_GetpLogicGraphic() );
		GodView_GodRender( Screen_GetpLogicGraphic(), gGodViewClass.mpGod, gGodViewClass.mX, gGodViewClass.mY );
	}

	if( apInput->mKeyStatus[ eINPUTKEY_FIREA ] & eINPUTKEYSTATUS_HIT )
	{
		Kernel_RequestShutdown();
	}

	return( lRes );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : GodView_GodRender( sGraphicCanvas * apCanvas,sGod * apGod,U16 aX,U16 aY )
* ACTION   : GodView_GodRender
* CREATION : 11.04.2005 PNK
*-----------------------------------------------------------------------------------*/

void	GodView_GodRender( sGraphicCanvas * apCanvas,sGod * apGod,U16 aX,U16 aY )
{
	U16 *	lpSrc0;
	U16 *	lpSrc1;
	U16 *	lpDst0;
	U16 *	lpDst1;
	U16		lX;
	S16		lW,lH;
	U32		lOff;
	
	if( apCanvas && apGod )
	{
		lOff   = aY;
		lOff  *=(apGod->mHeader.mWidth);
		lpDst0 = apCanvas->mpVRAM;
		lpSrc0 =&apGod->mPixels[lOff];
		lH     = apGod->mHeader.mHeight;
		lH    -= aY;
		if( lH > apCanvas->mHeight )
		{
			lH = apCanvas->mHeight;
		}

		lW     = apGod->mHeader.mWidth;
		lW    -= aX;
		if( lW > apCanvas->mWidth )
		{
			lW = apCanvas->mWidth;
		}

		if( (lW > 0) && (lH > 0) )
		{
			while( lH )
			{
				lpDst1 = lpDst0;
				lpSrc1 = lpSrc0 + aX;


				lX = lW;
				while( lX )
				{
					*lpDst1++ = *lpSrc1++;
					lX--;
				}
				lpSrc0 += apGod->mHeader.mWidth;
				lpDst0 += apCanvas->mWidth;

				lH--;
			}
		}
	}
}



