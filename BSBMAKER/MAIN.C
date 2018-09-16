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

#include	<STDIO.H>
#include	<STRING.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\FONT\FONT.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\SPRITE\SPRITE.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PICTYPES\DEGAS.H>

#include	<MATH.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

#define	dFILESTRLEN	1024

enum
{
	eBSB_SPRTOK_NULL,
	eBSB_SPRTOK_BRACKETCLOSE,
	eBSB_SPRTOK_BRACKETOPEN,
	eBSB_SPRTOK_CANVASWIDTH,
	eBSB_SPRTOK_CHARFIRST,
	eBSB_SPRTOK_CHARFIRSTVAL,
	eBSB_SPRTOK_CHARLAST,
	eBSB_SPRTOK_CHARLASTVAL,
	eBSB_SPRTOK_CHARMAP,
	eBSB_SPRTOK_DST,		
	eBSB_SPRTOK_FRAMECOUNT,	
	eBSB_SPRTOK_HEIGHT,		
	eBSB_SPRTOK_OPAQUE,		
	eBSB_SPRTOK_MSK,		
	eBSB_SPRTOK_PLANES,		
	eBSB_SPRTOK_ROT90,	
	eBSB_SPRTOK_ROTENDANG,	
	eBSB_SPRTOK_ROTFRMCOUNT,
	eBSB_SPRTOK_ROTPIVOTX,	
	eBSB_SPRTOK_ROTPIVOTY,	
	eBSB_SPRTOK_ROTSTARTANG,
	eBSB_SPRTOK_SPACEWIDTH,
	eBSB_SPRTOK_SRC,	
	eBSB_SPRTOK_STEPX,		
	eBSB_SPRTOK_STEPY,		
	eBSB_SPRTOK_WIDTH,		
	eBSB_SPRTOK_X,			
	eBSB_SPRTOK_Y,			

	eBSB_SPRTOK_LIMIT
};

enum
{
	eBSB_FILETOK_NULL,
	eBSB_FILETOK_SPRITE,
	eBSB_FILETOK_HEADER,
	eBSB_FILETOK_FONT,

	eBSB_FILETOK_LIMIT
};

enum
{
	eBSB_FILECHUNK_NONE,
	eBSB_FILECHUNK_SPRITE,
	eBSB_FILECHUNK_HEADER,

	eBSB_FILECHUNK_LIMIT
};

enum
{
	eBSB_HEADER_GFXPATH,
	eBSB_HEADER_MSKPATH,
	eBSB_HEADER_DSTPATH,
	eBSB_HEADER_BRACKETOPEN,
	eBSB_HEADER_BRACKETCLOSE,

	eBSB_HEADER_LIMIT
};

sTagString	gBsbHeaderTokens[ eBSB_HEADER_LIMIT ] =
{
	{	eBSB_HEADER_GFXPATH,	"gfxpath"	},
	{	eBSB_HEADER_MSKPATH,	"mskpath"	},
	{	eBSB_HEADER_DSTPATH,	"dstpath"	},
	{	eBSB_HEADER_BRACKETOPEN,	"{"		},
	{	eBSB_HEADER_BRACKETCLOSE,	"}"		},
};

sTagString	gBsbFileChunkTokens[ eBSB_FILETOK_LIMIT ] =
{
	{	eBSB_FILETOK_NULL,		"NULL"			},
	{	eBSB_FILETOK_SPRITE,	"[ SPRITE ]"	},
	{	eBSB_FILETOK_HEADER,	"[ HEADER ]"	},
	{	eBSB_FILETOK_FONT,		"[ FONT ]"		},
};

sTagString	gBsbSprTokens[ eBSB_SPRTOK_LIMIT ] =
{
	{	eBSB_SPRTOK_NULL,			"NULL"			},
	{	eBSB_SPRTOK_BRACKETCLOSE,	"}"				},
	{	eBSB_SPRTOK_BRACKETOPEN,	"{"				},
	{	eBSB_SPRTOK_CANVASWIDTH,	"canvaswidth"	},
	{	eBSB_SPRTOK_CHARFIRST,		"charfirst"		},
	{	eBSB_SPRTOK_CHARFIRSTVAL,	"charfirstvalue"},
	{	eBSB_SPRTOK_CHARLAST,		"charlast"		},
	{	eBSB_SPRTOK_CHARLASTVAL,	"charlastvalue"	},
	{	eBSB_SPRTOK_CHARMAP,		"charmap"		},
	{	eBSB_SPRTOK_DST,			"dst"			},
	{	eBSB_SPRTOK_FRAMECOUNT,		"framecount"	},
	{	eBSB_SPRTOK_HEIGHT,			"height"		},
	{	eBSB_SPRTOK_OPAQUE,			"opaque"		},
	{	eBSB_SPRTOK_MSK,			"msk"			},
	{	eBSB_SPRTOK_PLANES,			"planes"		},
	{	eBSB_SPRTOK_ROT90,			"rot90"			},
	{	eBSB_SPRTOK_ROTENDANG,		"rotendangle"	},
	{	eBSB_SPRTOK_ROTFRMCOUNT,	"rotframecount" },
	{	eBSB_SPRTOK_ROTPIVOTX,		"rotpivotx"		},
	{	eBSB_SPRTOK_ROTPIVOTY,		"rotpivoty"		},
	{	eBSB_SPRTOK_ROTSTARTANG,	"rotstartangle"	},
	{	eBSB_SPRTOK_SPACEWIDTH,		"spacewidth"	},
	{	eBSB_SPRTOK_SRC,			"src"			},
	{	eBSB_SPRTOK_STEPX,			"stepx"			},
	{	eBSB_SPRTOK_STEPY,			"stepy"			},
	{	eBSB_SPRTOK_WIDTH,			"width"			},
	{	eBSB_SPRTOK_X,				"x"				},
	{	eBSB_SPRTOK_Y,				"y"				},
};

