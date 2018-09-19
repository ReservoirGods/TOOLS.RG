#ifndef	INCLUDED_IMG_MENU_H
#define	INCLUDED_IMG_MENU_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>
#include	<GODLIB\HASHTREE\HASHTREE.H>
#include	<GODLIB\PICTYPES\DEGAS.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

#define	dIMGMENU_IMG_LIMIT	16

/* ###################################################################################
#  STRUCTS
################################################################################### */


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	ImageMenu_Init( sHashTree * apTree );
void	ImageMenu_DeInit( void );

sDegas *	ImageMenu_GetpImage( const U16 aImageIndex );
sDegas *	ImageMenu_GetpImageCurrent( void );
U16			ImageMenu_GetImageIndex( void );
char *		ImageMenu_GetpImageName( const U16 aImageIndex );
void		ImageMenu_SetpImageName( const U16 aImageIndex, const char * apFileName );
void		ImageMenu_ImageLoad( const U16 aImageIndex, const char * apFileName );

/* ################################################################################ */

#endif	/* INCLUDED_IMG_MENU_H */
