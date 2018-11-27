/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>

#include	<GODLIB\ASSET\PACKAGE.H>
#include	<GODLIB\AUDIO\AUDIO.H>
#include	<GODLIB\AUDIO\AMIXER.H>
#include	<GODLIB\EXCEPT\EXCEPT.H>
#include	<GODLIB\FADE\FADE.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PACKER\PACKER.H>
#include	<GODLIB\PICTYPES\DEGAS.H>
#include	<GODLIB\SCREEN\SCREEN.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\VIDEO\VIDEO.H>
#include	<GODLIB\VBL\VBL.H>

/* #include	<GODLIB\IKBD\IKBD_DI.H> */
/* #include	<GODLIB\VIDEO\VID_D3D.H> */

#include	<STRING.H>

#include	"BUILD.H"
#include	"AFONT.H"
#include	"FRAME.H"
#include	"IMAGE.H"
#include	"SAMPLE.H"
#include	"SCRIPT.H"
#include	"MUSIC.H"
#include	"MUSMON.H"

#ifdef	WIN32
/* #include	<d3dx8.h> */
/* #include	<mmsystem.h> */
#endif


/* ###################################################################################
#  DEFINES
################################################################################### */


#define	dANIM_SCRIPT_FILENAME	"ANIM.SPT"
#define	dANIM_FRAMES_FILENAME	"ANIM.FRM"
#define	dANIM_FONT_FILENAME		"FONT8X8.PI1"
#define	dANIM_MUSIC_FILENAME	"ANIM.MOD"



/* ###################################################################################
#  ENUMS
################################################################################### */


/* ###################################################################################
#  STRUCTS
################################################################################### */

#define	dMM_LIMIT	96

typedef	struct
{
	U8	mData[ dMM_LIMIT ];
} sMMData;

typedef struct
{
	S32		mSize;
	void *	mpData;
} sDataFile;


/* ###################################################################################
#  DATA
################################################################################### */


/*
sDegas *		gpFontPic;
void *			gpMusic;
char *	gpAnimScriptFile;
char *	gpAnimFramesFile;
S32		gAnimScriptFileSize;
S32		gAnimFramesFileSize;
*/

U16		gAnimPal[ 16 ];


sDataFile	gAnimScriptFile;
sDataFile	gAnimFramesFile;
sDataFile	gAnimMusicFile;
sPackage	gAscotPackage;
sContext	gAscotContext;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	Platform_Init( void );
void	Platform_DeInit( void );

void	Data_Init( void );
void	Data_DeInit( void );


void	DataFile_Load( sDataFile * apDF, char * apFileName );
void	DataFile_UnLoad( sDataFile * apDF );

void	Anim_Init( void );
void	Anim_DeInit( void );
void	Anim_Main( void );
void	Anim_Render( sScriptControl * apScript, U16 * apScreen );
void	Anim_Update( sScriptControl * apScript );

void	MMData_Debug( sMMData * apOld, sMMData * apNew );

void	Ascot_Main( void );

extern	void Layer_DrawPic( U16 * apScreen, const U16 * apGfx );

extern	sMMData	MusicMon_Data;

extern	U16	gMusicMonPat;
extern	U16	gMusicMonPos;

/* ###################################################################################
#  CODE
################################################################################### */

