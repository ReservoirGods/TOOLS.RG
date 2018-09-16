#ifndef	INCLUDED_SCRIPT_H
#define	INCLUDED_SCRIPT_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>

#include	"FRAME.H"
#include	"LAYER.H"


/* ###################################################################################
#  DEFINES
################################################################################### */

#define	dLAYERS_LIMIT	8


/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	eCMD_NULL,

	eCMD_COLOUR,
	eCMD_CURLYOPEN,
	eCMD_CURLYCLOSE,
	eCMD_DRAW,
	eCMD_END,
	eCMD_PAUSE,
	eCMD_RATE,
	eCMD_SCREEN_MODE,
	eCMD_FADE_TORGB,
	eCMD_FADE_TOPICPAL,
	eCMD_PIC,
	eCMD_PIC_SETPAL,
	eCMD_PIC_ON,
	eCMD_PIC_OFF,
	eCMD_LAYER_COLOUR,
	eCMD_LAYER_ON,
	eCMD_LAYER_OFF,
	eCMD_LAYER_FLIPX,
	eCMD_LAYER_FLIPY,
	eCMD_LAYER_FRAME,
	eCMD_LAYER_SCROLLX,
	eCMD_LAYER_SCROLLY,
	eCMD_LAYER_TILEX,
	eCMD_LAYER_TILEY,
	eCMD_LAYER_X,
	eCMD_LAYER_Y,
	eCMD_MUSIC_SYNC,
	eCMD_MUSICMON_PLAY,
	eCMD_MUSICMON_STOP,
	eCMD_SAMPLE,
	eCMD_SQUAREOPEN,
	eCMD_SQUARECLOSE,

	eCMD_LIMIT
};

enum
{
	eMODE_PROCESS,
	eMODE_DRAW,
	eMODE_PAUSE,
	eMODE_MUSIC_SYNC,

	eMODE_END,
};


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct
{
	U16	mCommand;
	U32	mArgument[ 2 ];
} sScriptCommand;

typedef	struct
{
	U32					mCount;
	sScriptCommand *	mpCmds;
} sScriptReel;


typedef struct
{
	U16					mMode;
	U16					mScriptIndex;
	U16					mFadeFrameCount;
	U16					mFadeRGB;
	U16					mFadeRGBFlag;
	U16					mFrameRate;
	U16					mMusicPat;
	U16					mMusicPos;
	S32					mPause;
	U16					mPalSetFlag;
	U16					mPicDrawFlag;
	U16					mPicPalSetFlag;
	U16					mPicPalFadeSetFlag;
	U16					mScreenModeSetFlag;
	U16					mScreenMode;
	U16					mPal[ 16 ];
	sLayer				mLayers[ dLAYERS_LIMIT ];
	sDegas *			mpPicture;
	sDegas *			mpPalette;
	sDegas *			mpFadePalette;
	sScriptReel *		mpReel;
} sScriptControl;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	ScriptReel_Init(   sScriptReel * apReel );
void	ScriptReel_DeInit( sScriptReel * apReel );
void	ScriptReel_Parse(  sScriptReel * apReel, const char * apText, const S32 aSize );

void	Script_Init(   sScriptControl * apScript );
void	Script_Update( sScriptControl * apScript );

void	ScriptManager_Init( void );
void	ScriptManager_DeInit( void );
void	ScriptManager_Update( void );

sScriptControl *	ScriptManager_GetpScript( void );


/* ################################################################################ */

#endif	/* INCLUDED_SCRIPT_H */
