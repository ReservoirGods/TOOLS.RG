/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\LINEA\LINEA.H>
#include	<GODLIB\PICTYPES\DEGAS.H>


/* ###################################################################################
#  DATA
################################################################################### */

sDegas		gDegas;
sLineA *	gpLineA;

/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	FontGrab_FontDisplay( sLineAFont * apFont, U16 * apScreen );
void	FontGrab_FontDebug( sLineAFont * apFont );
void	FontGrab_ScreenClear( U16 * apScreen );
void	FontGrab_CharPrint( const U8 * apChar, const U16 aSrcX, const U16 aWidth, const U16 aFormWidth, const U16 Height, const U16 aX, const U16 aY, U16 * apScreen );
void	FontGrab_ScreenSave( char * apFileName );


/* ###################################################################################
#  CODE
################################################################################### */


/*-----------------------------------------------------------------------------------*
* FUNCTION : main( void )
* ACTION   : it begins  ...
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

int	main( void )
{
	GemDos_Super( 0 );

	gpLineA = LineA_Init();

	FontGrab_FontDisplay( gpLineA->mpFontRing[ 0 ], &gDegas.mPixels[ 0 ] );
	FontGrab_ScreenSave( "FONT6X6.PI1" );

	FontGrab_FontDisplay( gpLineA->mpFontRing[ 1 ], &gDegas.mPixels[ 0 ] );
	FontGrab_ScreenSave( "FONT8X8.PI1" );

	FontGrab_FontDisplay( gpLineA->mpFontRing[ 1 ]->mpNext, &gDegas.mPixels[ 0 ] );
	FontGrab_ScreenSave( "FONT8X16.PI1" );

	FontGrab_FontDebug( gpLineA->mpFontRing[ 1 ]->mpNext );

	GemDos_Cnecin();

	return( 0 );
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : FontGrab_FontDisplay( sLineAFont * apFont,U16 * apScreen )
* ACTION   : FontGrab_FontDisplay
* CREATION : 08.12.2002 PNK
*-----------------------------------------------------------------------------------*/

void	FontGrab_FontDisplay( sLineAFont * apFont,U16 * apScreen )
{
	U16	lX,lY;
	U16	i;

	lX = 0;
	lY = 0;
	FontGrab_ScreenClear( apScreen );
	for( i=0; i<256; i++ )
	{
		FontGrab_CharPrint( apFont->mpData, apFont->mpCharOffsetTable[ i ], apFont->mCellWidthMax, apFont->mFormWidth, apFont->mFormHeight, lX, lY, apScreen );
		lX += 16;
		if( lX >= 320 )
		{
			lX  = 0;
			lY += apFont->mFormHeight;
		}
	}
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : FontGrab_FontDebug( sLineAFont * apFont )
* ACTION   : FontGrab_FontDebug
* CREATION : 08.12.2002 PNK
*-----------------------------------------------------------------------------------*/

void	FontGrab_FontDebug( sLineAFont * apFont )
{
	printf( "ID:%d Name:%s\n", apFont->mID, apFont->mName );
	printf( "Point Size:%d\n",   apFont->mPointSize );
	printf( "FirstChar:%d LastChar:%d\n",   apFont->mCharFirst, apFont->mCharLast );
	printf( "Top:%d Ascent:%d\n",   apFont->mTop, apFont->mAscent );
	printf( "Half:%d Descent:%d Bot:%d\n",   apFont->mHalf, apFont->mDescent, apFont->mBottom );
	printf( "MaxCharW: %d MaxCellW: %d\n", apFont->mCharWidthMax, apFont->mCellWidthMax );
	printf( "Loff:%d Roff:%d Thick:%d\n", apFont->mOffsetLeft, apFont->mOffsetRight, apFont->mThicken );
	printf( "UL:%d Lite:%d Skew:%d\n", apFont->mUnderLineSize, apFont->mLighten, apFont->mSkew );
	printf( "FormWidth:%d FormHeight:%d\n", apFont->mFormWidth, apFont->mFormHeight );
	printf( "pNext:%p\n", apFont->mpNext );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : FontGrab_ScreenClear( U16 * apScreen )
* ACTION   : FontGrab_ScreenClear
* CREATION : 08.12.2002 PNK
*-----------------------------------------------------------------------------------*/

void	FontGrab_ScreenClear( U16 * apScreen )
{
	U16	i;
	
	for( i=0; i<16000; i++ )
	{
		apScreen[ i ] = 0;
	}
}

/*-----------------------------------------------------------------------------------*
* FUNCTION : FontGrab_CharPrint( const U8 * apChar,const U16 aSrcX, const U16 aWidth,const U16 aFormWidth,const U16 Height,const U16 aX,const U16 aY,U16 * apScreen )
* ACTION   : FontGrab_CharPrint
* CREATION : 08.12.2002 PNK
*-----------------------------------------------------------------------------------*/

void	FontGrab_CharPrint( const U8 * apChar,const U16 aSrcX, const U16 aWidth,const U16 aFormWidth,const U16 aHeight,const U16 aX,const U16 aY,U16 * apScreen )
{
	U32		lOff;
	U16		lPixels;
	U8		lGrab;
	U16 *	lpScreen;
	const U8 *	lpChar;
	U16		i,j;

	if( (aX<(320-8)) && (aY<(200-aHeight)) )
	{
		lpScreen = apScreen;

		lOff  = aY;
		lOff *= 80;
		lOff += (aX>>4)<<2;

		lpScreen += lOff;

		lOff = 0;

		lpChar = apChar;
		for( i=0; i<aHeight; i++ )
		{
			lPixels = 0;

			lOff    = aSrcX;
			lPixels = 0;
			for( j=0; j<aWidth; j++ )
			{
				lGrab     = lpChar[ lOff>>3 ];
				lPixels <<= 1;
				if( lGrab & (0x80>>(lOff&7)) )
				{
					lPixels |=1;
				}
				lOff++;
			}
			lPixels <<= (16-aWidth);

			*lpScreen = lPixels;
			lpScreen += 80;
			lpChar   += aFormWidth;
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : FontGrab_ScreenSave( char * apFileName )
* ACTION   : FontGrab_ScreenSave
* CREATION : 08.12.2002 PNK
*-----------------------------------------------------------------------------------*/

void	FontGrab_ScreenSave( char * apFileName )
{
	FILE *	lpFile;
	U16		i;


	gDegas.mHeader.mMode = 0;
	
	gDegas.mHeader.mPalette[ 0 ] = 0xFFF;
	for( i=1; i<16; i++ )
	{
		gDegas.mHeader.mPalette[ i ] = 0;
	}

	lpFile = fopen( apFileName, "wb" );
	if( lpFile )
	{
		fwrite( &gDegas, sizeof(sDegas), 1, lpFile );
		fclose( lpFile );
	}
}






/* ################################################################################ */
