/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"PLATFORM.H"

#include	"BUILD.H"

#include	"R_FNT.H"
#include	"R_SPR.H"

#include	<GODLIB\ASSET\PACKAGE.H>
#include	<GODLIB\ASSET\RELOCATE.H>
#include	<GODLIB\AUDIO\AUDIO.H>
#include	<GODLIB\CLOCK\CLOCK.H>
#include	<GODLIB\EXCEPT\EXCEPT.H>
#include	<GODLIB\FADE\FADE.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GRAPHIC\GRAPHIC.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\PROFILER\PROFILER.H>
#include	<GODLIB\RANDOM\RANDOM.H>
#include	<GODLIB\SCRNGRAB\SCRNGRAB.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\VBL\VBL.H>
#include	<GODLIB\VIDEO\VIDEO.H>


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : Platform_Init( void )
* ACTION   : calls all initialisation routines
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	Platform_Init( void )
{
	Build_DebugPrintf( "System_Init()\n" );
	System_Init();
#ifdef	dBUILD_EXCEPTIONSCREEN_ACTIVE
	Build_DebugPrintf( "Except_Init()\n" );
	Except_Init();
#endif
	Build_DebugPrintf( "Vbl_Init()\n" );
	Vbl_Init();
	Build_DebugPrintf( "Video_Init()\n" );
	Video_Init();
	Build_DebugPrintf( "IKBD_Init()\n" );
#ifndef	dBUILD_MONST
	IKBD_Init();
#endif
	Build_DebugPrintf( "Audio_Init()\n" );
	Audio_Init();
	Build_DebugPrintf( "Cache_Init()\n" );
	System_DataCacheDisable();
	System_SetCPUSpeed( 16 );
	Build_DebugPrintf( "JoySticks_Init()\n" );
	IKBD_EnableJoysticks();
	Build_DebugPrintf( "Graphic_Init()\n" );
	Graphic_Init();
#ifdef	dBUILD_PROFILER_ACTIVE
	Build_DebugPrintf( "Profiler_Init()\n" );
	Profiler_Init( dBUILD_NUMBER_HI, dBUILD_NUMBER_LO );
#endif

#ifndef	dBUILD_MASTER
	Build_DebugPrintf( "ScreenGrab_Init()\n" );
	ScreenGrab_Init();
	ScreenGrab_SetDirectory( "SCRNGRAB\\" );
	ScreenGrab_Enable();
#endif

	Random_Init();
	Fade_Init();
	Clock_Init();

	PackageManager_Init();
	RelocaterManager_Init();
	PackageManager_SetFilePath( "UNLINK" );
	PackageManager_SetLinkPath( "DATA" );

	RenderFont_Init();
	RenderSprite_Init();
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Platform_DeInit( void )
* ACTION   : calls all de-initialisation routines
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	Platform_DeInit( void )
{
	RenderFont_DeInit();
	RenderSprite_DeInit();

	Clock_DeInit();
	Fade_DeInit();
#ifndef	dBUILD_MASTER
	Build_DebugPrintf( "ScreenGrab_Disable()\n" );
	ScreenGrab_Disable();
	Build_DebugPrintf( "ScreenGrab_DeInit()\n" );
	ScreenGrab_DeInit();
#endif

#ifdef	dBUILD_PROFILER_ACTIVE
	Build_DebugPrintf( "Profiler_DeInit()\n" );
	Profiler_DeInit();
#endif
	Build_DebugPrintf( "Graphic_DeInit()\n" );
	Graphic_DeInit();
	Build_DebugPrintf( "Audio_DeInit()\n" );
	Audio_DeInit();
	Build_DebugPrintf( "IKBD_EnableMouse()\n" );
	IKBD_EnableMouse();
	Build_DebugPrintf( "IKBD_DeInit()\n" );
#ifndef	dBUILD_MONST
	IKBD_DeInit();
#endif
	Build_DebugPrintf( "Video_DeInit()\n" );
	Video_DeInit();
	Build_DebugPrintf( "Vbl_DeInit()\n" );
	Vbl_DeInit();
#ifdef	dBUILD_EXCEPTIONSCREEN_ACTIVE
	Build_DebugPrintf( "Except_DeInit()\n" );
	Except_DeInit();
#endif
	Build_DebugPrintf( "System_DeInit()\n" );
	System_DeInit();
	Build_DebugPrintf( "Vbl_WaitVbl()\n" );
	Vbl_WaitVbl();
	Build_DebugPrintf( "IKBD_FlushGemDos()\n" );
	IKBD_FlushGemdos();
	Build_DebugPrintf( "Audio_SoundChipOff()\n" );
	Audio_SoundChipOff();

	PackageManager_DeInit();
	RelocaterManager_DeInit();
}


/* ################################################################################ */
