/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"KERNEL.H"

#include	"BUILD.H"
#include	"EDITMENU.H"
#include	"IMG_MENU.H"
#include	"MAINMENU.H"

#include	"R_CUSTOM.H"

#include	<GODLIB\ASSET\ASSET.H>
#include	<GODLIB\ASSET\PACKAGE.H>
#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\GUI\GUI.H>
#include	<GODLIB\GUI\GUIDATA.H>
#include	<GODLIB\GUI\GUIFS.H>
#include	<GODLIB\GUI\GUIPARSE.H>
#include	<GODLIB\GUI\R_GUI.H>
#include	<GODLIB\IKBD\IKBD.H>
#include	<GODLIB\HASHTREE\HASHTREE.H>
#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\SCREEN\SCREEN.H>
#include	<GODLIB\STRING\STRING.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\VIDEO\VIDEO.H>


/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	eKERNEL_VAR_SCROLLX,
	eKERNEL_VAR_SCROLLXMAX,

	eKERNEL_VAR_LIMIT
};


/* ###################################################################################
#  DATA
################################################################################### */

U16				gKernelShutdownFlag;
U16				gKernelState;
sContext		gTileEditContext;
sPackage		gGuiPackage;
sAssetClient	gGubAsset;
sGuiData *		gpGuiData;
sHashTree		gKernelHashTree;
sHashTreeVar 	gKernelVars[ eKERNEL_VAR_LIMIT ];

U16				gKernelPal[ 16 ] =
{
	0xFFF,
	0xF00,
	0x0F0,
	0x00F,
	0xF0F,
	0xFF0,
	0x000,
	0xFFF,
	0xF00,
	0x0F0,
	0x00F,
	0xF0F,
	0xFF0,
	0x000,
};

/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	TileEdit_Kernel_VarsInit( void );
void	TileEdit_Kernel_VarsDeInit( void );

U16		TileEdit_Kernel_GetShutdownFlag( void );
void	TileEdit_Kernel_SetShutdownFlag( const U16 aState );

void	TileEdit_Kernel_CLI_mem( const char * apArgs );
void	TileEdit_Kernel_CLI_ass( const char * apArgs );

