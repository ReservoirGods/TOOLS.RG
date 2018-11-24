/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"R_SPR.H"

#include	<GODLIB\ASSET\RELOCATE.H>
#include	<GODLIB\SPRITE\SPRITE.H>


/* ###################################################################################
#  DATA
################################################################################### */

sRelocater	gRSPRReloc;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32	RSPR_IsType(     void * apData, const U32 aSize, const U32 aID );
U32	RSPR_DoRelocate( void * apData, const U32 aSize, const U32 aID );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderSprite_Init( void )
* ACTION   : RenderSprite_Init
* CREATION : 06.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderSprite_Init( void )
{
	Relocater_Init( &gRSPRReloc, "BSB", RSPR_IsType, 0, 0, RSPR_DoRelocate, 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderSprite_DeInit( void )
* ACTION   : RenderSprite_DeInit
* CREATION : 06.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderSprite_DeInit( void )
{
	Relocater_DeInit( &gRSPRReloc );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RSPR_IsType( void * apData,const U32 aSize,const U32 aID )
* ACTION   : RSPR_IsType
* CREATION : 06.01.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	RSPR_IsType( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;
	(void)aID;
	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RSPR_DoRelocate( void * apData,const U32 aSize,const U32 aID )
* ACTION   : RSPR_DoRelocate
* CREATION : 06.01.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	RSPR_DoRelocate( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;
	(void)aID;
	Sprite_BlockRelocate( (sSpriteBlock*)apData );
	return( 1 );
}



/* ################################################################################ */
