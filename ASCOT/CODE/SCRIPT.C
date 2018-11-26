/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"SCRIPT.H"

#include	"BUILD.H"
#include	"HASH.H"
#include	"IMAGE.H"
#include	"MUSMON.H"
#include	"SAMPLE.H"
#include	"TOKENISE.H"

#include	<GODLIB\ASSET\ASSET.H>
#include	<GODLIB\AUDIO\AMIXER.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\SCREEN\SCREEN.H>


/* ###################################################################################
#  DATA
################################################################################### */

sAssetClient	gScriptAsset;
sScriptReel		gScriptReel;
sScriptControl	gScriptControl;

sTagString	gCmdTokens[ eCMD_LIMIT ] =
{
	{	eCMD_NULL,			"null"			},

	{	eCMD_COLOUR,		"COLOUR"		},
	{	eCMD_CURLYCLOSE,	"}"				},
	{	eCMD_CURLYOPEN,		"{"				},
	{	eCMD_DRAW,			"DRAW"			},
	{	eCMD_END,			"END"			},
	{	eCMD_FADE_TORGB,	"FADE_TORGB"	},
	{	eCMD_FADE_TOPICPAL,	"FADE_TOPICPAL"	},
	{	eCMD_LAYER_COLOUR,	"LAYER_COLOUR"	},
	{	eCMD_LAYER_FRAME,	"LAYER_FRAME"	},
	{	eCMD_LAYER_ON,		"LAYER_ON"		},
	{	eCMD_LAYER_OFF,		"LAYER_OFF"		},
	{	eCMD_LAYER_X,		"LAYER_X"		},
	{	eCMD_LAYER_Y,		"LAYER_Y"		},
	{	eCMD_LAYER_SCROLLX,	"LAYER_SCROLLX"	},
	{	eCMD_LAYER_SCROLLY,	"LAYER_SCROLLY"	},
	{	eCMD_LAYER_TILEX,	"LAYER_TILEX"	},
	{	eCMD_LAYER_TILEY,	"LAYER_TILEY"	},
	{	eCMD_LAYER_FLIPX,	"LAYER_FLIPX"	},
	{	eCMD_LAYER_FLIPY,	"LAYER_FLIPY"	},
	{	eCMD_MUSICMON_PLAY,	"MUSICMON_PLAY"	},
	{	eCMD_MUSICMON_STOP,	"MUSICMON_STOP"	},
	{	eCMD_MUSIC_SYNC,	"MUSIC_SYNC"	},
	{	eCMD_PAUSE,			"PAUSE"			},
	{	eCMD_PIC,			"PIC"			},
	{	eCMD_PIC_SETPAL,	"PIC_SETPAL"	},
	{	eCMD_PIC_ON,		"PIC_ON"		},
	{	eCMD_PIC_OFF,		"PIC_OFF"		},
	{	eCMD_RATE,			"RATE"			},
	{	eCMD_SAMPLE,		"SAMPLE"		},
	{	eCMD_SCREEN_MODE,	"SCREEN_MODE"	},
	{	eCMD_SQUAREOPEN,	"["				},
	{	eCMD_SQUARECLOSE,	"]"				},
};



