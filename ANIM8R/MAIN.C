/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: MAIN.C
::
:: ANIM8R :: A quick'n'dirty 4bitplane animation viewer
::
:: [c] 2001 Reservoir Gods
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\FADE\FADE.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\PICTYPES\DEGAS.H>
#include	<GODLIB\PLATFORM\PLATFORM.H>
#include	<GODLIB\SCREEN\SCREEN.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\VBL\VBL.H>
#include	<GODLIB\VIDEO\VIDEO.H>
#include	<STDIO.H>
#include	<STRING.H>



/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	PF_STARTLINE,
	PF_GETCOMMAND,
	PF_GETPARAMETERNUMBER,
	PF_GETPARAMETERSTRING,
};


enum
{
	CMD_GRAPHIC,
	CMD_WIDTH,
	CMD_HEIGHT,
	CMD_X,
	CMD_Y,
	CMD_FRAMECOUNT,
	CMD_FRAMES,
	CMD_TIME,
	
	CMD_LIMIT
};

/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct
{
	char *	pName;
	U16		ParameterType;
	U16		ArrayLimit;
	U16		Count;
	U32		Numbers[ 64 ];
} sCommander;


/* ###################################################################################
#  DATA
################################################################################### */

sDegas *	gpDegas;
sCommander	gCommands[ CMD_LIMIT ];


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	InitScreen( void );
void	DeInitScreen( void );
U8		ParseFile( char * apFileName );
void	AnimLoop( void );
void	InitCmds( void );
void	CmdsPrint( void );
void	WaitSpace( void );


/* ###################################################################################
#  FUNCTIONS
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION    : main( int argc, char * argv[] )
* DESCRIPTION : entry point; command line arguments passed in here
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

S16	GodLib_Game_Main( S16 argc, char * argv[] )
{
	char * lpFileName;

	GemDos_Super( 0 );

	InitCmds();


	if( argc >= 2 )
		lpFileName = argv[1];
	else
		lpFileName = "ANIM.TXT";

	if( !ParseFile( lpFileName ) )
		return( 0 );

	if( !gCommands[ CMD_GRAPHIC ].Numbers[0] )
	{
		printf( "ANIM8R : Error : no valid [GRAPHIC] tag found\n" );
		WaitSpace();
		return( 0 );
	}
	if( !gCommands[ CMD_FRAMECOUNT ].Numbers[0] )
	{
		printf( "ANIM8R : Error : no valid [FRAMECOUNT] tag found\n" );
		WaitSpace();
		return( 0 );
	}

	CmdsPrint();

	gpDegas = File_Load( (char*)&gCommands[ CMD_GRAPHIC ].Numbers[ 0 ] );

	if( !gpDegas )
	{
		printf( "ANIM8R : Error : couldn't load file %s\n", (char*)&gCommands[ CMD_GRAPHIC ].Numbers[ 0 ] );
		WaitSpace();
		return( 0 );
	}

	Platform_Init();

	InitScreen();
	AnimLoop();
	DeInitScreen();

	Platform_DeInit();

	return( 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION    : WaitSpace()
* DESCRIPTION : waits for space key to be pressed; checks h/w reg
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

void	WaitSpace()
{
/*	while( *(U8*)0xFFFFFC02L != 0x39 ); */
	IKBD_WaitForKey( eIKBDSCAN_SPACE );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION    : InitCmds()
* DESCRIPTION : builds command table
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

