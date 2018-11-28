/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"MUSMON.H"

#include	<GODLIB\ASSET\RELOCATE.H>
#include	<GODLIB\AUDIO\AUDIO.H>
#include	<GODLIB\VBL\VBL.H>


/* ###################################################################################
#  DATA
################################################################################### */

sHashList	gMusicMonList;
sRelocater 	gMusicMonRelocater;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	MusicMon_ItemInit( void * apItem );

U32	MusicMon_RelocIsType( sAsset * apAsset );
U32	MusicMon_RelocDoInit( sAsset * apAsset );
U32	MusicMon_RelocDoDeInit( sAsset * apAsset );

extern	void	MusicMon_Start( void * apSong );
extern	void	MusicMon_Stop( void );
extern	void	MusicMon_Vbl( void );

U16	gMusicMonPat;
U16	gMusicMonPos;


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMonManager_Init( void )
* ACTION   : MusicMonManager_Init
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	MusicMonManager_Init( void )
{
	HashList_Init( &gMusicMonList, sizeof(sMusicMonTune), MusicMon_ItemInit, 0 );
	MusicMonManager_Stop();

	Relocater_Init( &gMusicMonRelocater, "MOD", MusicMon_RelocIsType, MusicMon_RelocDoInit, MusicMon_RelocDoDeInit, 0, 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMonManager_DeInit( void )
* ACTION   : MusicMonManager_DeInit
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	MusicMonManager_DeInit( void )
{
	Relocater_DeInit( &gMusicMonRelocater );

	Vbl_RemoveCall( MusicMon_Vbl );
	MusicMonManager_Stop();
	HashList_DeInit( &gMusicMonList );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMonManager_Register( const char * apSongName )
* ACTION   : MusicMonManager_Register
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

sMusicMonTune *	MusicMonManager_Register( const char * apSongName )
{
	return( (sMusicMonTune*)HashList_ItemRegister( &gMusicMonList, HashList_BuildHash(apSongName) ) );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMonManager_UnRegister( sMusicMonTune * apTune )
* ACTION   : MusicMonManager_UnRegister
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	MusicMonManager_UnRegister( sMusicMonTune * apTune )
{
	if( apTune )
	{
		HashList_ItemUnRegister( &gMusicMonList, apTune->mHLI.mID );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMonManager_Play( sMusicMonTune * apTune )
* ACTION   : MusicMonManager_Play
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	MusicMonManager_Play( sMusicMonTune * apTune )
{
	if( apTune )
	{
		if( apTune->mpTune )
		{
			Vbl_RemoveCall( MusicMon_Vbl );
			MusicMon_Start( apTune->mpTune );
			Vbl_AddCall( MusicMon_Vbl );
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMonManager_Stop( void )
* ACTION   : MusicMonManager_Stop
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	MusicMonManager_Stop( void )
{
	Vbl_RemoveCall( MusicMon_Vbl );
	MusicMon_Stop();	
	Audio_SoundChipOff();
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMon_ItemInit( void * apItem )
* ACTION   : MusicMon_ItemInit
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	MusicMon_ItemInit( void * apItem )
{
	sMusicMonTune *	lpTune;
	
	lpTune = (sMusicMonTune*)apItem;
	if( lpTune )
	{
		lpTune->mpTune = 0;
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMon_RelocIsType( sAsset * apAsset )
* ACTION   : MusicMon_RelocIsType
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	MusicMon_RelocIsType( sAsset * apAsset )
{
	(void)apAsset;
	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMon_RelocDoInit( sAsset * apAsset )
* ACTION   : MusicMon_RelocDoInit
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	MusicMon_RelocDoInit( sAsset * apAsset )
{
	sMusicMonTune *	lpTune;

	lpTune = (sMusicMonTune*)HashList_ItemRegister( &gMusicMonList, apAsset->mID );

	if( lpTune )
	{
		lpTune->mpTune = apAsset->mpData;
	}

	return( (lpTune != 0) );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : MusicMon_RelocDoDeInit( sAsset * apAsset )
* ACTION   : MusicMon_RelocDoDeInit
* CREATION : 11.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	MusicMon_RelocDoDeInit( sAsset * apAsset )
{
	HashList_ItemUnRegister( &gMusicMonList, apAsset->mID );

	return( 1 );
}

#ifdef	WIN32

void	MusicMon_Start( void * apSong ) { (void)apSong;  }
void	MusicMon_Stop( void ){}
void	MusicMon_Vbl( void ){}

#endif


/* ################################################################################ */
