/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"AFONT.H"

#include	<GODLIB\ASSET\ASSET.H>
#include	<GODLIB\FILE\FILE.H>


/* ###################################################################################
#  DATA
################################################################################### */

sFontAscot		gFont;
sAssetClient	gFontAsset;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32		Font_OnLoad( void * apData, const U32 aSize, const U32 aUserData );
U32		Font_OnUnLoad( void * apData, const U32 aSize, const U32 aUserData );
void	Font_FixUp( sFontAscot * apFont,sDegas * apPic );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : Font_Init( void )
* ACTION   : Font_Init
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Font_Init( void )
{
	gFontAsset.OnLoad = Font_OnLoad;
	gFontAsset.OnUnLoad = Font_OnUnLoad;
	AssetClient_Init( &gFontAsset, "FONT8X8.PI1", "CONTEXT1", 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Font_DeInit( void )
* ACTION   : Font_DeInit
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Font_DeInit( void )
{
	AssetClient_DeInit( &gFontAsset );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Font_OnLoad( void * apData,const U32 aSize )
* ACTION   : Font_OnLoad
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Font_OnLoad( void * apData,const U32 aSize, const U32 aUserData )
{
	(void)aSize;
	(void)aUserData;
	Font_FixUp( &gFont, (sDegas*)apData );	
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Font_OnUnLoad( void * apData,const U32 aSize )
* ACTION   : Font_OnUnLoad
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Font_OnUnLoad( void * apData,const U32 aSize, const U32 aUserData )
{
	(void)apData;
	(void)aSize;
	(void)aUserData;
	return( 1 );
}




/*-----------------------------------------------------------------------------------*
* FUNCTION : Font_FixUp( sFont * apFont,sDegas * apPic )
* ACTION   : Font_FixUp
* CREATION : 30.04.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Font_FixUp( sFontAscot * apFont,sDegas * apPic )
{
	U16	i;
	U32	lOffset;

	apFont->mpGfx = apPic;


	for( i=0; i<256; i++ )
	{
		lOffset  = i/20;
		lOffset *= (160*8);

		lOffset += (i%20)<<3;

		apFont->mpChar[ i ] = (U8*)&apFont->mpGfx->mPixels[ lOffset/2 ];
	}	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Font_GetpFont( void )
* ACTION   : Font_GetpFont
* CREATION : 09.12.2003 PNK
*-----------------------------------------------------------------------------------*/

sFontAscot *	Font_GetpFont( void )
{
	return( &gFont );	
}


/* ################################################################################ */
