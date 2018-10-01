/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"MAINMENU.H"

#include	"EDITMENU.H"
#include	"TILEMAP.H"
#include	"TM_PROJ.H"

#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GUI\GUI.H>
#include	<GODLIB\GUI\GUIDATA.H>
#include	<GODLIB\GUI\GUIEDIT.H>
#include	<GODLIB\GUI\GUIFS.H>
#include	<GODLIB\MEMORY\MEMORY.H>


/* ###################################################################################
#  ENUMS
################################################################################### */



enum
{
	eMM_VARCLIENT_LOADMAP,
	eMM_VARCLIENT_SAVEMAP,
	eMM_VARCLIENT_LOADPRJ,
	eMM_VARCLIENT_SAVEPRJ,
	eMM_VARCLIENT_LOADBIN,
	eMM_VARCLIENT_SAVEBIN,

	eMM_VARCLIENT_LIMIT
};

enum
{
	eMM_FS_MAP,
	eMM_FS_IMG,
	eMM_FS_PRJ,
	eMM_FS_BIN,

	eMM_FS_LIMIT
};


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sMainMenuClass
{
	sGuiFSInfo				mFSs[ eMM_FS_LIMIT ];
	sHashTree *				mpTree;
	sHashTreeVarClient *	mpVarClients[ eMM_VARCLIENT_LIMIT ];
} sMainMenuClass;


/* ###################################################################################
#  DATA
################################################################################### */

sMainMenuClass	gMainMenuClass;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	MainMenu_OnLoadMap( sHashTreeVarClient * apClient );
void	MainMenu_OnSaveMap( sHashTreeVarClient * apClient );
void	MainMenu_OnLoadPrj( sHashTreeVarClient * apClient );
void	MainMenu_OnSavePrj( sHashTreeVarClient * apClient );
void	MainMenu_OnLoadBin( sHashTreeVarClient * apClient );
void	MainMenu_OnSaveBin( sHashTreeVarClient * apClient );

