/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"IMG_MENU.H"

#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GUI\GUI.H>
#include	<GODLIB\GUI\GUIFS.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PICTYPES\DEGAS.H>
#include	<GODLIB\STRING\STRING.H>
#include	<GODLIB\VIDEO\VIDEO.H>

/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	eIMGMENU_VAR_IMGINDEX,
	eIMGMENU_VAR_IMGINDEXMAX,

	eIMGMENU_VAR_LIMIT
};

enum
{
	eIMGMENU_VARCLIENT_IMGINDEX,
	eIMGMENU_VARCLIENT_IMGNEXT,
	eIMGMENU_VARCLIENT_IMGPREV,
	eIMGMENU_VARCLIENT_IMGLOAD,

	eIMGMENU_VARCLIENT_LIMIT
};


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sImageMenuAsset
{
	sString 	mFileName;
	sDegas *	mpDegas;
} sImageMenuAsset;

typedef	struct	sImageMenuClass
{
	sHashTree *			mpTree;
	sImageMenuAsset		mAssets[ dIMGMENU_IMG_LIMIT ];
	sHashTreeVar *		mpVars[ eIMGMENU_VAR_LIMIT ];
	sHashTreeVarClient	mVarClients[ eIMGMENU_VARCLIENT_LIMIT ];
	sGuiFSInfo			mFS;
	U32					mImageIndex;
	U32					mImageIndexMax;
} sImageMenuClass;


/* ###################################################################################
#  DATA
################################################################################### */

sImageMenuClass	gImgClass;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	ImageMenu_OnLoad( sHashTreeVar  * apVar );
void	ImageMenu_OnImageIndex( sHashTreeVarClient  * apClient );
void	ImageMenu_OnImageLoad( sHashTreeVarClient  * apClient );
void	ImageMenu_OnImageNext( sHashTreeVarClient  * apClient );
void	ImageMenu_OnImagePrev( sHashTreeVarClient  * apClient );

void	ImageMenu_OnLoadFS( sGuiFSInfo * apFS );