void	InitCmds()
{
	U16	i;

	gCommands[ CMD_GRAPHIC ].pName = "GRAPHIC";
	gCommands[ CMD_GRAPHIC ].ArrayLimit = 1;
	gCommands[ CMD_GRAPHIC ].ParameterType = PF_GETPARAMETERSTRING;

	gCommands[ CMD_WIDTH ].pName = "WIDTH";
	gCommands[ CMD_WIDTH ].ArrayLimit = 1;
	gCommands[ CMD_WIDTH ].ParameterType = PF_GETPARAMETERNUMBER;

	gCommands[ CMD_HEIGHT ].pName = "HEIGHT";
	gCommands[ CMD_HEIGHT ].ArrayLimit = 1;
	gCommands[ CMD_HEIGHT ].ParameterType = PF_GETPARAMETERNUMBER;

	gCommands[ CMD_X ].pName = "X";
	gCommands[ CMD_X ].ArrayLimit = 1;
	gCommands[ CMD_X ].ParameterType = PF_GETPARAMETERNUMBER;

	gCommands[ CMD_Y ].pName = "Y";
	gCommands[ CMD_Y ].ArrayLimit = 1;
	gCommands[ CMD_Y ].ParameterType = PF_GETPARAMETERNUMBER;

	gCommands[ CMD_TIME ].pName = "TIME";
	gCommands[ CMD_TIME ].ArrayLimit = 1;
	gCommands[ CMD_TIME ].ParameterType = PF_GETPARAMETERNUMBER;

	gCommands[ CMD_FRAMES ].pName = "FRAMES";
	gCommands[ CMD_FRAMES ].ArrayLimit = 64;
	gCommands[ CMD_FRAMES ].ParameterType = PF_GETPARAMETERNUMBER;

	gCommands[ CMD_FRAMECOUNT ].pName = "FRAMECOUNT";
	gCommands[ CMD_FRAMECOUNT ].ArrayLimit = 1;
	gCommands[ CMD_FRAMECOUNT ].ParameterType = PF_GETPARAMETERNUMBER;

	for( i=0; i<CMD_LIMIT; i++ )
	{
		gCommands[ i ].Numbers[ 0 ] = 0;
		gCommands[ i ].Count        = 0;
	}
	gCommands[ CMD_WIDTH ].Numbers[ 0 ] = 16;
	gCommands[ CMD_HEIGHT ].Numbers[ 0 ] = 16;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION    : CmdsPrint()
* DESCRIPTION : prints out current command parameters
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

void	CmdsPrint()
{
	U16	i,j;

	for( i=0; i<CMD_LIMIT; i++ )
	{
		printf( "%s :\n", gCommands[ i ].pName );
		switch( gCommands[ i ].ParameterType )
		{
		case PF_GETPARAMETERSTRING:
			printf( "%s\n", (char*)&gCommands[ i ].Numbers[ 0 ] );
			break;
		case PF_GETPARAMETERNUMBER:
			for( j=0; j<gCommands[ i ].Count; j++ )
			{
				printf( "%ld\n", gCommands[ i ].Numbers[ j ] );
			}
			break;

		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION    : ParseFile( char * apFileName )
* DESCRIPTION : parses file and sets up all command parameters
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

U8	ParseFile( char * apFileName )
{
	char	lString[ 1024 ];
	U16		lCmdIndex;
	U16		lStrIndex;
	U16		lMode;
	U8 *	lpTxt;
	U8		lChar;
	U32		lSize;
	U32		lIndex;
	U32		lValue;
	U16		i;
	U8 *	lpString;

	lpTxt = File_Load( apFileName );

	if( !lpTxt )
	{
		printf( "ANIM8R : Couldn't load file '%s'\n", apFileName );
		return( 0 );
	}

	lSize     = File_GetSize( apFileName );
	lIndex    = 0;
	lCmdIndex = 0;
	lStrIndex = 0;
	lMode     = PF_STARTLINE;

	while( lIndex < lSize )
	{
		lChar = lpTxt[ lIndex++ ];

		switch( lMode )
		{

		case	PF_STARTLINE:
			lCmdIndex = 0;
			lStrIndex = 0;
			if( lChar == '[' )
			{
				lMode = PF_GETCOMMAND;
			}
			break;

		case	PF_GETCOMMAND:
			if( lChar == ']' )
			{
				while( (lpTxt[ lIndex ] != 0xA) && (lpTxt[ lIndex ] != 0xD) && (lIndex<lSize) )
					lIndex++;
				if(( lpTxt[lIndex] == 0xA ) || (lpTxt[lIndex]==0xD))
					lIndex++;
				if(( lpTxt[lIndex] == 0xA ) || (lpTxt[lIndex]==0xD))
					lIndex++;

				lString[ lStrIndex ] = 0;

				lMode = PF_STARTLINE;
				i = 0;
				while( i < CMD_LIMIT )
				{
					if( !strcmp( lString, gCommands[i].pName ) )
					{
						lCmdIndex = i;
						lMode = gCommands[ lCmdIndex ].ParameterType;
						i = CMD_LIMIT;
						lStrIndex = 0;
						lValue = 0;
					}
					i++;
				}
			}
			else if( lChar > ' ' )
			{
				lString[ lStrIndex++ ] = lChar;
			}
			else if(( lChar == 0xA ) || (lChar==0xD))
			{
				if(( lpTxt[lIndex] == 0xA ) || (lpTxt[lIndex]==0xD))
					lIndex++;

				lMode = PF_STARTLINE;
			}
			break;

		case	PF_GETPARAMETERSTRING:
			if( (lChar == 0xA ) || ( lChar == 0xD ) )
			{
				if(( lpTxt[lIndex] == 0xA ) || (lpTxt[lIndex]==0xD))
					lIndex++;
				lString[ lStrIndex ] = 0;
				lMode = PF_STARTLINE;
			}
			else if( lChar > ' ' )
			{
				lpString = (U8*)&gCommands[ lCmdIndex ].Numbers[ 0 ];
				lpString[ lStrIndex++ ] = lChar;
			}
			break;

		case	PF_GETPARAMETERNUMBER:
			if( (lChar == 0xA ) || ( lChar == 0xD ) )
			{
				if(( lpTxt[lIndex] == 0xA ) || (lpTxt[lIndex]==0xD))
					lIndex++;

				if( gCommands[ lCmdIndex ].Count > gCommands[ lCmdIndex ].ArrayLimit )
				{
					lMode = PF_STARTLINE;
				}
				else
				{
					gCommands[ lCmdIndex ].Numbers[ gCommands[lCmdIndex].Count ] = lValue;
					gCommands[ lCmdIndex ].Count++;
				}

				if( gCommands[ lCmdIndex ].Count > gCommands[ lCmdIndex ].ArrayLimit )
					lMode = PF_STARTLINE;

				lValue = 0;

			}
			else if((lChar>='0') && (lChar<='9'))
			{
				lValue *= 10;
				lValue += lChar-'0';
			}
			else if( lChar=='[' )
			{
				lMode = PF_GETCOMMAND;
			}
			break;
			
		}
	}

	switch( lMode )
	{
	case	PF_GETPARAMETERSTRING:
		lString[ lStrIndex ] = 0;
		break;
	case	PF_GETPARAMETERNUMBER:
		if( gCommands[ lCmdIndex ].Count < gCommands[ lCmdIndex ].ArrayLimit )
		{
			gCommands[ lCmdIndex ].Numbers[ gCommands[lCmdIndex].Count ] = lValue;
			gCommands[ lCmdIndex ].Count++;
		}
		break;
	}

	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION    : InitScreen()
* DESCRIPTION : allocates & aligns memory for screen buffers
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

void	InitScreen()
{
	Screen_Init( 320, 200, eGRAPHIC_COLOURMODE_4PLANE, eSCREEN_SCROLL_NONE );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION    : DeInitScreen()
* DESCRIPTION : frees memory allocated for screen buffer
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

void	DeInitScreen()
{
	Screen_DeInit();
}




/*-----------------------------------------------------------------------------------*
* FUNCTION    : DrawFrame( U32 aFrameIndex )
* DESCRIPTION : renders a sprite frame to the logical screen buffer
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

void	DrawFrame( U32 aFrameIndex )
{
	U16		x,y;
	U16		lSpritesPerLine;
	U32		lFrameOffset;
	U16 *	lpScreen;
	U16	*	lpSrc;

	lSpritesPerLine = (U16)( 320 / gCommands[ CMD_WIDTH ].Numbers[ 0 ] );
	y = (U16)( aFrameIndex / lSpritesPerLine);
	x = (U16)( aFrameIndex - ( y*lSpritesPerLine ) );
	lFrameOffset = y;
	lFrameOffset *= 80 * gCommands[ CMD_HEIGHT ].Numbers[ 0 ];
	lFrameOffset += ( x * (((gCommands[ CMD_WIDTH ].Numbers[ 0 ]+15)>>4)<<2) );

	for( y=0; y<gCommands[ CMD_HEIGHT ].Numbers[ 0 ]; y++ )
	{
		lpScreen  = Screen_GetpLogic();
		lpScreen += y*80L;

		lpSrc = gpDegas->mPixels;
		lpSrc += y*80L;
		lpSrc += lFrameOffset;

		for( x=0; x<((gCommands[ CMD_WIDTH ].Numbers[ 0 ]+15)>>4); x++ )
		{
			*lpScreen++ = *lpSrc++;
			*lpScreen++ = *lpSrc++;
			*lpScreen++ = *lpSrc++;
			*lpScreen++ = *lpSrc++;
		}
	}

}


/*-----------------------------------------------------------------------------------*
* FUNCTION    : AnimLoop()
* DESCRIPTION : main loop; manages anim speed and display
* AUTHOR      : 03.01.01 PNK
*-----------------------------------------------------------------------------------*/

void	AnimLoop()
{
	U8		lExitFlag;
	U32		lFrameIndex;
/*	U16 *	lpTemp;*/
	S32		lTime;

	lExitFlag   = 0;
	lFrameIndex = 0;
	lTime       = gCommands[ CMD_TIME ].Numbers[ 0 ];

	Video_SetMode( 320, 200, eVIDEO_MODE_4PLANE, 320, 0, 0 );
	Vbl_WaitVbl();
	Video_SetPalST( gpDegas->mHeader.mPalette );

	while( !lExitFlag )
	{
		Screen_Update();
		DrawFrame( gCommands[ CMD_FRAMES ].Numbers[ lFrameIndex ] );
		

		lTime--;
		if( lTime <= 0 )
		{
			lFrameIndex++;
			if( lFrameIndex >= gCommands[ CMD_FRAMECOUNT ].Numbers[ 0 ] )
			{
				lFrameIndex = 0;
			}
			if( lFrameIndex >= gCommands[ CMD_FRAMES ].Count )
				lFrameIndex = 0;

			lTime = gCommands[ CMD_TIME ].Numbers[ 0 ];
		}

		if( IKBD_GetKbdBytesWaiting() )
		{
			if( IKBD_PopKbdByte() == 0x44 )
			{
				lExitFlag = 1;
			}
		}
	}

}