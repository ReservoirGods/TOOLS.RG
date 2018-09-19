/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"R_SCREEN.H"

#include	"BUILD.H"

#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GRAPHIC\GRAPHIC.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PICTYPES\GFX.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\VIDEO\VIDEO.H>
#include	<GODLIB\VBL\VBL.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

sGraphicCanvas	gRScreenGraphic;
sGraphicCanvas	gRScreenBackGraphic;

U16 *	gpRScreenMem;
U16 *	gpRScreenBuffers[ 3 ];

U16		gRScreenPhysicIndex;
U16		gRScreenSwapMode;

U16		gRScreenOffX60	= 0;
U16		gRScreenOffX50	= 0;

U16		gRScreenBaseHDB;
U16		gRScreenBaseHDE;
U16		gRScreenBaseVDB;
U16		gRScreenBaseVBB;
U16		gRScreenBaseVBE;
U16		gRScreenBaseVDE;

U32		gRScreenLineOffsets[ 200 ];
U32		gRScreenLastVbl;
U32		gRScreenFrameRate = 0;
U8		gRScreenFirstTimeFlag = 0;


U8		gRScreenNTSCFlag = 0;
U8		gRScreenWideScreenFlag = 1;


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_Init( void )
* ACTION   : called at start of game
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

void	RenderScreen_Init( void )
{
	U32	lScreenBase;
	U16	i;

	gpRScreenMem  = Memory_ScreenCalloc( 96000L + 255L );
	lScreenBase   = (U32)gpRScreenMem;
	lScreenBase  += 255L;
	lScreenBase  &= 0xFFFFFF00L;

	gpRScreenBuffers[ 0 ]  = (U16*)lScreenBase;
	lScreenBase           += 32000L;
	gpRScreenBuffers[ 1 ]  = (U16*)lScreenBase;
	lScreenBase           += 32000L;
	gpRScreenBuffers[ 2 ]  = (U16*)lScreenBase;

	gRScreenPhysicIndex = 0;

	if( System_GetRefreshRate()->w.w1 > 55 )
	{
		gRScreenNTSCFlag = 1;
	}
	else
	{
		gRScreenNTSCFlag = 0;
	}
	gRScreenWideScreenFlag = 1;

	RenderScreen_SetResolution();


	switch( System_GetMCH() )
	{
	case	MCH_FALCON:
		gRScreenSwapMode = 0;
		break;

	default:
		gRScreenSwapMode = 1;
		break;
	}

	for( i=0; i<200; i++ )
	{
		gRScreenLineOffsets[ i ] = (160L * i);
	}

	GraphicCanvas_Init( &gRScreenGraphic, eGRAPHIC_COLOURMODE_4PLANE, 320, 200 );
	gRScreenGraphic.mpVRAM = gpRScreenBuffers[ 0 ];

	GraphicCanvas_Init( &gRScreenBackGraphic, eGRAPHIC_COLOURMODE_4PLANE, 320, 200 );
	gRScreenBackGraphic.mpVRAM = gpRScreenBuffers[ 2 ];

#ifndef dBUILD_TOSSCREEN_ACTIVE
	Video_SetPhysic( gpRScreenBuffers[ 0 ] );
#endif
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_Clean( void )
* ACTION   : called at end of game
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

void	RenderScreen_DeInit( void )
{
	mMEMFREE( gpRScreenMem );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_Update( void )
* ACTION   : renders all holes
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

void	RenderScreen_Update( void )
{
	uU32	lScrn;
	S32		lVbls;
#ifndef	WIN32
	U16		lOldBack;
	U16 *	lpReg;
	U16		lResFlag;
#endif
	
	if( !gRScreenFirstTimeFlag )
	{
		gRScreenLastVbl       = Vbl_GetCounter();
		gRScreenFirstTimeFlag = 1;
	}

	gRScreenPhysicIndex ^= 1;
#ifndef dBUILD_TOSSCREEN_ACTIVE
	Video_SetPhysic( gpRScreenBuffers[ gRScreenPhysicIndex ] );
#endif
	if( System_GetMCH() == MCH_ST )
	{
		lScrn.l      = ((U32)gpRScreenBuffers[ gRScreenPhysicIndex ]);
		lScrn.w.w0 >>= 8;
#ifndef	WIN32
		*(U32*)0xFFFF8200L = lScrn.l;
#endif
	}

#ifndef	WIN32
	lpReg = (U16*)0xFFFF8240L;
	lOldBack = lpReg[ 0 ];

	if( IKBD_GetKeyStatus( eIKBDSCAN_TAB ) )
	{
		lpReg[ 0 ] = lOldBack ^ 0xFFF;
		lResFlag = 1;
	}
	else
	{
		lResFlag = 0;
	}
#endif

	do
	{
		Vbl_WaitVbl();
		lVbls = Vbl_GetCounter() - gRScreenLastVbl;
	} while( lVbls < (S32)gRScreenFrameRate );

#ifndef	WIN32
	if( lResFlag )
	{
		lpReg[ 0 ] = lOldBack;
	}
#endif

	gRScreenLastVbl = Vbl_GetCounter();

	Build_CliUpdate();
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_GetpPhysic( void )
* ACTION   : returns pointer to physic
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

U16 *	RenderScreen_GetpPhysic( void )
{
	return( gpRScreenBuffers[ gRScreenPhysicIndex ] );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_GetpLogic( void )
* ACTION   : returns pointer to physic
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

U16 *	RenderScreen_GetpLogic( void )
{
	return( gpRScreenBuffers[ gRScreenPhysicIndex ^ 1 ] );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_GetpBack( void )
* ACTION   : returns pointer to physic
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

U16 *	RenderScreen_GetpBack( void )
{
	return( gpRScreenBuffers[ 2 ] );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_GetPhysicIndex( void )
* ACTION   : returns index of physic - can be used for other funcs which swap structs
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

U16 	RenderScreen_GetPhysicIndex( void )
{
	return( gRScreenPhysicIndex );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_GetLogicIndex( void )
* ACTION   : returns index of logic - can be used for other funcs which swap structs
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

U16 	RenderScreen_GetLogicIndex( void )
{
	return( gRScreenPhysicIndex ^ 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_SetFrameRate( U32 aRate )
* ACTION   : locks framerate to aRate VBLs
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

void	RenderScreen_SetFrameRate( U32 aRate )
{
	gRScreenFrameRate = aRate;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderScreen_GetpScreenBuffer( const U16 aIndex )
* ACTION   : returns pointer to screen buffer aIndex
* CREATION : 30.12.00 PNK
*-----------------------------------------------------------------------------------*/

U16 *	RenderScreen_GetpScreenBuffer( const U16 aIndex )
{
	return( gpRScreenBuffers[ aIndex ] );
}

void	RenderScreen_SetResolution( void )
{
#ifndef dBUILD_TOSSCREEN_ACTIVE
	Video_SetMode( 320, 200, eVIDEO_MODE_4PLANE, 320, gRScreenNTSCFlag, gRScreenWideScreenFlag );
#endif
	Vbl_WaitVbl();
/*	System_Calibrate();*/

	if( System_GetMCH() == MCH_FALCON )
	{
		gRScreenBaseHDB = *(U16*)0xFFFF8288L;
		gRScreenBaseHDE = *(U16*)0xFFFF828AL;
		gRScreenBaseVDB = *(U16*)0xFFFF82A8L;
		gRScreenBaseVBB = *(U16*)0xFFFF82A4L;
		gRScreenBaseVBE = *(U16*)0xFFFF82A6L;
		gRScreenBaseVDE = *(U16*)0xFFFF82AAL;
		if( gRScreenNTSCFlag )
		{
			RenderScreen_SetHorizontalOffset( gRScreenOffX60 );
			RenderScreen_SetVerticalOffset( gRScreenOffX60 );
		}
		else
		{
			RenderScreen_SetHorizontalOffset( gRScreenOffX50 );
			RenderScreen_SetVerticalOffset( gRScreenOffX50 );
		}
	}
	System_CalibrateVbl();
}

void	RenderScreen_SetVerticalOffset( const S16 aOffY )
{
	if( System_GetMCH() == MCH_FALCON )
	{
		*(U16*)0xFFFF82A8L = gRScreenBaseVDB + (aOffY<<1);
		*(U16*)0xFFFF82AAL = gRScreenBaseVDE + (aOffY<<1);
/*		*(U16*)0xFFFF82A4L = gRScreenBaseVBB + (aOffY<<1);
		*(U16*)0xFFFF82A6L = gRScreenBaseVBE + (aOffY<<1);*/
	}
}

void	RenderScreen_SetHorizontalOffset( const S16 aOffX )
{
	if( System_GetMCH() == MCH_FALCON )
	{
		*(U16*)0xFFFF8288L = gRScreenBaseHDB + aOffX;
		*(U16*)0xFFFF828AL = gRScreenBaseHDE + aOffX;
	}
}


/* ################################################################################ */
