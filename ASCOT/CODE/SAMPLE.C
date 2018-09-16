/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"SAMPLE.H"

#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\ASSET\RELOCATE.H>


/* ###################################################################################
#  DATA
################################################################################### */

sHashList		gSampleList;
sRelocater *	gpSampleRelocater;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32		Sample_RelocIsType( void * apData, const U32 aSize, const U32 aID );
U32		Sample_RelocDoInit( void * apData, const U32 aSize, const U32 aID );
U32		Sample_RelocDoDeInit( void * apData, const U32 aSize, const U32 aID );
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
	gpSampleRelocater = Relocater_Create( "SPL", Sample_RelocIsType, Sample_RelocDoInit, Sample_RelocDoDeInit, 0, 0 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : SampleManager_DeInit( void )
* ACTION   : SampleManager_DeInit
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

void	SampleManager_DeInit( void )
{
	Relocater_Destroy( gpSampleRelocater );
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
* FUNCTION : Sample_RelocIsType( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Sample_RelocIsType
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	Sample_RelocIsType( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;
	(void)aID;

	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Sample_RelocDoInit( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Sample_RelocDoInit
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	Sample_RelocDoInit( void * apData,const U32 aSize,const U32 aID )
{
	sAscotSpl *	lpSpl;

	lpSpl = (sAscotSpl*)HashList_ItemRegister( &gSampleList, aID );

	if( lpSpl )
	{
		lpSpl->mSound.mpSound = apData;
		lpSpl->mSound.mLength = aSize;
	}

	return( lpSpl != 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Sample_RelocDoDeInit( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Sample_RelocDoDeInit
* CREATION : 11.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	Sample_RelocDoDeInit( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;

	HashList_ItemUnRegister( &gSampleList, aID );
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