/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32		Script_OnLoad( void * apData, const U32 aSize, const U32 aUserData );
U32		Script_OnUnLoad( void * apData, const U32 aSize, const U32 aUserData );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : ScriptReel_Init( sScriptReel * apReel )
* ACTION   : ScriptReel_Init
* CREATION : 04.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ScriptReel_Init( sScriptReel * apReel )
{
	apReel->mCount = 0;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ScriptReel_DeInit( sScriptReel * apReel )
* ACTION   : ScriptReel_DeInit
* CREATION : 04.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ScriptReel_DeInit( sScriptReel * apReel )
{
	apReel->mCount = 0;
	if( apReel->mpCmds )
	{
		mMEMFREE( apReel->mpCmds );
		apReel->mpCmds = 0;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ScriptReel_Parse( sScriptReel * apReel,const char * apText,const U32 aSize )
* ACTION   : ScriptReel_Parse
* CREATION : 04.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ScriptReel_Parse( sScriptReel * apReel, const char * apText,const S32 aSize )
{
	U8		lExitFlag;
	sArgs	lArgs;
	S32		lOffset;
	S32		lLayerIndex;
	U16		lLineCount;
	sScriptCommand *	lpCmd;

	ScriptReel_Init( apReel );


	lOffset   = 0;

	lLineCount = 0;

	while( lOffset < aSize )
	{
		lOffset += Args_Build( &lArgs, &apText[ lOffset ] );

		if( lArgs.mArgCount )
		{
			lLineCount++;
		}
	}

	apReel->mpCmds = Memory_Calloc( sizeof( sScriptCommand ) * lLineCount );


	lOffset        = 0;
	lExitFlag      = 0;
	apReel->mCount = 0;

	while( (!lExitFlag) && (lOffset < aSize) )
	{
		lpCmd = &apReel->mpCmds[ apReel->mCount ];

		lOffset += Args_Build( &lArgs, &apText[ lOffset ] );

		if( lArgs.mArgCount >= 1 )
		{
			switch( (U16)sTagString_GetID( lArgs.mpArgs[ 0 ], gCmdTokens, eCMD_LIMIT ) )
			{
			case	eCMD_DRAW:

				lpCmd->mCommand = eCMD_DRAW;
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mArgument[ 0 ] = String_GrabNumber( lArgs.mpArgs[ 1 ] );
				}
				else
				{
					lpCmd->mArgument[ 0 ] = 0;
				}

				apReel->mCount++;
				break;

			case	eCMD_COLOUR:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = (U16)String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < 16 )
					{
						lpCmd->mCommand = eCMD_COLOUR;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_END:
				lpCmd->mCommand = eCMD_END;
				apReel->mCount++;
				break;

			case	eCMD_FADE_TORGB:
				if( lArgs.mArgCount >= 3 )
				{
					lpCmd->mCommand = eCMD_FADE_TORGB;
					lpCmd->mArgument[ 0 ] = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_FADE_TOPICPAL:
				if( lArgs.mArgCount >= 3 )
				{
					lpCmd->mCommand = eCMD_FADE_TOPICPAL;
					lpCmd->mArgument[ 0 ] = (U32)ImageManager_Register( lArgs.mpArgs[ 1 ] );
					lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_MUSICMON_PLAY:
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mCommand = eCMD_MUSICMON_PLAY;
					lpCmd->mArgument[ 0 ] = (U32)MusicMonManager_Register( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_MUSICMON_STOP:
				lpCmd->mCommand = eCMD_MUSICMON_STOP;
				apReel->mCount++;
				break;

			case	eCMD_PAUSE:
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mCommand       = eCMD_PAUSE;
					lpCmd->mArgument[ 0 ] = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_PIC:
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mCommand = eCMD_PIC;
					lpCmd->mArgument[ 0 ] = (U32)ImageManager_Register( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_SAMPLE:
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mCommand = eCMD_SAMPLE;
					lpCmd->mArgument[ 0 ] = (U32)SampleManager_Register( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_PIC_SETPAL:
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mCommand = eCMD_PIC_SETPAL;
					lpCmd->mArgument[ 0 ] = (U32)ImageManager_Register( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_PIC_ON:
				lpCmd->mCommand = eCMD_PIC_ON;
				apReel->mCount++;
				break;

			case	eCMD_PIC_OFF:
				lpCmd->mCommand = eCMD_PIC_OFF;
				apReel->mCount++;
				break;

			case	eCMD_RATE:
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mCommand       = eCMD_RATE;
					lpCmd->mArgument[ 0 ] = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_SCREEN_MODE:
				if( lArgs.mArgCount >= 2 )
				{
					lpCmd->mCommand       = eCMD_SCREEN_MODE;
					lpCmd->mArgument[ 0 ] = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			case	eCMD_LAYER_FRAME:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_FRAME;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = (U32)FrameManager_GetpFrame( lArgs.mpArgs[2] );
						Build_SteemPrintfLine2( "Layer Frame %s %ld", lArgs.mpArgs[2], lpCmd->mArgument[ 1 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_COLOUR:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand = eCMD_LAYER_COLOUR;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_ON:
				if( lArgs.mArgCount >= 2 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand = eCMD_LAYER_ON;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_OFF:
				if( lArgs.mArgCount >= 2 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand = eCMD_LAYER_OFF;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_X:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_X;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_Y:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_Y;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_SCROLLX:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_SCROLLX;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_SCROLLY:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_SCROLLY;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_FLIPX:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_FLIPX;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_FLIPY:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_FLIPY;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_TILEX:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_TILEX;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_LAYER_TILEY:
				if( lArgs.mArgCount >= 3 )
				{
					lLayerIndex = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					if( lLayerIndex < dLAYERS_LIMIT )
					{
						lpCmd->mCommand       = eCMD_LAYER_TILEY;
						lpCmd->mArgument[ 0 ] = lLayerIndex;
						lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 2 ] );
						apReel->mCount++;
					}
				}
				break;

			case	eCMD_MUSIC_SYNC:
				Build_SteemPrint( "Script_Parse() music sync" );
				if( lArgs.mArgCount >= 3 )
				{
					Build_SteemPrint( "Script_Parse() music sync enuf cmds" );
					lpCmd->mCommand       = eCMD_MUSIC_SYNC;
					lpCmd->mArgument[ 0 ] = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					lpCmd->mArgument[ 1 ] = String_GrabNumber( lArgs.mpArgs[ 1 ] );
					apReel->mCount++;
				}
				break;

			}
		}

	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Script_Update( sScriptControl * apScript )
* ACTION   : Script_Update
* CREATION : 05.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Script_Update( sScriptControl * apScript )
{
	sScriptCommand *	lpCmd;
	sImage *			lpImage;
	sAscotSpl *			lpSpl;

	if( apScript->mScriptIndex < apScript->mpReel->mCount )
	{
		lpCmd = &apScript->mpReel->mpCmds[ apScript->mScriptIndex ];

		switch( lpCmd->mCommand )
		{

		case	eCMD_COLOUR:
			apScript->mPal[ lpCmd->mArgument[ 0 ] & 0xF ] = (U16)lpCmd->mArgument[ 1 ];
			apScript->mPalSetFlag = 1;
			break;

		case	eCMD_DRAW:
			apScript->mMode  = eMODE_DRAW;
			apScript->mPause = lpCmd->mArgument[ 0 ];
			break;

		case	eCMD_END:
			apScript->mMode = eMODE_END;
			break;

		case	eCMD_FADE_TORGB:
			apScript->mFadeRGB        = (U16)lpCmd->mArgument[ 0 ];
			apScript->mFadeFrameCount = (U16)lpCmd->mArgument[ 1 ];
			apScript->mFadeRGBFlag    = 1;
			break;

		case	eCMD_FADE_TOPICPAL:
			lpImage                   = (sImage*)lpCmd->mArgument[ 0 ];
			apScript->mpFadePalette   = lpImage->mpPicture;
			apScript->mFadeFrameCount = (U16)lpCmd->mArgument[ 1 ];
			apScript->mPicPalFadeSetFlag = 1;
			break;

		case	eCMD_PAUSE:
			apScript->mPause = lpCmd->mArgument[ 0 ];
			apScript->mMode  = eMODE_PAUSE;
			break;

		case	eCMD_RATE:
			apScript->mFrameRate = (U16)lpCmd->mArgument[ 0 ];
			break;

		case	eCMD_SCREEN_MODE:
			apScript->mScreenModeSetFlag = 1;
			apScript->mScreenMode = (U16)lpCmd->mArgument[ 0 ];
			break;

		case	eCMD_LAYER_COLOUR:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mColour = (U16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_ON:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mActiveFlag = 1;
			break;

		case	eCMD_LAYER_OFF:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mActiveFlag = 0;
			break;

		case	eCMD_LAYER_FLIPX:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mFlipX = (U16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_FLIPY:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mFlipY = (U16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_FRAME:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mpFrame = (sFrame*)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_SCROLLX:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mScrollX.l = lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_SCROLLY:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mScrollY.l = lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_TILEX:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mTileX = (U16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_TILEY:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mTileY = (U16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_X:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mX.w.w1 = (S16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_LAYER_Y:
			apScript->mLayers[ lpCmd->mArgument[ 0 ] ].mY.w.w1 = (S16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_MUSICMON_PLAY:
			MusicMonManager_Play( (sMusicMonTune*)lpCmd->mArgument[ 0 ] );
			break;

		case	eCMD_MUSICMON_STOP:
			MusicMonManager_Stop();
			break;

		case	eCMD_MUSIC_SYNC:
			Build_SteemPrint( "Script_Update() music sync" );
			apScript->mMode     = eMODE_MUSIC_SYNC;
			apScript->mMusicPat = (U16)lpCmd->mArgument[ 0 ];
			apScript->mMusicPos = (U16)lpCmd->mArgument[ 1 ];
			break;

		case	eCMD_PIC:
			lpImage = (sImage*)lpCmd->mArgument[ 0 ];
			if( lpImage )
			{
				apScript->mpPicture = lpImage->mpPicture;
			}
			else
			{
				apScript->mpPicture = 0;
			}
			break;

		case	eCMD_SAMPLE:
			lpSpl = (sAscotSpl*)lpCmd->mArgument[ 0 ];
			if( lpSpl )
			{
				if( lpSpl->mSound.mpSound && lpSpl->mSound.mLength )
				{
					AudioMixer_PlaySample( &lpSpl->mSound, 127 );
				}
			}
			break;

		case	eCMD_PIC_SETPAL:
			lpImage = (sImage*)lpCmd->mArgument[ 0 ];
			if( lpImage )
			{
				apScript->mpPalette      = lpImage->mpPicture;
				apScript->mPicPalSetFlag = 1;
			}
			else
			{
				apScript->mpPalette = 0;
			}
			break;

		case	eCMD_PIC_ON:
			apScript->mPicDrawFlag = 1;
			break;

		case	eCMD_PIC_OFF:
			apScript->mPicDrawFlag = 0;
			break;
		}

		apScript->mScriptIndex++;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Script_Init( sScriptControl * apScript )
* ACTION   : Script_Init
* CREATION : 05.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Script_Init( sScriptControl * apScript )
{
	U16 i;

	apScript->mMode              = eMODE_PROCESS;
	apScript->mFadeFrameCount    = 0;
	apScript->mFadeRGB           = 0;
	apScript->mFadeRGBFlag       = 0;
	apScript->mFrameRate         = 1;
	apScript->mMusicPat          = 0;
	apScript->mMusicPos          = 0;
	apScript->mPalSetFlag        = 0;
	apScript->mPicDrawFlag       = 0;
	apScript->mPicPalSetFlag     = 0;
	apScript->mPicPalFadeSetFlag = 0;
	apScript->mPause             = 0;
	apScript->mScreenMode        = 0;
	apScript->mScreenModeSetFlag = 0;
	apScript->mScriptIndex       = 0;

	apScript->mpFadePalette= 0;
	apScript->mpPalette    = 0;
	apScript->mpPicture    = 0;

	for( i=0; i<16; i++ )
	{
		apScript->mPal[ i ] = 0xFFF;
	}

	for( i=0; i<dLAYERS_LIMIT; i++ )
	{
		Layer_Init( &apScript->mLayers[ i ] );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ScriptManager_Init( void )
* ACTION   : ScriptManager_Init
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ScriptManager_Init( void )
{
	gScriptAsset.OnLoad = Script_OnLoad;
	gScriptAsset.OnUnLoad = Script_OnUnLoad;
	AssetClient_Init( &gScriptAsset, "ANIM.SPT", "CONTEXT1",  0 );
	ScriptReel_Init( &gScriptReel );
	Script_Init( &gScriptControl );
	gScriptControl.mpReel = &gScriptReel;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ScriptManager_DeInit( void )
* ACTION   : ScriptManager_DeInit
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ScriptManager_DeInit( void )
{
	AssetClient_DeInit( &gScriptAsset );
	ScriptReel_DeInit( &gScriptReel );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ScriptManager_Update( void )
* ACTION   : ScriptManager_Update
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ScriptManager_Update( void )
{
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Script_OnLoad( void * apData,const U32 aSize, const U32 aUserData )
* ACTION   : Script_OnLoad
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Script_OnLoad( void * apData,const U32 aSize, const U32 aUserData )
{
	(void)apData;
	(void)aSize;
	(void)aUserData;

	ScriptReel_Init( &gScriptReel );
	ScriptReel_Parse( &gScriptReel, (const char*)apData, aSize );
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Script_OnUnLoad( void * apData,const U32 aSize, const U32 aUserData )
* ACTION   : Script_OnUnLoad
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Script_OnUnLoad( void * apData,const U32 aSize, const U32 aUserData )
{
	(void)apData;
	(void)aSize;
	(void)aUserData;

	ScriptReel_DeInit( &gScriptReel );
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ScriptManager_GetpScript( void )
* ACTION   : ScriptManager_GetpScript
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

sScriptControl *	ScriptManager_GetpScript( void )
{
	return( &gScriptControl );	
}


/* ################################################################################ */
