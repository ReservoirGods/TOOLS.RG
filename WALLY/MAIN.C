/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\MEMORY\MEMORY.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

#define	dWALLY_SRC_FILENAME	"PUZZLES.TXT"
#define	dWALLY_DST_FILENAME	"PUZZLES2.TXT"


/* ###################################################################################
#  DATA
################################################################################### */

S32		gWallySrcSize;
S32		gWallyDstSize;
U32		gWallySrcLineCount;
char *	gpWallySrc;
char *	gpWallyDst;
char **	gppWallySrcLines;
char **	gppWallyDstLines;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U8		Wally_Init( void );
void	Wally_Main( void );
void	Wally_DeInit( void );

void	Wally_SrcLineCountInit( void );
void	Wally_pSrcLinesInit( void );

S32		Wally_GetSrcSize( void );
void	Wally_SetSrcSize( S32 aSize );

char *	Wally_GetpSrc( void );
void	Wally_SetpSrc( char * apSrc );

S32		Wally_GetDstSize( void );
void	Wally_SetDstSize( S32 aSize );

char *	Wally_GetpDst( void );
void	Wally_SetpDst( char * apDst );

U32		Wally_GetSrcLineCount( void );
void	Wally_SetSrcLineCount( U32 aLineCount );

U8		Wally_TokenIsMap( char * apString );


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( void )
{
	if( Wally_Init() )
	{
		Wally_Main();
		Wally_DeInit();
	}

	return( 0 );
}

U8	Wally_Init()
{
	if( !File_Exists( dWALLY_SRC_FILENAME) )
		return( 0 );

	Wally_SetSrcSize( File_GetSize( dWALLY_SRC_FILENAME ) );

	if( Wally_GetSrcSize() <= 0 )
		return( 0 );

	Wally_SetpSrc( Memory_Calloc( Wally_GetSrcSize() + 1 ) );
	if( !Wally_GetpSrc() )
		return( 0 );

	File_LoadAt( dWALLY_SRC_FILENAME, Wally_GetpSrc() );

	Wally_SrcLineCountInit();

	Wally_pSrcLinesInit();

	Wally_SetpDst( Memory_Calloc( Wally_GetSrcSize() << 1 ) );

	return( 1 );
}

void	Wally_DeInit()
{
	mMEMFREE( Wally_GetpSrc() );
	mMEMFREE( Wally_GetpDst() );
}

void	Wally_Main( void )
{
	char *	lpSrc;
	char *	lpSrcNext;
	char *	lpDst;
	char *	lpDstCurLine;
	char *	lpDstTopLine;
	char *	lpDstBotLine = 0;
	U32		lWallIndex;
	U32		lLineIndex;
	U32		lDstLineIndex;
	U32		lSize;
	U16		i,j;

	lLineIndex    = 0;
	lDstLineIndex = 0;

	lpDst = Wally_GetpDst();

	while( lLineIndex < Wally_GetSrcLineCount() )
	{
		lpSrc     = gppWallySrcLines[ lLineIndex   ];
		lpSrcNext = gppWallySrcLines[ lLineIndex+1 ];

		if( (Wally_TokenIsMap( lpSrc )) && (lLineIndex+11 < Wally_GetSrcLineCount() ) )
		{
			/*			copy token */
			gppWallyDstLines[ lDstLineIndex++ ] = lpDst;
			while( lpSrc < lpSrcNext )
			{
				*lpDst++ = *lpSrc++;
			}
			lLineIndex++;
			lpSrc     = gppWallySrcLines[ lLineIndex   ];
			lpSrcNext = gppWallySrcLines[ lLineIndex+1 ];

			/*			copy top line (no duplication needed ) */

			gppWallyDstLines[ lDstLineIndex++ ] = lpDst;
			lpDstTopLine                        = lpDst;
			while( lpSrc < lpSrcNext )
			{
				*lpDst++ = *lpSrc++;
			}
			lpDstTopLine[ 0 ]='|';
			for( i=0; i<24; i++ )
			{
				if( (lpDstTopLine[ i ] == '_') || (lpDstTopLine[ i ] == '¯') )
				{
					lpDstTopLine[ i ] = '-';
				}
			}
			lpDstTopLine[24 ]='|';
			

			lLineIndex++;

			lWallIndex = lDstLineIndex;

			for( i=0; i<9; i++ )
			{
				lpSrc     = gppWallySrcLines[ lLineIndex   ];
				lpSrcNext = gppWallySrcLines[ lLineIndex+1 ];
				gppWallyDstLines[ lDstLineIndex++ ] = lpDst;
				while( lpSrc < lpSrcNext )
				{
					*lpDst++ = *lpSrc++;
				}
				lpSrc     = gppWallySrcLines[ lLineIndex ];

				gppWallyDstLines[ lDstLineIndex++ ] = lpDst;
				while( lpSrc < lpSrcNext )
				{
					*lpDst++ = *lpSrc++;
				}
				lLineIndex++;
			}

			/*			copy bot line (no duplication needed ) */

			lpSrc     = gppWallySrcLines[ lLineIndex   ];
			lpSrcNext = gppWallySrcLines[ lLineIndex+1 ];
			gppWallyDstLines[ lDstLineIndex++ ] = lpDst;
			while( lpSrc < lpSrcNext )
			{
				*lpDst++ = *lpSrc++;
			}
			lLineIndex++;

			for( i=0; i<9; i++ )
			{
				lpDstTopLine = gppWallyDstLines[ lWallIndex+0 ];
				lpDstCurLine = gppWallyDstLines[ lWallIndex+1 ];
				lpDstBotLine = gppWallyDstLines[ lWallIndex+2 ];
				lWallIndex += 2;

				for( j=0; j<24; j++ )
				{
					if( lpDstTopLine[ j ] == '_' )
					{
						lpDstCurLine[ j ] =  '-';
					}
					if( lpDstBotLine[ j ] == '¯' )
					{
						lpDstCurLine[ j ] =  '-';
					}
					if( lpDstCurLine[ j ] != '-') 
					{
						if( (j==0) || (j==24) )
						{
							lpDstCurLine[ j ] = '|';
						}
						else
						{
							lpDstCurLine[ j ] = ' ';
						}
					}
					if( (lpDstTopLine[ j ] == '_') || (lpDstTopLine[ j ] == '¯') )
					{
						lpDstTopLine[ j ] = ' ';
					}

				}
			}
			lpDst = lpDstBotLine;
/*
			lpDstCurLine[ 0 ] = ' ';
			lpDstCurLine[24 ] = ' ';
*/
			lDstLineIndex--;

		}
		else
		{
			gppWallyDstLines[ lDstLineIndex++ ] = lpDst;
			while( lpSrc < lpSrcNext )
			{
				*lpDst++ = *lpSrc++;
			}
			lLineIndex++;
		}
	}
	gppWallyDstLines[ lDstLineIndex++ ] = lpDst;

	lSize  = (U32)lpDst;
	lSize -= (U32)Wally_GetpDst();
	Wally_SetDstSize( lSize );

	File_Save( dWALLY_DST_FILENAME, gpWallyDst, lSize );
}


