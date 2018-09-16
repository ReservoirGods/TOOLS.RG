#ifndef	INCLUDED_MUSMON_H
#define	INCLUDED_MUSMON_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>
#include	<GODLIB\HASHLIST\HASHLIST.H>


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sMusicMonTune
{
	sHashListItem	mHLI;
	void *			mpTune;
} sMusicMonTune;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	MusicMonManager_Init( void );
void	MusicMonManager_DeInit( void );

sMusicMonTune *	MusicMonManager_Register( const char * apSongName );
void			MusicMonManager_UnRegister( sMusicMonTune * apTune );

void			MusicMonManager_Play( sMusicMonTune * apTune );
void			MusicMonManager_Stop( void );


/* ################################################################################ */

#endif	/* INCLUDED_MUSMON_H */
