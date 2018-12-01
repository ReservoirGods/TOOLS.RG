/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"SAMPLE.H"

#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\ASSET\RELOCATE.H>


/* ###################################################################################
#  DATA
################################################################################### */

sHashList	gSampleList;
sRelocater	gSampleRelocater;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32		Sample_RelocIsType(   sAssetItem * apAsset );
U32		Sample_RelocDoInit(   sAssetItem * apAsset );
U32		Sample_RelocDoDeInit( sAssetItem * apAsset );
void	Sample_ItemInit( void * apItem );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : SampleManager_Init( void )
* ACTION   : SampleManager_Init
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

void	SampleManager_Init( void )
{
	HashList_Init( &gSampleList, sizeof( sAscotSpl ), Sample_ItemInit, 0 );
	Relocater_Init( &gSampleRelocater, "SPL", Sample_RelocIsType, Sample_RelocDoInit, Sample_RelocDoDeInit, 0, 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : SampleManager_DeInit( void )
* ACTION   : SampleManager_DeInit
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

void	SampleManager_DeInit( void )
{
	Relocater_DeInit( &gSampleRelocater );
	HashList_DeInit( &gSampleList );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : SampleManager_Register( const char * apName )
* ACTION   : SampleManager_Register
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

sAscotSpl *	SampleManager_Register( const char * apName )
{
	return( (sAscotSpl*)HashList_ItemRegister( &gSampleList, HashList_BuildHash(apName) ) );		
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : SampleManager_UnRegister( sAscotSpl * apSpl )
* ACTION   : SampleManager_UnRegister
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

void	SampleManager_UnRegister( sAscotSpl * apSpl )
{
	HashList_ItemUnRegister( &gSampleList, apSpl->mHLI.mID );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Sample_RelocIsType( sAssetItem * apAsset )
* ACTION   : Sample_RelocIsType
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	Sample_RelocIsType( sAssetItem * apAsset )
{
	(void)apAsset;

	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Sample_RelocDoInit( sAssetItem * apAsset )
* ACTION   : Sample_RelocDoInit
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	Sample_RelocDoInit( sAssetItem * apAsset )
{
	sAscotSpl *	lpSpl;

	lpSpl = (sAscotSpl*)HashList_ItemRegister( &gSampleList, apAsset->mHashKey );

	if( lpSpl )
	{
		lpSpl->mSound.mpSound = apAsset->mpData;
		lpSpl->mSound.mLength = apAsset->mSize;
	}

	return( lpSpl != 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Sample_RelocDoDeInit( sAssetItem * apAsset )
* ACTION   : Sample_RelocDoDeInit
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	Sample_RelocDoDeInit( sAssetItem * apAsset )
{
	HashList_ItemUnRegister( &gSampleList, apAsset->mHashKey );
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Sample_ItemInit( void * apItem )
* ACTION   : Sample_ItemInit
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

void	Sample_ItemInit( void * apItem )
{
	sAscotSpl *	lpSpl;

	lpSpl = (sAscotSpl*)apItem;
	lpSpl->mSound.mpSound = 0;
	lpSpl->mSound.mLength = 0;
}



/* ################################################################################ */
