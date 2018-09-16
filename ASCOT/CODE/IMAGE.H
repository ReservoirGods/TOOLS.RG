#ifndef	INCLUDED_IMAGE_H
#define	INCLUDED_IMAGE_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>
#include	<GODLIB\HASHLIST\HASHLIST.H>
#include	<GODLIB\PICTYPES\DEGAS.H>


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sImage
{
	sHashListItem	mHLI;
	sDegas *		mpPicture;
} sImage;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void		ImageManager_Init( void );
void		ImageManager_DeInit( void );
sImage *	ImageManager_Register( const char * apName );
void		ImageManager_UnRegister( sImage * apImage );


/* ################################################################################ */

#endif	/* INCLUDED_IMAGE_H */