void	MainMenu_OnLoadMapFS( sGuiFSInfo * apFS );
void	MainMenu_OnSaveMapFS( sGuiFSInfo * apFS );
void	MainMenu_OnLoadPrjFS( sGuiFSInfo * apFS );
void	MainMenu_OnSavePrjFS( sGuiFSInfo * apFS );
void	MainMenu_OnLoadBinFS( sGuiFSInfo * apFS );
void	MainMenu_OnSaveBinFS( sGuiFSInfo * apFS );


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_Init(sHashTree * apTree)
* ACTION   : MainMenu_Init
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_Init(sHashTree * apTree)
{
	Memory_Clear( sizeof(gMainMenuClass), &gMainMenuClass );
	gMainMenuClass.mpTree = apTree;

	gMainMenuClass.mpVarClients[ eMM_VARCLIENT_LOADMAP ] = HashTree_VarClientRegister( apTree, "GUI\\BUTTONS\\BUTT_MM_LOADMAP", MainMenu_OnLoadMap, 0, 0, 0 );
	gMainMenuClass.mpVarClients[ eMM_VARCLIENT_SAVEMAP ] = HashTree_VarClientRegister( apTree, "GUI\\BUTTONS\\BUTT_MM_SAVEMAP", MainMenu_OnSaveMap, 0, 0, 0 );
	gMainMenuClass.mpVarClients[ eMM_VARCLIENT_LOADPRJ ] = HashTree_VarClientRegister( apTree, "GUI\\BUTTONS\\BUTT_MM_LOADPRJ", MainMenu_OnLoadPrj, 0, 0, 0 );
	gMainMenuClass.mpVarClients[ eMM_VARCLIENT_SAVEPRJ ] = HashTree_VarClientRegister( apTree, "GUI\\BUTTONS\\BUTT_MM_SAVEPRJ", MainMenu_OnSavePrj, 0, 0, 0 );
	gMainMenuClass.mpVarClients[ eMM_VARCLIENT_LOADBIN ] = HashTree_VarClientRegister( apTree, "GUI\\BUTTONS\\BUTT_MM_LOADBIN", MainMenu_OnLoadBin, 0, 0, 0 );
	gMainMenuClass.mpVarClients[ eMM_VARCLIENT_SAVEBIN ] = HashTree_VarClientRegister( apTree, "GUI\\BUTTONS\\BUTT_MM_SAVEBIN", MainMenu_OnSaveBin, 0, 0, 0 );

	GuiFSInfo_Init( &gMainMenuClass.mFSs[ eMM_FS_BIN ], "LOAD BIN", "*.BIN", "TEST.BIN" );
	GuiFSInfo_Init( &gMainMenuClass.mFSs[ eMM_FS_IMG ], "LOAD IMG", "*.PI1", "TEST.PI1" );
	GuiFSInfo_Init( &gMainMenuClass.mFSs[ eMM_FS_MAP ], "LOAD MAP", "*.MAP", "TEST.MAP" );
	GuiFSInfo_Init( &gMainMenuClass.mFSs[ eMM_FS_PRJ ], "LOAD PRJ", "*.TPJ", "TEST.TPJ" );

}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_DeInit(void)
* ACTION   : MainMenu_DeInit
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_DeInit(void)
{
	U16	i;
	for( i=0; i<eMM_FS_LIMIT; i++ )
	{
		GuiFSInfo_DeInit( &gMainMenuClass.mFSs[ i ] );
	}
	
	for( i=0; i<eMM_VARCLIENT_LIMIT; i++ )
	{
		HashTree_VarClientUnRegister( gMainMenuClass.mpTree, gMainMenuClass.mpVarClients[ i ] );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnLoadMap(sHashTreeVarClient * apClient)
* ACTION   : MainMenu_OnLoadMap
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnLoadMap(sHashTreeVarClient * apClient)
{
	sGuiEvent *	lpEvent;

	lpEvent = (sGuiEvent*)apClient->mpVar->mpData;

	if( lpEvent )
	{
		if( eGUIEVENT_BUTTON_LEFTCLICK == lpEvent->mEvent )
		{
			GuiButton_DeSelect( (sGuiButton*)lpEvent->mpInfo );

			String_Update( &gMainMenuClass.mFSs[ eMM_FS_MAP ].mTitle, "LOAD MAP" );
			gMainMenuClass.mFSs[ eMM_FS_MAP ].mpCB = MainMenu_OnLoadMapFS;
			GuiFS_Open( &gMainMenuClass.mFSs[ eMM_FS_MAP ] );
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnSaveMap(sHashTreeVarClient * apClient)
* ACTION   : MainMenu_OnSaveMap
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnSaveMap(sHashTreeVarClient * apClient)
{
	sGuiEvent *	lpEvent;

	lpEvent = (sGuiEvent*)apClient->mpVar->mpData;

	if( lpEvent )
	{
		if( eGUIEVENT_BUTTON_LEFTCLICK == lpEvent->mEvent )
		{
			GuiButton_DeSelect( (sGuiButton*)lpEvent->mpInfo );

			String_Update( &gMainMenuClass.mFSs[ eMM_FS_MAP ].mTitle, "SAVE MAP" );
			gMainMenuClass.mFSs[ eMM_FS_MAP ].mpCB = MainMenu_OnSaveMapFS;
			GuiFS_Open( &gMainMenuClass.mFSs[ eMM_FS_MAP ] );
		}
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnLoadPrj(sHashTreeVarClient * apClient)
* ACTION   : MainMenu_OnLoadPrj
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnLoadPrj(sHashTreeVarClient * apClient)
{
	sGuiEvent *	lpEvent;

	lpEvent = (sGuiEvent*)apClient->mpVar->mpData;

	if( lpEvent )
	{
		if( eGUIEVENT_BUTTON_LEFTCLICK == lpEvent->mEvent )
		{
			GuiButton_DeSelect( (sGuiButton*)lpEvent->mpInfo );

			String_Update( &gMainMenuClass.mFSs[ eMM_FS_PRJ ].mTitle, "LOAD PROJECT" );
			gMainMenuClass.mFSs[ eMM_FS_PRJ ].mpCB = MainMenu_OnLoadPrjFS;
			GuiFS_Open( &gMainMenuClass.mFSs[ eMM_FS_PRJ ] );
		}
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnSavePrj(sHashTreeVarClient * apClient)
* ACTION   : MainMenu_OnSavePrj
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnSavePrj(sHashTreeVarClient * apClient)
{
	sGuiEvent *	lpEvent;

	lpEvent = (sGuiEvent*)apClient->mpVar->mpData;

	if( lpEvent )
	{
		if( eGUIEVENT_BUTTON_LEFTCLICK == lpEvent->mEvent )
		{
			GuiButton_DeSelect( (sGuiButton*)lpEvent->mpInfo );

			String_Update( &gMainMenuClass.mFSs[ eMM_FS_PRJ ].mTitle, "SAVE PROJECT" );
			gMainMenuClass.mFSs[ eMM_FS_PRJ ].mpCB = MainMenu_OnSavePrjFS;
			GuiFS_Open( &gMainMenuClass.mFSs[ eMM_FS_PRJ ] );
		}
	}	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnLoadBin(sHashTreeVarClient * apClient)
* ACTION   : MainMenu_OnLoadBin
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnLoadBin(sHashTreeVarClient * apClient)
{
	sGuiEvent *	lpEvent;

	lpEvent = (sGuiEvent*)apClient->mpVar->mpData;

	if( lpEvent )
	{
		if( eGUIEVENT_BUTTON_LEFTCLICK == lpEvent->mEvent )
		{
			GuiButton_DeSelect( (sGuiButton*)lpEvent->mpInfo );

			String_Update( &gMainMenuClass.mFSs[ eMM_FS_MAP ].mTitle, "LOAD BIN" );
			gMainMenuClass.mFSs[ eMM_FS_BIN ].mpCB = MainMenu_OnLoadBinFS;
			GuiFS_Open( &gMainMenuClass.mFSs[ eMM_FS_BIN ] );
		}
	}	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnSaveBin(sHashTreeVarClient * apClient)
* ACTION   : MainMenu_OnSaveBin
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnSaveBin(sHashTreeVarClient * apClient)
{
	sGuiEvent *	lpEvent;

	lpEvent = (sGuiEvent*)apClient->mpVar->mpData;

	if( lpEvent )
	{
		if( eGUIEVENT_BUTTON_LEFTCLICK == lpEvent->mEvent )
		{
			GuiButton_DeSelect( (sGuiButton*)lpEvent->mpInfo );

			String_Update( &gMainMenuClass.mFSs[ eMM_FS_MAP ].mTitle, "SAVE BIN" );
			gMainMenuClass.mFSs[ eMM_FS_BIN ].mpCB = MainMenu_OnSaveBinFS;
			GuiFS_Open( &gMainMenuClass.mFSs[ eMM_FS_BIN ] );
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnLoadMapFS(sGuiFSInfo * apFS)
* ACTION   : MainMenu_OnLoadMapFS
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnLoadMapFS(sGuiFSInfo * apFS)
{
	sString *	lpString;

	if( apFS )
	{
		if( apFS->mButton == eGUIFS_BUTTON_OK )
		{
			lpString = GuiFSInfo_FullNameBuild( apFS );
			EditMenu_MapLoad( lpString->mpChars );
			String_DeInit( lpString );
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnSaveMapFS(sGuiFSInfo * apFS)
* ACTION   : MainMenu_OnSaveMapFS
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnSaveMapFS(sGuiFSInfo * apFS)
{
	sString *	lpString;

	if( apFS )
	{
		if( apFS->mButton == eGUIFS_BUTTON_OK )
		{
			lpString = GuiFSInfo_FullNameBuild( apFS );
			EditMenu_MapSave( lpString->mpChars );
			String_DeInit( lpString );
		}
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnLoadPrjFS(sGuiFSInfo * apFS)
* ACTION   : MainMenu_OnLoadPrjFS
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnLoadPrjFS(sGuiFSInfo * apFS)
{
	sString *	lpString;

	if( apFS )
	{
		if( apFS->mButton == eGUIFS_BUTTON_OK )
		{
			lpString = GuiFSInfo_FullNameBuild( apFS );
			TileMapProject_Load( lpString->mpChars );
			String_DeInit( lpString );
		}
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnSavePrjFS(sGuiFSInfo * apFS)
* ACTION   : MainMenu_OnSavePrjFS
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnSavePrjFS(sGuiFSInfo * apFS)
{
	sString *	lpString;

	if( apFS )
	{
		if( apFS->mButton == eGUIFS_BUTTON_OK )
		{
			lpString = GuiFSInfo_FullNameBuild( apFS );
			TileMapProject_Save( lpString->mpChars );
			String_DeInit( lpString );
		}
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnLoadBinFS(sGuiFSInfo * apFS)
* ACTION   : MainMenu_OnLoadBinFS
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnLoadBinFS(sGuiFSInfo * apFS)
{
	if( apFS )
	{
		if( apFS->mButton == eGUIFS_BUTTON_OK )
		{
		}
	}
	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : void MainMenu_OnSaveBinFS(sGuiFSInfo * apFS)
* ACTION   : MainMenu_OnSaveBinFS
* CREATION : 01.03.2004 PNK
*-----------------------------------------------------------------------------------*/

void MainMenu_OnSaveBinFS(sGuiFSInfo * apFS)
{
	if( apFS )
	{
		if( apFS->mButton == eGUIFS_BUTTON_OK )
		{
		}
	}	
}







/* ################################################################################ */
