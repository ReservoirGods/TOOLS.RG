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

U32	Image_RelocIsType( void * apData, const U32 aSize, const U32 aID );
U32	Image_RelocDoInit( void * apData, const U32 aSize, const U32 aID );
U32	Image_RelocDoDeInit( void * apData, const U32 aSize, const U32 aID );
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
* FUNCTION : Image_RelocIsType( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Image_RelocIsType
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Image_RelocIsType( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;
	(void)aID;

	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Image_RelocDoInit( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Image_RelocDoInit
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Image_RelocDoInit( void * apData,const U32 aSize,const U32 aID )
{
	sImage *	lpImage;

	(void)aSize;

	(void)apData;
	lpImage = (sImage*)HashList_ItemRegister( &gImageList, aID );

	if( lpImage )
	{
		lpImage->mpPicture = apData;
	}

	return( (lpImage != 0) );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Image_RelocDoDeInit( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Image_RelocDoDeInit
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Image_RelocDoDeInit( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;

	HashList_ItemUnRegister( &gImageList, aID );

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
