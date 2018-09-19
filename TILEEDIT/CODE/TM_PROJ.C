/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"TILEMAP.H"

#include	"EDITMENU.H"
#include	"IMG_MENU.H"

#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\STRING\STRING.H>
#include	<GODLIB\TOKENISE\TOKENISE.H>


/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	eTMPROJ_TAG_IMAGE,
	eTMPROJ_TAG_MAP,

	eTMPROJ_TAG_LIMIT
};

/* ###################################################################################
#  STRUCTS
################################################################################### */



/* ###################################################################################
#  DATA
################################################################################### */

sTokeniserTagString	gTileMapProjTags[] =
{
	{	eTMPROJ_TAG_IMAGE,	"IMAGE"	},
	{	eTMPROJ_TAG_MAP,	"MAP"	},
	{	0,	0	}
};


/* ###################################################################################
#  PROTOTYPES
################################################################################### */


/* ###################################################################################
#  CODE
################################################################################### */


/*-----------------------------------------------------------------------------------*
* FUNCTION : void TileMapProject_Load( char * apFileName)
* ACTION   : TileMapProject_Load
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void TileMapProject_Load( char * apFileName)
{
	const sTokeniserTagString *	lpTag;
	sTokeniserArgs				lArgs;
	char *						lpText;
	S32							lSize,lOffset;

	DebugLog_Printf1( "TileMapProject_Load %s", apFileName );
	if( apFileName )
	{
		lSize = File_GetSize( apFileName );
		if( lSize > 0 )
		{
			lpText = File_Load( apFileName );
			if( lpText )
			{
				lOffset = 0;
				while( lOffset < lSize )
				{
					lOffset = TokeniserArgs_Build( &lArgs, lpText, lOffset, lSize );
					if( lArgs.mArgCount >= 2 )
					{
						DebugLog_Printf2( "TileMapProject_Load argcount %d |%s|", lArgs.mArgCount, lArgs.mpStrings[1] );
						if( !String_StrCmp( "=", lArgs.mpStrings[ 1 ] ) )
						{
							DebugLog_Printf0( "got =" );
							lpTag = Tokeniser_GetTagString( gTileMapProjTags, lArgs.mpStrings[ 0 ] );
							if( lpTag )
							{
								DebugLog_Printf1( "got tag %s", lpTag->mpString );
								switch( lpTag->mID )
								{
								case	eTMPROJ_TAG_IMAGE:
									if( lArgs.mArgCount >= 4 )
									{
										ImageMenu_ImageLoad( TokeniserArgs_GetU16( &lArgs ), lArgs.mpStrings[ 3 ] );
									}
									break;
								case	eTMPROJ_TAG_MAP:
									EditMenu_MapLoad( lArgs.mpStrings[ 2 ] );
									break;
								}
							}
						}
					}
				}
				File_UnLoad( lpText );
			}
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void TileMapProject_Save( char * apFileName)
* ACTION   : TileMapProject_Save
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void TileMapProject_Save( char * apFileName)
{
	U16		i;
	char	lString[ 512 ];
	FILE *	lpFile;

	if( apFileName )
	{
		lpFile = fopen( apFileName, "w" );
		if( lpFile )
		{
			sprintf( lString, "MAP = %s\n", EditMenu_GetpMapFileName() );
			fwrite( lString, String_StrLen(lString), 1, lpFile );

			for( i=0; i<dIMGMENU_IMG_LIMIT; i++ )
			{
				if( ImageMenu_GetpImage( i ) )
				{
					sprintf( lString, "IMAGE = %d %s\n", i, ImageMenu_GetpImageName( i ) );
					fwrite( lString, String_StrLen(lString), 1, lpFile );
				}
			}
			fclose( lpFile );
		}
	}
}
