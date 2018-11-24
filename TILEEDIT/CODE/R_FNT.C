/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"R_FNT.H"

#include	<GODLIB\ASSET\RELOCATE.H>
#include	<GODLIB\FONT\FONT.H>


/* ###################################################################################
#  DATA
################################################################################### */

sRelocater gRFontReloc;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32	RFONT_IsType( void * apData, const U32 aSize, const U32 aID );
U32	RFONT_DoRelocate( void * apData, const U32 aSize, const U32 aID );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderFont_Init( void )
* ACTION   : RenderFont_Init
* CREATION : 22.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderFont_Init( void )
{
	Relocater_Init( &gRFontReloc, "BFB", RFONT_IsType, 0, 0, RFONT_DoRelocate, 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RenderFont_DeInit( void )
* ACTION   : RenderFont_DeInit
* CREATION : 22.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	RenderFont_DeInit( void )
{
	Relocater_DeInit( &gRFontReloc );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RFONT_IsType( void * apData,const U32 aSize,const U32 aID )
* ACTION   : RFONT_IsType
* CREATION : 22.01.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	RFONT_IsType( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;
	(void)aID;
	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RFONT_DoRelocate( void * apData,const U32 aSize,const U32 aID )
* ACTION   : RFONT_DoRelocate
* CREATION : 22.01.2004 PNK
*-----------------------------------------------------------------------------------*/

U32	RFONT_DoRelocate( void * apData,const U32 aSize,const U32 aID )
{
	sFont * lpFont;

	(void)apData;
	(void)aSize;
	(void)aID;

	lpFont = (sFont*)apData;
	Font_Relocate( lpFont );

	return( 1 );
}


/* ################################################################################ */