U32		GuiGub_OnLoad( void * apData, const U32 aSize, const U32 aUserData );
U32		GuiGub_OnUnLoad( void * apData, const U32 aSize, const U32 aUserData );
/*void	Cli_PrintLine( const char * apTxt );*/


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_Init( void )
* ACTION   : calls all initialisation routines
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_Init( void )
{
	Context_Init( &gTileEditContext, "GUI_GFX");

	HashTree_Init( &gKernelHashTree );

	Build_CliInit();

	Build_CliPrintLine( "TILEEDIT CLI" );
	Build_CliPrintfLine2( "BUILD : %d.%02d", dBUILD_NUMBER_HI, dBUILD_NUMBER_LO );
	Build_CliPrintfLine1( "DATE  : %s", __DATE__ );
	Build_CliPrintfLine1( "TIME  : %s", __TIME__ );

/*	RenderScreen_Init();*/
	Screen_Init( 320, 200, eGRAPHIC_COLOURMODE_4PLANE, eSCREEN_SCROLL_NONE );


	System_SetSnapShotFlag( 0 );
	Build_CliCmdInit( "mem", TileEdit_Kernel_CLI_mem );
	Build_CliCmdInit( "ass", TileEdit_Kernel_CLI_ass );

	TileEdit_Kernel_VarsInit();
	Gui_Init( &gKernelHashTree );
	GuiFS_Init( &gKernelHashTree );
	RenderGui_Init();
	EditMenu_Init( &gKernelHashTree );
	MainMenu_Init( &gKernelHashTree );
	ImageMenu_Init( &gKernelHashTree );
	RenderCustom_Init();

	Package_Init( &gGuiPackage, "GUI", "GUI_GFX" );
	gGubAsset.OnLoad = GuiGub_OnLoad;
	gGubAsset.OnUnLoad = GuiGub_OnUnLoad;
	AssetClient_Init( &gGubAsset, "TILEEDIT.GUB", "GUI_GFX", (void**)&gpGuiData );
	PackageManager_Load( &gGuiPackage );

	Video_SetPalST( gKernelPal );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_DeInit( void )
* ACTION   : calls all de-initialisation routines
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_DeInit( void )
{
	AssetClient_DeInit( &gGubAsset );
	Package_DeInit( &gGuiPackage );

	Build_CliDeInit();

	RenderCustom_DeInit();
	EditMenu_DeInit();
	ImageMenu_DeInit();
	MainMenu_DeInit();
	RenderGui_DeInit();
	GuiFS_DeInit( &gKernelHashTree );
	Gui_DeInit();
/*	RenderScreen_DeInit();*/
	Screen_DeInit();

	TileEdit_Kernel_VarsDeInit();
	HashTree_DeInit( &gKernelHashTree );
	Context_DeInit( &gTileEditContext );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_Main( void )
* ACTION   : main kernel loop
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_Main( void )
{
	U16 lPal[ 16 ] = { 0xFFFF, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	Build_CliPrintLine( "kernel main" );

	Video_SetPalST( lPal );
	Video_SetNextPalST( lPal );

	TileEdit_Kernel_SetShutdownFlag( 0 );
	while( !TileEdit_Kernel_GetShutdownFlag() )
	{

		Memory_Validate();
		PackageManager_Update();
		if( !Gui_Update() )
		{
			TileEdit_Kernel_RequestShutdown();
		}
		Screen_Update();
		RenderGui_Update( Screen_GetpLogicGraphic() );
/*
		RenderScreen_GetpGraphic()->mpVRAM = RenderScreen_GetpLogic();
		RenderGui_Update( RenderScreen_GetpGraphic() );
*/
		IKBD_Update();
	}
}




/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_GetState( void )
* ACTION   : requests that kernel be shutdown on next kernel loop
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_RequestShutdown( void )
{
	gKernelShutdownFlag = 1;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_GetState( void )
* ACTION   : returns kernel state
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

U16		TileEdit_Kernel_GetState( void )
{
	return( gKernelState );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_SetState( const U16 aState )
* ACTION   : sets kernel state
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_SetState( const U16 aState )
{
	gKernelState = aState;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_GetShutdownFlag( void )
* ACTION   : returns shutdown flag
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

U16		TileEdit_Kernel_GetShutdownFlag( void )
{
	return( gKernelShutdownFlag );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_SetShutdownFlag( const U16 aShutdown )
* ACTION   : sets shutdown flag
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_SetShutdownFlag( const U16 aShutdown )
{
	gKernelShutdownFlag = aShutdown;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_CLI_mem( const char * apArgs )
* ACTION   : Kernel_CLI_mem
* CREATION : 13.11.2002 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_CLI_mem( const char * apArgs )
{
	(void)apArgs;
	Build_CliPrintLine( "MEMORY DEBUG:" );
	Build_CliPrintfLine1( "allocs : %ld", Memory_GetAllocCount() );	
	Build_CliPrintfLine1( "size   : %ld", Memory_GetAllocatedSize() );	
	Build_CliPrintfLine1( "hitide : %ld", Memory_GetHighTide() );	
	Build_CliPrintfLine1( "bigal  : %ld", Memory_GetLargestAlloc() );	
	Build_CliPrintfLine1( "lilal  : %ld", Memory_GetSmallestAlloc() );	
	Build_CliPrintfLine1( "fail   : %ld", Memory_GetFailedSize() );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_CLI_ass( const char * apArgs )
* ACTION   : Kernel_CLI_ass
* CREATION : 11.01.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_CLI_ass( const char * apArgs )
{
	(void)apArgs;
/*	Build_CliFunc( PackageManager_ShowAll( Cli_PrintLine ) );	*/
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : GuiGub_OnLoad( void * apData,const U32 aSize, const U32 aUserData )
* ACTION   : GuiGub_OnLoad
* CREATION : 16.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32		GuiGub_OnLoad( void * apData,const U32 aSize, const U32 aUserData )
{
	(void)aUserData;
	(void)aSize;
	(void)apData;
	DebugLog_Printf2( "GuiGub_OnLoad %p %ld", apData, aSize );
	DebugLog_Printf0( "GuiGub_OnLoad relocate" );
	GuiData_Relocate( (sGuiData*)apData );
	DebugLog_Printf0( "GuiGub_OnLoad init" );
	GuiData_Init( (sGuiData*)apData, &gKernelHashTree );
	Gui_DataAdd( (sGuiData*)apData );
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : GuiGub_OnUnLoad( void * apData,const U32 aSize, const U32 aUserData )
* ACTION   : GuiGub_OnUnLoad
* CREATION : 16.02.2004 PNK
*-----------------------------------------------------------------------------------*/

U32		GuiGub_OnUnLoad( void * apData,const U32 aSize, const U32 aUserData )
{
	(void)aSize;
	(void)aUserData;
	Gui_DataRemove( (sGuiData*)apData );
	GuiData_Delocate( (sGuiData*)apData );	
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_VarsInit( void )
* ACTION   : Kernel_VarsInit
* CREATION : 18.02.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_VarsInit( void )
{
	S16	lScrollX,lScrollXMax;

	lScrollX = 6;
	lScrollXMax = 300;
	HashTree_Var_Init( &gKernelVars[ eKERNEL_VAR_SCROLLX    ], &gKernelHashTree, "TILEEDIT\\SCROLLX",	sizeof(S16), &lScrollX    );
	HashTree_Var_Init( &gKernelVars[ eKERNEL_VAR_SCROLLXMAX ], &gKernelHashTree, "TILEEDIT\\SCROLLXMAX",	sizeof(S16), &lScrollXMax );

}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Kernel_VarsDeInit( void )
* ACTION   : Kernel_VarsDeInit
* CREATION : 18.02.2004 PNK
*-----------------------------------------------------------------------------------*/

void	TileEdit_Kernel_VarsDeInit( void )
{
	U16	i;

	for( i=0; i<eKERNEL_VAR_LIMIT; i++ )
	{
		HashTree_Var_DeInit( &gKernelVars[ i ], &gKernelHashTree );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Assert_Display( const char * apString )
* ACTION   : Assert_Display
* CREATION : 29.02.2004 PNK
*-----------------------------------------------------------------------------------*/
#if 0
void	Assert_Display( const char * apString )
{
	(void)apString;
	Build_CliPrintLine( apString );
	Build_CliMain();
}
#endif



/* ################################################################################ */