/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct
{
	char		SrcGfxFileName[ dFILESTRLEN ];
	char		SrcMskFileName[ dFILESTRLEN ];
	char		DstFileName[ dFILESTRLEN ];
	sDegas *	pGfx;
	U16			mFontFlag;
	U8			mCharFirst;
	U8			mCharLast;
	U16			mSpaceWidth;
	U16			StartLine;
	U16			X;
	U16			Y;
	U16			StepX;
	U16			StepY;
	U16			Width;
	U16			Height;
	U16			Planes;
	U16			CanvasWidth;
	U16			FrameCount;
	U16			OpaqueFlag;
	U16			Rot90Flag;
	S16			RotMinX;
	S16			RotMinY;
	S16			RotMaxX;
	S16			RotMaxY;
	U16			RotPivotX;
	U16			RotPivotY;
	U16			RotWidth;
	U16			RotHeight;
	U16			RotFrameCount;
	FP32		RotStartAngle;
	FP32		RotEndAngle;
} sBsbSprite;


/* ###################################################################################
#  DATA
################################################################################### */

char *		gpScriptFile;
S32			gScriptFileSize;
S32			gScriptFileIndex;
sBsbSprite	gBsbSprite;
U16			gLine;

U8		gBsfCharMap[ 256 ];
char	gBsbGfxPath[ 512 ];
char	gBsbMskPath[ 512 ];
char	gBsbDstPath[ 512 ];
char	gBsbScriptFile[ 512 ];

/* ###################################################################################
#  PROTOTYPES
################################################################################### */

S16		BsbMaker_ParseFile( char * apFileName );
void	BsbMaker_CalcRotBounds( sBsbSprite * apSprite );
void	BsbMaker_BitplaneToChunky( sBsbSprite * apSprite, U16 * apPlane, U16 * apChunky );
void	BsbMaker_ChunkyToBitPlane( sBsbSprite * apSprite, U16 * apChunky, U16 * apPlane );
void	BsbMaker_BuildRotFrame( sBsbSprite * apSprite, U16 aFrameIndex, U16 * apSrc, U16 * apDst );
void	BsbMaker_ClearChunkyBuffer( sBsbSprite * apSprite, U16 * apDst );
void	BsbMaker_FormatPathString( char * apString );
void	BsbMaker_CharMapInit( void );
void	BsbMaker_CharMapBuild( void );
void	BsbMaker_CharMapParse( const char * apString );


/* ###################################################################################
#  FUNCTIONS
################################################################################### */