S16	GodLib_Game_Main( S16 aArgCount, char * apArgs[] )
{
	(void)aArgCount;
	(void)apArgs;
	Ascot_Main();
	return( 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Ascot_Main( void )
* ACTION   : Ascot_Main
* CREATION : 09.02.2005 PNK
*-----------------------------------------------------------------------------------*/

void	Ascot_Main( void )
{
	GemDos_Super( 0 );

	Platform_Init();

	if( System_GetMON() == MON_MONO )
	{
		Screen_Init( 640, 400, eGRAPHIC_COLOURMODE_1PLANE, eSCREEN_SCROLL_NONE );
	}
	else
	{
		Screen_Init( 320, 200, eGRAPHIC_COLOURMODE_4PLANE, eSCREEN_SCROLL_NONE );
	}

	Data_Init();
	Anim_Init();

	Anim_Main();

	Anim_DeInit();
	Data_DeInit();
	Screen_DeInit();
	Platform_DeInit();
	
}






/*-----------------------------------------------------------------------------------*
* FUNCTION : Data_Init( void )
* ACTION   : Data_Init
* CREATION : 21.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Data_Init( void )
{
	Context_Init( &gAscotContext, "CONTEXT1" );

	ImageManager_Init();
	SampleManager_Init();
	Font_Init();
	FrameManager_Init();
	MusicMonManager_Init();
	ScriptManager_Init();

	PackageManager_SetLinkEnableFlag( 0 );

	Package_Init( &gAscotPackage, "PACKAGE1", "CONTEXT1" );

	PackageManager_Load( &gAscotPackage );

	while( !PackageManager_OpQueueIsEmpty() )
	{
		PackageManager_Update();
	}

/*	DataFile_Load( &gAnimScriptFile, dANIM_SCRIPT_FILENAME );
	DataFile_Load( &gAnimFramesFile, dANIM_FRAMES_FILENAME );
	DataFile_Load( &gAnimMusicFile,  dANIM_MUSIC_FILENAME  );
*/
/*	Font_FixUp( &gFont, (sDegas*)gAnimFontFile.mpData );*/
/*
	gpAnimScriptFile    = File_Load( dANIM_SCRIPT_FILENAME );
	gpAnimFramesFile    = File_Load( dANIM_FRAMES_FILENAME );
	gAnimScriptFileSize = File_GetSize( dANIM_SCRIPT_FILENAME );
	gAnimFramesFileSize = File_GetSize( dANIM_FRAMES_FILENAME );	
	gpMusic             = File_Load( dANIM_MUSIC_FILENAME );

	Font_Load( &gFont, dANIM_FONT_FILENAME );
*/
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Data_DeInit( void )
* ACTION   : Data_DeInit
* CREATION : 21.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Data_DeInit( void )
{
	PackageManager_UnLoad( &gAscotPackage );
	while( !PackageManager_OpQueueIsEmpty() )
	{
		PackageManager_Update();
	}

/*	DataFile_UnLoad( &gAnimScriptFile );
	DataFile_UnLoad( &gAnimFramesFile );
	DataFile_UnLoad( &gAnimMusicFile  );*/
/*
	File_UnLoad( gpAnimScriptFile );
	File_UnLoad( gpAnimFramesFile );
	File_UnLoad( gpMusic );

	Font_UnLoad( &gFont );
*/
	Package_DeInit( &gAscotPackage );

	ScriptManager_DeInit();
	MusicMonManager_DeInit();
	FrameManager_DeInit();
	Font_DeInit();
	ImageManager_DeInit();
	SampleManager_DeInit();

	Context_DeInit( &gAscotContext );
}





/*-----------------------------------------------------------------------------------*
* FUNCTION : Anim_Init( void )
* ACTION   : Anim_Init
* CREATION : 21.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Anim_Init( void )
{
/*	ScriptReel_Parse( &gScriptReel, gAnimScriptFile.mpData, gAnimScriptFile.mSize );*/
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Anim_DeInit( void )
* ACTION   : Anim_DeInit
* CREATION : 21.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Anim_DeInit( void )
{
/*	ScriptReel_DeInit( &gScriptReel );*/
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Anim_Main( void )
* ACTION   : Anim_Main
* CREATION : 21.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Anim_Main( void )
{
	U8	lExitFlag;
	U16	i;
	sScriptControl *	lpScript;

/*	Script_Init( &gScript );

	gScript.mpReel = &gScriptReel;
*/
	lExitFlag = 0;

/*	MusicMon_Start( gAnimMusicFile.mpData ); 
	Vbl_AddCall( MusicMon_Vbl );
*/

	while( !lExitFlag )
	{
		lpScript = ScriptManager_GetpScript();
		if( lpScript )
		{
			switch( lpScript->mMode )
			{
			case	eMODE_PROCESS:
				Script_Update( lpScript );
				break;

			case	eMODE_DRAW:
				if( lpScript->mScreenModeSetFlag )
				{
					sVideoConfig lConfig;
					Video_GetConfig( &lConfig );
					if( 0 == lpScript->mScreenMode )
					{
						Video_SetMode( 320, 200, eVIDEO_MODE_4PLANE, 320, lConfig.mNTSCFlag, 1 );
					}
					else
					{
						Video_SetMode( 640, 200, eVIDEO_MODE_2PLANE, 320, lConfig.mNTSCFlag, 1 );
					}
					lpScript->mScreenModeSetFlag = 0;
				}

				if( lpScript->mPicPalSetFlag )
				{
					if( lpScript->mpPalette )
					{
						for( i=0; i<16; i++ )
						{
							gAnimPal[ i ] = lpScript->mpPalette->mHeader.mPalette[ i ];
						}
						Video_SetPalST( &gAnimPal[ 0 ] );
					}
					lpScript->mPicPalSetFlag = 0;
					lpScript->mPalSetFlag    = 0;
				}
				if( lpScript->mPalSetFlag )
				{
					for( i=0; i<16; i++ )
					{
						gAnimPal[ i ] = lpScript->mPal[ i ];
					}
					Video_SetPalST( &gAnimPal[ 0 ] );
					lpScript->mPalSetFlag = 0;
				}
				if( lpScript->mFadeRGBFlag )
				{
					for( i=0; i<16; i++ )
					{
						gAnimPal[ i ] = lpScript->mFadeRGB;
					}
					Fade_StartVblFade( &gAnimPal[ 0 ], lpScript->mFadeFrameCount );
					lpScript->mFadeRGBFlag = 0;
				}
				if( lpScript->mPicPalFadeSetFlag )
				{
					if( lpScript->mpFadePalette )
					{
						for( i=0; i<16; i++ )
						{
							gAnimPal[ i ] = lpScript->mpFadePalette->mHeader.mPalette[ i ];
						}
						Fade_StartVblFade( &gAnimPal[ 0 ], lpScript->mFadeFrameCount );
					}
					lpScript->mPicPalFadeSetFlag = 0;
				}

				Screen_Update();
				Anim_Update( lpScript );
				Anim_Render( lpScript, Screen_GetpLogic() );
				if( lpScript->mPause )
				{
					lpScript->mPause--;
				}
				else
				{
					lpScript->mMode = eMODE_PROCESS;
				}
				break;

			case	eMODE_PAUSE:
				if( lpScript->mPause )
				{
					lpScript->mPause--;
					Anim_Update( lpScript );
					Vbl_WaitVbl();
				}
				else
				{
					lpScript->mMode = eMODE_PROCESS;
				}
				break;

			case	eMODE_MUSIC_SYNC:
				if( (gMusicMonPat >= lpScript->mMusicPat) && (gMusicMonPos >= lpScript->mMusicPos) )
				{
					lpScript->mMode = eMODE_PROCESS;
				}
				break;

			case	eMODE_END:
				lExitFlag = 1;
				break;
			}
		}
	}
/*	Vbl_RemoveCall( MusicMon_Vbl );
	MusicMon_Stop();*/
}




/*-----------------------------------------------------------------------------------*
* FUNCTION : Anim_Render( sScriptControl * apScript,U16 * apScreen )
* ACTION   : Anim_Render
* CREATION : 05.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Anim_Render( sScriptControl * apScript,U16 * apScreen )
{
	U16	i;
	

	if( (apScript->mPicDrawFlag) && (apScript->mpPicture) )
	{
		Layer_DrawPic( apScreen, &apScript->mpPicture->mPixels[ 0 ] );
	}
	else
	{
		Memory_Clear( 32000L, apScreen );
	}

	for( i=0; i<dLAYERS_LIMIT; i++ )
	{
		if( apScript->mLayers[ i ].mActiveFlag )
		{
			Layer_Render( &apScript->mLayers[ i ], apScreen, Font_GetpFont() );
		}
	}
	
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : Anim_Update( void )
* ACTION   : Anim_Update
* CREATION : 05.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Anim_Update( sScriptControl * apScript )
{
	U16	i;

	for( i=0; i<dLAYERS_LIMIT; i++ )
	{
		Layer_Update( &apScript->mLayers[ i ] );
	}
	
}

void	MMData_Debug( sMMData * apOld, sMMData * apNew )
{
	U16	i;

	Build_SteemPrint( "MMData_Debug" );
	for( i=0; i<dMM_LIMIT; i++ )
	{
		if( apOld->mData[ i ] != apNew->mData[ i ] )
		{
			Build_SteemPrintfLine3( "Data[ %d ] : %x : %x", i, apOld->mData[ i ], apNew->mData[ i ] );
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : DataFile_Load( sDataFile * apDF,char * apFileName )
* ACTION   : DataFile_Load
* CREATION : 29.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	DataFile_Load( sDataFile * apDF,char * apFileName )
{
	sPackerHeader	lHeader;
	sFileHandle		lHandle;

	apDF->mSize  = 0;
	apDF->mpData = 0;

	lHandle = File_Open( apFileName );
	if( lHandle >= 0 )
	{
		File_Read(  lHandle, sizeof( sPackerHeader ), &lHeader );
		File_Close( lHandle );

		if( Packer_IsPacked( &lHeader ) )
		{
			void * lpTmp;
			apDF->mSize  = Packer_GetDepackSize( &lHeader );
			apDF->mpData = mMEMALLOC( apDF->mSize );

			lpTmp = mMEMALLOC( File_GetSize( apFileName ) );

			File_LoadAt( apFileName, lpTmp );
			Packer_Depack( lpTmp, apDF->mpData );

			mMEMFREE( lpTmp );
		}
		else
		{
			apDF->mSize  = File_GetSize( apFileName );
			apDF->mpData = File_Load( apFileName );
		}
	}
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : DataFile_UnLoad( sDataFile * apDF )
* ACTION   : DataFile_UnLoad
* CREATION : 29.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	DataFile_UnLoad( sDataFile * apDF )
{
	if( apDF->mpData )
	{
		mMEMFREE( apDF->mpData );
	}
}





/* ################################################################################ */
