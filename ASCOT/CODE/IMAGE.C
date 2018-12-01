/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"IMAGE.H"

#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\ASSET\RELOCATE.H>


/* ###################################################################################
#  DATA
################################################################################### */

sHashList	gImageList;
sRelocater	gImageRelocater;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32	Image_RelocIsType( sAssetItem * apAsset );
U32	Image_RelocDoInit( sAssetItem * apAsset );
U32	Image_RelocDoDeInit( sAssetItem * apAsset );
void	Image_ItemInit( void * apItem );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageManager_Init( void )
* ACTION   : ImageManager_Init
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ImageManager_Init( void )
{
	HashList_Init( &gImageList, sizeof( sImage ), Image_ItemInit, 0 );
	Relocater_Init( &gImageRelocater, "PI1", Image_RelocIsType, Image_RelocDoInit, Image_RelocDoDeInit, 0, 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageManager_DeInit( void )
* ACTION   : ImageManager_DeInit
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ImageManager_DeInit( void )
{
	Relocater_DeInit( &gImageRelocater );
	HashList_DeInit( &gImageList );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageManager_Register( const char * apName )
* ACTION   : ImageManager_Register
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

sImage *	ImageManager_Register( const char * apName )
{
	return( (sImage*)HashList_ItemRegister( &gImageList, HashList_BuildHash(apName) ) );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : ImageManager_UnRegister( sImage * apImage )
* ACTION   : ImageManager_UnRegister
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	ImageManager_UnRegister( sImage * apImage )
{
	HashList_ItemUnRegister( &gImageList, apImage->mHLI.mID );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Image_RelocIsType( sAssetItem * apAsset )
* ACTION   : Image_RelocIsType
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Image_RelocIsType( sAssetItem * apAsset )
{
	(void)apAsset;

	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Image_RelocDoInit( sAssetItem * apAsset )
* ACTION   : Image_RelocDoInit
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Image_RelocDoInit( sAssetItem * apAsset )
{
	sImage *	lpImage;

	lpImage = (sImage*)HashList_ItemRegister( &gImageList, apAsset->mHashKey );

	if( lpImage )
	{
		lpImage->mpPicture = apAsset->mpData;
	}

	return( (lpImage != 0) );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Image_RelocDoDeInit( sAssetItem * apAsset )
* ACTION   : Image_RelocDoDeInit
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Image_RelocDoDeInit( sAssetItem * apAsset )
{
	HashList_ItemUnRegister( &gImageList, apAsset->mHashKey );

	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Image_ItemInit( void * apItem )
* ACTION   : Image_ItemInit
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Image_ItemInit( void * apItem )
{
	sImage *	lpImage;
	
	lpImage            = (sImage*)apItem;
	lpImage->mpPicture = 0;
}


/* ################################################################################ */