/*-----------------------------------------------------------------------------------*
* FUNCTION : main( S16 argc, char * argv[] )
* ACTION   : entry point
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

S16	main( S16 argc, char * argv[] )
{

	if( argc < 2 )
	{
		printf( "Usage : BSBMAKER <Scriptfile>\n" );
		strcpy( gBsbScriptFile, "makegfx.scr" );
		GemDos_Cnecin();
	}
	else
	{
		strcpy( gBsbScriptFile, argv[1] );
	}

	if( !BsbMaker_ParseFile( gBsbScriptFile ) )
	{
		GemDos_Cnecin();
	}

	return( 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_SpriteInit( sBsbSprite * apSprite )
* ACTION   : inits sprite definition data
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_SpriteInit( sBsbSprite * apSprite )
{
	apSprite->pGfx        = 0;
	apSprite->X           = 0;
	apSprite->Y           = 0;
	apSprite->StepX       = 0;
	apSprite->StepY       = 0;
	apSprite->Width       = 0;
	apSprite->Height      = 0;
	apSprite->Planes      = 4;
	apSprite->CanvasWidth = 320;
	apSprite->FrameCount  = 1;
	apSprite->OpaqueFlag  = 0;
	apSprite->Rot90Flag   = 0;
	apSprite->RotFrameCount = 0;
	apSprite->mFontFlag = 0;
	apSprite->mCharFirst = 0;
	apSprite->mCharLast = 0;
	apSprite->mSpaceWidth = 4;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_SpritePrint( sBsbSprite * apSprite )
* ACTION   : BsbMaker_SpritePrint
* CREATION : 19.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_SpritePrint( sBsbSprite * apSprite )
{
	printf( "script line    : %d\n ", apSprite->StartLine );
	printf( "CanvasWidth    : %d\n ", apSprite->CanvasWidth );
	printf( "DstFileName    : %s\n ", &apSprite->DstFileName[0] );
	printf( "FrameCount     : %d\n ", apSprite->FrameCount );
	printf( "Height         : %d\n ", apSprite->Height );
	printf( "Opaque         : %d\n ", apSprite->OpaqueFlag );
	printf( "Planes         : %d\n ", apSprite->Planes );
	printf( "RotEndAngle    : %f\n ", apSprite->RotEndAngle );
	printf( "RotFrameCount  : %d\n ", apSprite->RotFrameCount );
	printf( "RotPivotX      : %d\n ", apSprite->RotPivotX );
	printf( "RotPivotY      : %d\n ", apSprite->RotPivotY );
	printf( "RotStartAngle  : %f\n ", apSprite->RotStartAngle );
	printf( "SrcFileName    : %s\n ", &apSprite->SrcGfxFileName[0] );
	printf( "SrcMskFileName : %s\n ", &apSprite->SrcMskFileName[0] );
	printf( "StepX          : %d\n ", apSprite->StepX );
	printf( "StepY          : %d\n ", apSprite->StepY );
	printf( "Width          : %d\n ", apSprite->Width );
	printf( "X              : %d\n ", apSprite->X );
	printf( "Y              : %d\n ", apSprite->Y );

}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_GrabFirstWord( char * apSrcString, char * apDstString )
* ACTION   : grabs variable name on left of line
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_GrabFirstWord( char * apSrcString, char * apDstString )
{
	char * lpString;

	lpString = apSrcString;
	while( (*lpString) && (*lpString>' ') && (*lpString!='=') )
	{
		*apDstString++ = *lpString++;
	}
	*apDstString = 0;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_GrabAssignedValue( char * apString )
* ACTION   : grabs assigned value on right of equals
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

U16		BsbMaker_GrabAssignedValue( char * apString )
{
	char * lpString;
	U16		lVal;

	lpString = apString;
	lVal     = 0;
	while( (*lpString) && (*lpString!='=') )
	{
		lpString++;
	}
	if( 0 == *lpString )
	{
		return( 0 );
	}

	while( (*lpString) && ((*lpString<'0')||(*lpString>'9')) )
	{
		lpString++;
	}
	while( (*lpString) && (*lpString>='0') && (*lpString<='9'))
	{
		lVal *= 10;
		lVal += (*lpString)-'0';
		lpString++;
	}
	return( lVal );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_GrabAssignedValue( char * apString )
* ACTION   : grabs assigned value on right of equals
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

FP32		BsbMaker_GrabAssignedFloat( char * apString )
{
	char *	lpString;
	U8		lNegFlag;
	FP32	lVal;
	FP32	lValDiv;

	lpString = apString;
	lVal     = 0;
	while( (*lpString) && (*lpString!='=') )
	{
		lpString++;
	}
	if( 0 == *lpString )
	{
		return( 0 );
	}

	lNegFlag = 0;
	while( (*lpString) && ((*lpString<'0')||(*lpString>'9')) )
	{
		if( *lpString == '-' )
		{
			lNegFlag = 1;
		}
		lpString++;
	}
	while( (*lpString) && (*lpString>='0') && (*lpString<='9'))
	{
		lVal *= 10.f;
		lVal += (*lpString)-'0';
		lpString++;
	}
	if( *lpString == '.' )
	{
		lValDiv = 10.f;
		lpString++;
		while( (*lpString) && (*lpString>='0') && (*lpString<='9'))
		{
			lVal     += (FP32)(*lpString-'0') / lValDiv;
			lValDiv *= 10.f;
			lpString++;
		}
	}
	if( lNegFlag )
	{
		lVal = 0.f - lVal;
	}
	return( lVal );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_GrabAssignedString( char * apSrcString, char * apDstString )
* ACTION   : grabs assigned string on right of equals
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_GrabAssignedString( char * apSrcString, char * apDstString )
{
	char * lpString;
	char * lpDstString;

	lpString = apSrcString;
	lpDstString = apDstString;
	*lpDstString = 0;

	while( (*lpString) && (*lpString!='=') )
	{
		lpString++;
	}
	if( 0 == *lpString )
	{
		return;
	}
	lpString++;
	while( (*lpString) && (*lpString<=' ') )
	{
		lpString++;
	}
	while( (*lpString) && (*lpString>' '))
	{
		*lpDstString++ = *lpString++;
	}
	*lpDstString = 0;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_GrabAssignedString( char * apSrcString, char * apDstString )
* ACTION   : grabs assigned string on right of equals
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

char	BsbMaker_GrabAssignedChar( char * apSrcString )
{
	char	lChar;
	U16		lLen;
	char	lString[ 256 ];

	BsbMaker_GrabAssignedString( apSrcString, lString );
	lLen = (U16)strlen( lString );
	if( lLen == 1 )
	{
		lChar = lString[ 0 ];
	}
	else if( lLen > 1 )
	{
		if( (lString[0]=='\'') || (lString[0]=='"') )
		{
			lChar = lString[ 1 ];
		}
		else
		{
			lChar = (char)BsbMaker_GrabAssignedValue( apSrcString );
		}
	}
	else
	{
		while( (*apSrcString) && (*apSrcString!='=') )
		{
			apSrcString++;
		}
		if( 0 == *apSrcString )
		{
			lChar = 0;
		}
		else
		{
			apSrcString++;

			lChar = *apSrcString;
			while( (*apSrcString) && (lChar == *apSrcString) )
			{
				apSrcString++;
			}
			if( *apSrcString )
			{
				lChar = *apSrcString;
			}
		}
	}
	return( lChar );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_SpriteWarning( sBsbSprite * apSprite, char * apString )
* ACTION   : displays a warning related to a sprite
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_SpriteWarning( sBsbSprite * apSprite, char * apString )
{
	printf( "*** WARNING : SpriteLine %d src %s dst %s\n%s\n", apSprite->StartLine, &apSprite->SrcGfxFileName[0], &apSprite->DstFileName[0], apString );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_ScriptGrabString( char * lpString )
* ACTION   : grabs a line of script file
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

U8	BsbMaker_ScriptGrabString( char * lpString )
{
	char		lChar;

	*lpString = 0;

	if( gScriptFileIndex >= gScriptFileSize )
	{
		return( 0 );
	}

	lChar = gpScriptFile[ gScriptFileIndex ];
	while( (gScriptFileIndex < gScriptFileSize)
			&& ( (lChar==0) || (lChar==10) || (lChar==13) ) 
		)
	{
		gScriptFileIndex++;
		lChar = gpScriptFile[ gScriptFileIndex ];
	}

	if( gScriptFileIndex >= gScriptFileSize )
	{
		return( 0 );
	}

	while( (gScriptFileIndex < gScriptFileSize)
			&& !( (lChar==0) || (lChar==10) || (lChar==13) ) 
		)
	{
		*lpString++ = lChar;
		gScriptFileIndex++;
		lChar = gpScriptFile[ gScriptFileIndex ];
	}
	*lpString = 0;
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_SpritesBuild( sBsbSprite * apSprite )
* ACTION   : builds all frames of sprite
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

U8	BsbMaker_SpritesBuild( sBsbSprite * apSprite )
{
	char			lString[ 1024 ];
	char			lFileName[ 1024 ];
	sSpriteBlock *	lpBlock0;
	sSpriteBlock *	lpBlock1;
	sFont *			lpFont;
	sDegas *		lpSrcGfx;
	sDegas *		lpSrcMsk;
	U32				lOffset;
	U32				lSize;
	U16 *			lpPlaneGfx;
	U16 *			lpPlaneMsk;
	U16 *			lpChunkyGfx;
	U16 *			lpChunkyMsk;
	U16 *			lpChunkyDst;
	U16				x,y;
	U16				i;
	U16				lMskPlaneCount;

	sprintf( lFileName,"%s%s", gBsbGfxPath, apSprite->SrcGfxFileName ); 
/*	printf( "%s\n", lFileName );*/
	lpSrcGfx = File_Load( lFileName );
	if( !lpSrcGfx )
	{
		sprintf( lString, "File Not Found %s\n", lFileName );
		BsbMaker_SpriteWarning( apSprite, lString );
		return( 0 );
	}

	if( 0 == apSprite->OpaqueFlag )
	{
		sprintf( lFileName,"%s%s", gBsbMskPath, apSprite->SrcMskFileName ); 
/*		printf( "%s\n", lFileName );*/
		lpSrcMsk = File_Load( lFileName );
		if( !lpSrcMsk )
		{
			sprintf( lString, "File Not Found %s\n", lFileName );
			BsbMaker_SpriteWarning( apSprite, lString );
			return( 0 );
		}
	}
	else
	{
		lpSrcMsk = lpSrcGfx;
	}


	if( apSprite->OpaqueFlag )
	{
		lMskPlaneCount = 0;
	}
	else
	{
		lMskPlaneCount = apSprite->Planes;
	}


	x = apSprite->X;
	y = apSprite->Y;

	if( apSprite->RotFrameCount )
	{
		lpBlock0 = Sprite_BlockCreate( apSprite->RotFrameCount );

		lOffset  = (x>>4)<<2;
		lOffset += (y*80L);

		BsbMaker_CalcRotBounds( apSprite );

		lpChunkyDst = mMEMCALLOC( 64000L );
		lpChunkyGfx = mMEMCALLOC( 32000L );
		lpChunkyMsk = mMEMCALLOC( 32000L );
		lpPlaneGfx  = mMEMCALLOC( 64000L );
		lpPlaneMsk  = mMEMCALLOC( 64000L );

		BsbMaker_BitplaneToChunky( apSprite, &lpSrcGfx->mPixels[ lOffset ], lpChunkyGfx );
		BsbMaker_BitplaneToChunky( apSprite, &lpSrcMsk->mPixels[ lOffset ], lpChunkyMsk );

		for( i=0; i<apSprite->RotFrameCount; i++ )
		{
			BsbMaker_ClearChunkyBuffer( apSprite, lpChunkyDst );
			BsbMaker_BuildRotFrame( apSprite, i, lpChunkyGfx, lpChunkyDst );
			BsbMaker_ChunkyToBitPlane( apSprite, lpChunkyDst, lpPlaneGfx );

			BsbMaker_ClearChunkyBuffer( apSprite, lpChunkyDst );
			BsbMaker_BuildRotFrame( apSprite, i, lpChunkyMsk, lpChunkyDst );
			BsbMaker_ChunkyToBitPlane( apSprite, lpChunkyDst, lpPlaneMsk );

			lpBlock0->mpSprite[ i ] = Sprite_Create(
				lpPlaneGfx, 
				lpPlaneMsk, 
				(apSprite->RotWidth+15)&0xFFF0, 
				apSprite->RotHeight, 
				apSprite->Planes, 
				lMskPlaneCount,
				(U8)apSprite->OpaqueFlag );
		}

		mMEMFREE( lpPlaneGfx  );
		mMEMFREE( lpPlaneMsk  );
		mMEMFREE( lpChunkyGfx );
		mMEMFREE( lpChunkyMsk );
		mMEMFREE( lpChunkyDst );
	}
	else
	{
		lpBlock0 = Sprite_BlockCreate( apSprite->FrameCount );


		for( i=0; i<apSprite->FrameCount; i++ )
		{
			lOffset  = (x>>4)<<2;
			lOffset += (y*80L);

			lpBlock0->mpSprite[ i ] = Sprite_Create( &lpSrcGfx->mPixels[ lOffset ], &lpSrcMsk->mPixels[ lOffset ], apSprite->Width, apSprite->Height, apSprite->Planes, lMskPlaneCount, (U8)apSprite->OpaqueFlag );

			x += apSprite->StepX;

			if( (x+apSprite->Width) > (apSprite->X+apSprite->CanvasWidth) )
			{
				x = apSprite->X;
				y += apSprite->StepY;
			}
		}
	}

	if( gBsbSprite.Rot90Flag )
	{
		lpBlock1 = SpriteBlock_Rot90( lpBlock0 );
	}
	else
	{
		lpBlock1 = Sprite_BlockSerialise( lpBlock0 );
	}
	lSize    = Sprite_BlockGetSize( lpBlock1 );

	if( gBsbSprite.mFontFlag )
	{		
		lpFont = Font_Create( lpBlock1, &gBsfCharMap[0], gBsbSprite.Width );
		lpFont->mSpaceWidth = (U8)gBsbSprite.mSpaceWidth;
		lSize = Font_GetSize( lpFont );
		Font_Delocate( lpFont );
		sprintf( lFileName, "%s%s", gBsbDstPath, apSprite->DstFileName );
		File_Save( lFileName, lpFont, lSize );
		printf( "%s\n", lFileName );
		mMEMFREE( lpFont );
	}
	else
	{
		Sprite_BlockDelocate( lpBlock1 );
		sprintf( lFileName, "%s%s", gBsbDstPath, apSprite->DstFileName );
		File_Save( lFileName, lpBlock1, lSize );
		printf( "%s\n", lFileName );
	}

	mMEMFREE( lpBlock1 );
	Sprite_BlockDestroy( lpBlock0 );

	if( lpSrcMsk != lpSrcGfx )
		File_UnLoad( lpSrcMsk );
	File_UnLoad( lpSrcGfx );

	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_SpriteOutput( sBsbSprite * apSprite )
