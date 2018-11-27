/* ###################################################################################
#  INCLUDES
################################################################################### */


#include	"BUILD.H"
#include	"PLATFORM.H"
#include	"KERNEL.H"

#include	<GODLIB\ASSET\PACKAGE.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>



/* ###################################################################################
#  PROTOTYPES
################################################################################### */

S16	TileEdit_Main( void );


/* ###################################################################################
#  CODE
################################################################################### */


/*-----------------------------------------------------------------------------------*
* FUNCTION : main( void )
* ACTION   : it begins  ...
* CREATION : 21.11.01 PNK
*-----------------------------------------------------------------------------------*/

S16	GodLib_Game_Main( S16 argc, char * argv[] )
{
	(void)argc;
	(void)argv;

	GemDos_Super( 0 );

	printf( "[ Tile Edit ]\n\n(c) 2004 Reservoir Gods\n\n" );

	if( System_GetMON() == MON_MONO )
	{
		printf( "This app requires a colour monitor!\n" );
		GemDos_Cnecin();
	}
#if 0
	else if( System_GetMemory() < 1024 * 1024 )
	{
		printf( "This app requires 1MB of memory!\n" );
		GemDos_Cnecin();
	}
#endif
	else
	{
		TileEdit_Main();
	}

	return( 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : TileEdit_Main( void )
* ACTION   : TileEdit_Main
* CREATION : 09.04.2004 PNK
*-----------------------------------------------------------------------------------*/

S16	TileEdit_Main( void )
{
	Platform_Init();
/*	PackageManager_SetLinkEnableFlag(0);*/
	if( ( System_GetEMU() == EMU_STEEM ) && ( System_GetEmuVersion() < 0x0240 ) )
	{
		printf( "This game requires STEEM version 2.4\n\n" );
		printf( "Please download new version from:\n" );
		printf( "http://steem.atari.org\n" );
		GemDos_Cnecin();
	}
	else
	{
		TileEdit_Kernel_Init();
		TileEdit_Kernel_Main();
		TileEdit_Kernel_DeInit();
	}
	Platform_DeInit();

	return( 0 );
}



/* ################################################################################ */