U8		Wally_TokenIsMap( char * apString )
{

	if( apString[ 0 ] == '[' )
	{
		if( apString[ 1 ] == 'M' )
		{
			if( apString[ 2 ] == 'A' )
			{
				if( apString[ 3 ] == 'P' )
				{
					if( apString[ 4 ] == ']' )
					{
						return( 1 );
					}
				}
			}
		}
	}
	return( 0 );
}

void	Wally_SrcLineCountInit( void )
{
	char *	lpTxt;
	U32		lLineCount;
	S32		i;

	lLineCount = 1;
	lpTxt      = Wally_GetpSrc();
	i          = 0;

	while( i < Wally_GetSrcSize() )
	{
		if( (lpTxt[ i ] == 0xA) || (lpTxt[ i ] == 0xD) )
		{
			lLineCount++;
			if( (lpTxt[ i+1 ] == 0xA) || (lpTxt[ i+1 ] == 0xD) )
			{
				i++;
			}
		}
		i++;
	}

	Wally_SetSrcLineCount( lLineCount );
}

void	Wally_pSrcLinesInit( void )
{
	char *	lpTxt;
	U32	lLineIndex;
	S32		i;

	gppWallySrcLines = Memory_Calloc( (Wally_GetSrcLineCount()+1) * 4 );
	gppWallyDstLines = Memory_Calloc( (Wally_GetSrcLineCount()<<1) * 4 );

	lLineIndex = 0;
	lpTxt      = Wally_GetpSrc();
	i          = 0;

	gppWallySrcLines[ lLineIndex++ ] = lpTxt;

	while( i < Wally_GetSrcSize() )
	{
		if( (lpTxt[ i ] == 0xA) || (lpTxt[ i ] == 0xD) )
		{
			if( (lpTxt[ i+1 ] == 0xA) || (lpTxt[ i+1 ] == 0xD) )
			{
				i++;
			}
			gppWallySrcLines[ lLineIndex++ ] = &lpTxt[ i+1 ];
		}
		i++;
	}
	gppWallySrcLines[ lLineIndex++ ] = lpTxt;

}


S32		Wally_GetSrcSize( void )
{
	return( gWallySrcSize );
}

void	Wally_SetSrcSize( S32 aSize )
{
	gWallySrcSize = aSize;
}


U32		Wally_GetSrcLineCount( void )
{
	return( gWallySrcLineCount );
}

void	Wally_SetSrcLineCount( U32 aLineCount )
{
	gWallySrcLineCount = aLineCount;
}


char *	Wally_GetpSrc( void )
{
	return( gpWallySrc );
}

void	Wally_SetpSrc( char * apSrc )
{
	gpWallySrc = apSrc;
}


S32		Wally_GetDstSize( void )
{
	return( gWallyDstSize );
}

void	Wally_SetDstSize( S32 aSize )
{
	gWallyDstSize = aSize;
}


char *	Wally_GetpDst( void )
{
	return( gpWallyDst );
}

void	Wally_SetpDst( char * apDst )
{
	gpWallyDst = apDst;
}