* ACTION   : creates sprite
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_SpriteOutput( sBsbSprite * apSprite )
{
	char	lString[ 128 ];

	if( 0 == strlen( &apSprite->SrcGfxFileName[0] ) )
	{
		BsbMaker_SpriteWarning( apSprite, "no source gfx filename specified" );
		return;
	}
	if( 0 == strlen( &apSprite->DstFileName[0] ) )
	{
		BsbMaker_SpriteWarning( apSprite, "no dest sprite filename specified" );
		return;
	}
	if( 0 == strlen( &apSprite->SrcMskFileName[0] ) )
	{
		if( 0==apSprite->OpaqueFlag )
		{
			BsbMaker_SpriteWarning( apSprite, "no mask filename specified for non-opaque sprite" );
			return;
		}
	}
	if( 0 == apSprite->Width )
	{
		BsbMaker_SpriteWarning( apSprite, "no width specified" );
		return;
	}
	if( 0 == apSprite->Height )
	{
		BsbMaker_SpriteWarning( apSprite, "no width specified" );
		return;
	}
	if( apSprite->X > 320 )
	{
		sprintf( lString, "x out of range %d", apSprite->X );
		BsbMaker_SpriteWarning( apSprite, lString );
		return;
	}
	if( apSprite->Y > 200 )
	{
		sprintf( lString, "y out of range %d", apSprite->Y );
		BsbMaker_SpriteWarning( apSprite, lString );
		return;
	}
	if( apSprite->CanvasWidth > 320 )
	{
		sprintf( lString, "canvas width too big %d", apSprite->CanvasWidth );
		BsbMaker_SpriteWarning( apSprite, lString );
		return;
	}
	if( 0 == apSprite->StepX )
	{
		apSprite->StepX = apSprite->Width;
	}
	if( 0 == apSprite->StepY )
	{
		apSprite->StepY = apSprite->Height;
	}
	if( (0==apSprite->Planes) || (apSprite->Planes>4) )
	{
		sprintf( lString, "planes out of range %d", apSprite->Planes );
		BsbMaker_SpriteWarning( apSprite, lString );
	}
/*	BsbMaker_SpritePrint( apSprite );*/
	
	BsbMaker_SpritesBuild( apSprite );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_ParseFile( char * apFileName )
* ACTION   : parses script file
* CREATION : 12.11.01 PNK
*-----------------------------------------------------------------------------------*/

S16	BsbMaker_ParseFile( char * apFileName )
{
	char	lString[ 1024 ];
	char	lWord[ 1024 ];
	U8		lFileChunk;

#if	0
	strcpy( gBsbGfxPath, "C:\\Program Files\\WinSTon\\Hard_Drv\\SOURCE\\RG\\CLOG_UP\\GFX\\" );
	strcpy( gBsbMskPath, "C:\\Program Files\\WinSTon\\Hard_Drv\\SOURCE\\RG\\CLOG_UP\\GFX\\" );
	strcpy( gBsbDstPath, "C:\\Program Files\\WinSTon\\Hard_Drv\\SOURCE\\RG\\CLOG_UP\\GFX\\" );
#else
	gBsbGfxPath[ 0 ] = 0;
	gBsbMskPath[ 0 ] = 0;
	gBsbDstPath[ 0 ] = 0;
#endif

	if( 0 == File_Exists( apFileName ) )
	{
		printf( "ERROR : File Not Found : %s\n", apFileName );
		return( 0 );
	}

	gScriptFileSize = File_GetSize( apFileName );

	if( 0 == gScriptFileSize )
	{
		printf( "ERROR : File Size 0 : %s\n", apFileName );
		return( 0 );
	}

	gpScriptFile = File_Load( apFileName );

	if( 0 == gpScriptFile )
	{
		printf( "ERROR : Couldn't Load : %s\n", apFileName );
		return( 0 );
	}

	BsbMaker_SpriteInit( &gBsbSprite );
	lFileChunk = eBSB_FILECHUNK_NONE;
	gLine =0;

	while( BsbMaker_ScriptGrabString( &lString[0] ) )
	{
/*		printf( "%s\n", &lString[0] );*/
		if( strlen( lString ) )
		{
			switch( lFileChunk )
			{
			case	eBSB_FILECHUNK_NONE:
				switch( (U16)sTagString_GetID( lString, gBsbFileChunkTokens, eBSB_FILETOK_LIMIT ) )
				{
				case	eBSB_FILETOK_SPRITE:
					BsbMaker_SpriteInit( &gBsbSprite );
					lFileChunk      = eBSB_FILECHUNK_SPRITE;
					gBsbSprite.StartLine = gLine;
					break;
				case	eBSB_FILETOK_FONT:
					BsbMaker_SpriteInit( &gBsbSprite );
					gBsbSprite.mFontFlag = 1;
					BsbMaker_CharMapInit();
					lFileChunk      = eBSB_FILECHUNK_SPRITE;
					gBsbSprite.StartLine = gLine;
					break;
				case	eBSB_FILETOK_HEADER:
					lFileChunk      = eBSB_FILECHUNK_HEADER;
					break;
				}
				break;


			case	eBSB_FILECHUNK_HEADER:
				BsbMaker_GrabFirstWord( &lString[0], &lWord[0] );
				switch( (U16)sTagString_GetID( lWord, gBsbHeaderTokens, eBSB_HEADER_LIMIT ) )
				{
				case	eBSB_HEADER_GFXPATH:
					BsbMaker_GrabAssignedString( lString, gBsbGfxPath );
					BsbMaker_FormatPathString( gBsbGfxPath );
					break;
				case	eBSB_HEADER_MSKPATH:
					BsbMaker_GrabAssignedString( lString, gBsbMskPath );
					BsbMaker_FormatPathString( gBsbMskPath );
					break;
				case	eBSB_HEADER_DSTPATH:
					BsbMaker_GrabAssignedString( lString, gBsbDstPath );
					BsbMaker_FormatPathString( gBsbDstPath );
					break;
				case	eBSB_HEADER_BRACKETCLOSE:
					lFileChunk = eBSB_FILECHUNK_NONE;
					break;
				}
				break;

			case	eBSB_FILECHUNK_SPRITE:
				BsbMaker_GrabFirstWord( &lString[0], &lWord[0] );

				switch( (U16)sTagString_GetID( lWord, gBsbSprTokens, eBSB_SPRTOK_LIMIT ) )
				{
				case eBSB_SPRTOK_CANVASWIDTH:
					gBsbSprite.CanvasWidth = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_CHARFIRST:
					gBsbSprite.mCharFirst = BsbMaker_GrabAssignedChar( lString );
					BsbMaker_CharMapBuild();
					break;
				case eBSB_SPRTOK_CHARFIRSTVAL:
					gBsbSprite.mCharFirst = (U8)BsbMaker_GrabAssignedValue( lString );
					BsbMaker_CharMapBuild();
					break;
				case eBSB_SPRTOK_CHARLAST:
					gBsbSprite.mCharLast = BsbMaker_GrabAssignedChar( lString );
					BsbMaker_CharMapBuild();
					break;
				case eBSB_SPRTOK_CHARLASTVAL:
					gBsbSprite.mCharLast = (U8)BsbMaker_GrabAssignedValue( lString );
					BsbMaker_CharMapBuild();
					break;
				case eBSB_SPRTOK_CHARMAP:
					BsbMaker_CharMapParse( lString );
					break;
				case eBSB_SPRTOK_DST:		
					BsbMaker_GrabAssignedString( lString, &gBsbSprite.DstFileName[0] );
					break;
				case eBSB_SPRTOK_FRAMECOUNT:	
					gBsbSprite.FrameCount = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_HEIGHT:		
					gBsbSprite.Height = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_OPAQUE:		
					gBsbSprite.OpaqueFlag = 1;
					break;
				case eBSB_SPRTOK_MSK:		
					BsbMaker_GrabAssignedString( lString, &gBsbSprite.SrcMskFileName[0] );
					break;
				case eBSB_SPRTOK_PLANES:		
					gBsbSprite.Planes = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_ROT90:		
					gBsbSprite.Rot90Flag = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_ROTENDANG:	
					gBsbSprite.RotEndAngle = BsbMaker_GrabAssignedFloat( lString );
					break;
				case eBSB_SPRTOK_ROTFRMCOUNT:
					gBsbSprite.RotFrameCount = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_ROTPIVOTX:	
					gBsbSprite.RotPivotX = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_ROTPIVOTY:	
					gBsbSprite.RotPivotY = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_ROTSTARTANG:
					gBsbSprite.RotStartAngle = BsbMaker_GrabAssignedFloat( lString );
					break;
				case eBSB_SPRTOK_SPACEWIDTH:
					gBsbSprite.mSpaceWidth = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_SRC:		
					BsbMaker_GrabAssignedString( lString, &gBsbSprite.SrcGfxFileName[0] );
					break;
				case eBSB_SPRTOK_STEPX:		
					gBsbSprite.StepX = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_STEPY:		
					gBsbSprite.StepY = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_WIDTH:		
					gBsbSprite.Width = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_X:			
					gBsbSprite.X = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_Y:			
					gBsbSprite.Y = BsbMaker_GrabAssignedValue( lString );
					break;
				case eBSB_SPRTOK_BRACKETOPEN:	
					break;
				case eBSB_SPRTOK_BRACKETCLOSE:
					if( eBSB_FILECHUNK_SPRITE == lFileChunk )
					{
/*						BsbMaker_SpritePrint( &gBsbSprite );*/
						BsbMaker_SpriteOutput( &gBsbSprite );
						BsbMaker_SpriteInit( &gBsbSprite );
						lFileChunk = eBSB_FILECHUNK_NONE;
					}
					break;

				}
			}
		}
		gLine++;
	}

	if( eBSB_FILECHUNK_SPRITE == lFileChunk )
	{
/*		BsbMaker_SpritePrint( &gBsbSprite );*/
		BsbMaker_SpriteOutput( &gBsbSprite );
		lFileChunk = 0;
	}

	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_ChunkyToBitPlane( sBsbSprite * apSprite,U16 * apChunky,U16 * apPlane )
* ACTION   : BsbMaker_ChunkyToBitPlane
* CREATION : 19.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_ChunkyToBitPlane( sBsbSprite * apSprite,U16 * apChunky,U16 * apPlane )
{
	U16	i,j;
	U16	lMask;
	U16	lCol;
	U16 *	lpPlane;


	for( i=0; i<apSprite->RotHeight; i++ )
	{
		lMask = 0x8000;
		lpPlane = apPlane;

		lpPlane[ 0 ] = 0;
		lpPlane[ 1 ] = 0;
		lpPlane[ 2 ] = 0;
		lpPlane[ 3 ] = 0;

		for( j=0; j<apSprite->RotWidth; j++ )
		{
			lCol = *apChunky++;

			if( lCol & 1 )
			{
				lpPlane[0] |= lMask;
			}
			if( lCol & 2 )
			{
				lpPlane[1] |= lMask;
			}
			if( lCol & 4 )
			{
				lpPlane[2] |= lMask;
			}
			if( lCol & 8 )
			{
				lpPlane[3] |= lMask;
			}


			lMask>>=1;
			if( !lMask )
			{
				lMask   = 0x8000;
				lpPlane += 4;
				lpPlane[ 0 ] = 0;
				lpPlane[ 1 ] = 0;
				lpPlane[ 2 ] = 0;
				lpPlane[ 3 ] = 0;
			}
		}
		apPlane += 80;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_BitplaneToChunky( sBsbSprite * apSprite,U16 * apPlane,U16 * apChunky )
* ACTION   : BsbMaker_BitplaneToChunky
* CREATION : 19.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_BitplaneToChunky( sBsbSprite * apSprite,U16 * apPlane,U16 * apChunky )
{
	U16	i,j;
	U16	lMask;
	U16	lCol;
	U16 *	lpPlane;

	for( i=0; i<apSprite->Height; i++ )
	{
		lMask = 0x8000;
		lpPlane = apPlane;
		for( j=0; j<apSprite->Width; j++ )
		{
			lCol = 0;

			if( lpPlane[0] & lMask )
			{
				lCol += 1;
			}
			if( lpPlane[1] & lMask )
			{
				lCol += 2;
			}
			if( lpPlane[2] & lMask )
			{
				lCol += 4;
			}
			if( lpPlane[3] & lMask )
			{
				lCol += 8;
			}

			apChunky[ (apSprite->RotWidth*i)+j ] = lCol;

			lMask>>=1;
			if( !lMask )
			{
				lMask   = 0x8000;
				lpPlane += 4;
			}
		}
		apPlane += 80;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_BuildRotFrame( sBsbSprite * apSprite,U16 aFrameIndex,U16 * apSrc,U16 * apDst )
* ACTION   : BsbMaker_BuildRotFrame
* CREATION : 19.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_BuildRotFrame( sBsbSprite * apSprite,U16 aFrameIndex,U16 * apSrc,U16 * apDst )
{
	FP32	lX,lY;
	FP32	lRX,lRY;
	FP32	lSin,lCos;
	FP32	lCentreX,lCentreY;
	FP32	lAngle;
	S16		lBX,lBY;
	U16		i,j;
	U16 *	lpBuffer;
	
	lpBuffer = apDst;
	for( i=0; i<apSprite->RotHeight; i++ )
	{
		for( j=0; j<apSprite->RotWidth; j++ )
		{
			*lpBuffer++ = 0;
		}
	}

	lAngle  = (FP32)aFrameIndex;
	if( apSprite->RotFrameCount )
	{
		lAngle /= apSprite->RotFrameCount;
	}
	lAngle *= (apSprite->RotEndAngle-apSprite->RotStartAngle);
	lAngle += apSprite->RotStartAngle;

	printf( "lAngle %f", lAngle );

	lSin = (FP32)sin( lAngle );
	lCos = (FP32)cos( lAngle );

	lCentreX = (FP32)apSprite->RotMinX;
	lCentreY = (FP32)apSprite->RotMinY;

	lY  = 0;
	lY -= apSprite->RotPivotY;
	for( i=0; i<apSprite->Height; i++ )
	{
		lX  = 0;
		lX -= apSprite->RotPivotX;
		for( j=0; j<apSprite->Width; j++ )
		{
			lRX  = (lCos*lX)-(lSin*lY);
			lRY  = (lSin*lX)+(lCos*lY);

			lRX -= lCentreX;
			lRY -= lCentreY;
			
			lBX = (S16)lRX;
			lBY = (S16)lRY;

			if( lBX < 0 )
			{
				printf( "clipped x<0 %f, %d\n", lRX, lBX);
				lBX = 0;
			}
			if( lBX >= (S16)apSprite->RotWidth )
			{
				printf( "clipped x>w %d\n", lBX );
				lBX = apSprite->RotWidth-1;
			}
			if( lBY < 0 )
			{
				printf( "clipped y<0 %d\n", lBY );
				lBY = 0;
			}
			if( lBY >= (S16)apSprite->RotHeight )
			{
				printf( "clipped y>h %d\n", lBY );
				lBY = apSprite->RotHeight-1;
			}


			apDst[ (lBY*apSprite->RotWidth)+lBX ] = apSrc[ (i*apSprite->RotWidth)+j ];


			lX++;
		}
		lY++;
	}

}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_ClearChunkyBuffer( sBsbSprite * apSprite,U16 * apDst )
* ACTION   : BsbMaker_ClearChunkyBuffer
* CREATION : 19.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_ClearChunkyBuffer( sBsbSprite * apSprite,U16 * apDst )
{
	U16	i,j;
	U16 *	lpDst;

	lpDst = apDst;

	for( i=0; i<apSprite->RotHeight; i++ )
	{
		for( j=0; j<apSprite->RotWidth; j++ )
		{
			*lpDst++ = 0;
		}
	}

}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_CalcRotBounds( sBsbSprite * apSprite )
* ACTION   : BsbMaker_CalcRotBounds
* CREATION : 19.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_CalcRotBounds( sBsbSprite * apSprite )
{
	U16		i,j;
	FP32	lRX,lRY;
	FP32	lMinX;
	FP32	lMaxX;
	FP32	lMinY;
	FP32	lMaxY;
	FP32	lX0,lY0,lX1,lY1;
	FP32	lVertsX[4];
	FP32	lVertsY[4];
	FP32	lSin,lCos;
	FP32	lAngle;
	FP32	lAngleStep;

	if( !apSprite->FrameCount )
	{
		return;
	}

	lMaxX = 0.f;
	lMaxY = 0.f;
	lMinX = 65536.f;
	lMinY = 65536.f;

	lX0 = 0;
	lY0 = 0;
	lX1 = (FP32)apSprite->Width;
	lY1 = (FP32)apSprite->Height;

	lX0 -= apSprite->RotPivotX;
	lX1 -= apSprite->RotPivotX;
	lY0 -= apSprite->RotPivotY;
	lY1 -= apSprite->RotPivotY;

/*
	lMinX = lX0;
	lMaxX = lX1;
	lMinY = lY0;
	lMaxY = lY1;
*/

	lVertsX[0] = lX0;
	lVertsX[1] = lX1;
	lVertsX[2] = lX0;
	lVertsX[3] = lX1;

	lVertsY[0] = lY0;
	lVertsY[1] = lY0;
	lVertsY[2] = lY1;
	lVertsY[3] = lY1;


	lAngle      = apSprite->RotStartAngle;
	lAngleStep  = apSprite->RotEndAngle-apSprite->RotStartAngle;
	lAngleStep /= apSprite->RotFrameCount;
	for( i=0; i<apSprite->RotFrameCount; i++ )
	{
		lCos = (FP32)cos( lAngle );
		lSin = (FP32)sin( lAngle );

		for( j=0; j<4; j++ )
		{
			lRX = (lCos*lVertsX[j])-(lSin*lVertsY[j]);
			lRY = (lSin*lVertsX[j])+(lCos*lVertsY[j]);

			if( lRX < lMinX )
			{
				lMinX = lRX;
			}
			if( lRX > lMaxX )
			{
				lMaxX = lRX;
			}
			if( lRY < lMinY )
			{
				lMinY = lRY;
			}
			if( lRY > lMaxY )
			{
				lMaxY = lRY;
			}
		
		}
		lAngle += lAngleStep;
	}

	apSprite->RotMinX = (S16)lMinX;
	apSprite->RotMaxX = (S16)lMaxX;
	apSprite->RotMinY = (S16)lMinY;
	apSprite->RotMaxY = (S16)lMaxY;

	apSprite->RotWidth  = (apSprite->RotMaxX-apSprite->RotMinX)+1;
	apSprite->RotHeight = (apSprite->RotMaxY-apSprite->RotMinY)+1;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_FormatPathString( char * apString )
* ACTION   : BsbMaker_FormatPathString
* CREATION : 19.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_FormatPathString( char * apString )
{
	S32	lIndex;
	
	lIndex = strlen( apString );
	if( lIndex )
	{
		if( (apString[ lIndex-1 ] != '\\') && (apString[ lIndex-1 ] != '/') )
		{
			apString[ lIndex-1 ] = '\\';
		}
	}
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_CharMapInit( void )
* ACTION   : BsbMaker_CharMapInit
* CREATION : 21.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_CharMapInit( void )
{
	U16	i;
	
	for( i=0; i<256; i++ )
	{
		gBsfCharMap[ i ] = 0xFF;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_CharMapBuild( void )
* ACTION   : BsbMaker_CharMapBuild
* CREATION : 21.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_CharMapBuild( void )
{
	U16	i,j;

	BsbMaker_CharMapInit();	

	j = gBsbSprite.mCharFirst;
	i = 0;
	while( (i<256) && (j<=gBsbSprite.mCharLast) )
	{
		gBsfCharMap[ j ] = (U8)i;
		j++;
		i++;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : BsbMaker_CharMapParser( const char * apString )
* ACTION   : BsbMaker_CharMapParser
* CREATION : 21.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	BsbMaker_CharMapParse( const char * apString )
{
	U16	i,j;


	while( (*apString) && (*apString!='=') )
	{
		apString++;
	}
	if( 0 == *apString )
	{
		return;
	}
	apString++;
	while( (*apString) && (*apString<=' ') )
	{
		apString++;
	}
	
	i = 0;
	while( (i<256) && (*apString) )
	{
		j = *apString++;
		gBsfCharMap[ j ] = (U8)i;
		i++;
	}
}



