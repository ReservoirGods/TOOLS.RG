#ifndef	INCLUDED_SAMPLE_H
#define	INCLUDED_SAMPLE_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>
#include	<GODLIB\HASHLIST\HASHLIST.H>
#include	<GODLIB\AUDIO\AUDIO.H>


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sAscotSpl
{
	sHashListItem	mHLI;
	sAudioDmaSound 	mSound;
} sAscotSpl;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void		SampleManager_Init( void );
void		SampleManager_DeInit( void );
sAscotSpl *	SampleManager_Register( const char * apName );
void		SampleManager_UnRegister( sAscotSpl * apSpl );


/* ################################################################################ */

#endif	/* INCLUDED_SAMPLE_H */