/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageMenu_Init( sHashTree * apTree )
* ACTION   : ImageMenu_Init
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	ImageMenu_Init( sHashTree * apTree )
{
	U16	i;

	Memory_Clear( sizeof(sImageMenuClass), &gImgClass );

	for( i=0; i<dIMGMENU_IMG_LIMIT; i++ )
	{
		String_Init( &gImgClass.mAssets[ i ].mFileName,  "TEST.PI1" );
	}

	GuiFSInfo_Init( &gImgClass.mFS, "LOAD IMAGE", "*.PI1", "TEST.PI1" );
	gImgClass.mFS.mpCB = ImageMenu_OnLoadFS;
	gImgClass.mpTree = apTree;

	gImgClass.mImageIndexMax = dIMGMENU_IMG_LIMIT;

	gImgClass.mpVars[ eIMGMENU_VAR_IMGINDEX    ] = HashTree_VarInit( apTree, "TILEEDIT\\IMAGEMENU\\IMAGEINDEX", sizeof(U32), &gImgClass.mImageIndex );
	gImgClass.mpVars[ eIMGMENU_VAR_IMGINDEXMAX ] = HashTree_VarInit( apTree, "TILEEDIT\\IMAGEMENU\\IMAGEINDEXMAX", sizeof(U32), &gImgClass.mImageIndexMax );

	HashTree_VarClient_Init( &gImgClass.mVarClients[ eIMGMENU_VARCLIENT_IMGINDEX ], apTree, "TILEEDIT\\IMAGEMENU\\IMAGEINDEX",	ImageMenu_OnImageIndex );
	HashTree_VarClient_Init( &gImgClass.mVarClients[ eIMGMENU_VARCLIENT_IMGNEXT  ], apTree, "GUI\\BUTTONS\\BUTT_IMG_NEXT",		ImageMenu_OnImageNext );
	HashTree_VarClient_Init( &gImgClass.mVarClients[ eIMGMENU_VARCLIENT_IMGPREV  ], apTree, "GUI\\BUTTONS\\BUTT_IMG_PREV",		ImageMenu_OnImagePrev );
	HashTree_VarClient_Init( &gImgClass.mVarClients[ eIMGMENU_VARCLIENT_IMGLOAD  ], apTree, "GUI\\BUTTONS\\BUTT_IMG_LOAD",		ImageMenu_OnImageLoad );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageMenu_DeInit( void )
* ACTION   : ImageMenu_DeInit
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	ImageMenu_DeInit( void )
{
	U16	i;

	for( i=0; i<eIMGMENU_VARCLIENT_LIMIT; i++ )
	{
		HashTree_VarClient_DeInit( &gImgClass.mVarClients[ i ], gImgClass.mpTree );
	}

	for( i=0; i<dIMGMENU_IMG_LIMIT; i++ )
	{
		String_DeInit( &gImgClass.mAssets[ i ].mFileName );
	}	
	GuiFSInfo_DeInit( &gImgClass.mFS );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageMenu_OnLoad( sHashTreeVar  * apVar )
* ACTION   : ImageMenu_OnLoad
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void	ImageMenu_OnLoad( sHashTreeVar  * apVar )
{
	(void)apVar;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void ImageMenu_OnImageIndex(sHashTreeVarClient  * apClient)
* ACTION   : ImageMenu_OnImageIndex
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void ImageMenu_OnImageIndex(sHashTreeVarClient  * apClient)
{
	gImgClass.mImageIndex = *(S32*)apClient->mpVar->mpData;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void ImageMenu_OnImageNext(sHashTreeVarClient  * apClient)
* ACTION   : ImageMenu_OnImageNext
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void ImageMenu_OnImageNext(sHashTreeVarClient  * apClient)
{
	(void)apClient;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void ImageMenu_OnImagePrev(sHashTreeVarClient  * apClient)
* ACTION   : ImageMenu_OnImagePrev
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void ImageMenu_OnImagePrev(sHashTreeVarClient  * apClient)
{
	(void)apClient;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void ImageMenu_OnImageLoad(sHashTreeVarClient  * apClient)
* ACTION   : ImageMenu_OnImageLoad
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void ImageMenu_OnImageLoad(sHashTreeVarClient  * apClient)
{
	sGuiEvent *	lpEvent;

	lpEvent = (sGuiEvent*)apClient->mpVar->mpData;

	if( lpEvent )
	{
		if( eGUIEVENT_BUTTON_LEFTCLICK == lpEvent->mEvent )
		{
			GuiButton_DeSelect( (sGuiButton*)lpEvent->mpInfo );

			GuiFS_Open( &gImgClass.mFS );
		}
	}		
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void ImageMenu_OnLoadFS(sGuiFSInfo * apFS)
* ACTION   : ImageMenu_OnLoadFS
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void ImageMenu_OnLoadFS(sGuiFSInfo * apFS)
{
	sString *			lpString;

	if( apFS )
	{
		if( apFS->mButton == eGUIFS_BUTTON_OK )
		{
			lpString = GuiFSInfo_FullNameBuild( &gImgClass.mFS );
			ImageMenu_ImageLoad( (U16)gImgClass.mImageIndex, lpString->mpChars );
			String_DeInit( lpString );
		}
	}		
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : sDegas * ImageMenu_GetpImage(const U16 aImageIndex)
* ACTION   : ImageMenu_GetpImage
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

sDegas * ImageMenu_GetpImage(const U16 aImageIndex)
{
	sDegas *	lpDegas;

	lpDegas = 0;

	if( aImageIndex < dIMGMENU_IMG_LIMIT )
	{
		lpDegas = gImgClass.mAssets[ aImageIndex ].mpDegas;
	}

	return( lpDegas );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : sDegas * ImageMenu_GetpImageCurrent(void)
* ACTION   : ImageMenu_GetpImageCurrent
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

sDegas * ImageMenu_GetpImageCurrent(void)
{
	return( gImgClass.mAssets[ gImgClass.mImageIndex ].mpDegas );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageMenu_GetpImageName( const U16 aImageIndex )
* ACTION   : ImageMenu_GetpImageName
* CREATION : 02.03.2004 PNK
*-----------------------------------------------------------------------------------*/

char *	ImageMenu_GetpImageName( const U16 aImageIndex )
{
	char * lpChars;

	if( aImageIndex < dIMGMENU_IMG_LIMIT )
	{
		lpChars = gImgClass.mAssets[ aImageIndex ].mFileName.mpChars;
	}
	else
	{
		lpChars = 0;
	}

	return( lpChars );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageMenu_GetImageIndex( void )
* ACTION   : ImageMenu_GetImageIndex
* CREATION : 04.03.2004 PNK
*-----------------------------------------------------------------------------------*/

U16	ImageMenu_GetImageIndex( void )
{
	return( (U16)gImgClass.mImageIndex );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void ImageMenu_ImageLoad(const U16 aImageIndex,const char * apFileName)
* ACTION   : ImageMenu_ImageLoad
* CREATION : 05.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void ImageMenu_ImageLoad(const U16 aImageIndex,const char * apFileName)
{
	if( (aImageIndex < dIMGMENU_IMG_LIMIT) && (apFileName) )
	{
		String_Set( &gImgClass.mAssets[ aImageIndex ].mFileName, apFileName );
		if( gImgClass.mAssets[ aImageIndex ].mpDegas )
		{
			File_UnLoad( gImgClass.mAssets[ aImageIndex ].mpDegas );
			gImgClass.mAssets[ aImageIndex ].mpDegas = 0;
		}
		gImgClass.mAssets[ aImageIndex ].mpDegas = File_Load( gImgClass.mAssets[ aImageIndex ].mFileName.mpChars );
		if( gImgClass.mAssets[ aImageIndex ].mpDegas )
		{
			Video_SetPalST( &gImgClass.mAssets[ aImageIndex ].mpDegas->mHeader.mPalette[0] );
		}
	}	
}




/* ################################################################################ */
