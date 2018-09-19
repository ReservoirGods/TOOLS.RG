#ifndef	INCLUDED_EDITMENU_H
#define	INCLUDED_EDITMENU_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>
#include	<GODLIB\HASHTREE\HASHTREE.H>


/* ###################################################################################
#  DEFINES
################################################################################### */


/* ###################################################################################
#  STRUCTS
################################################################################### */


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void				EditMenu_Init( sHashTree * apTree );
void				EditMenu_DeInit( void );

void				EditMenu_MapLoad( char * apFileName );
void				EditMenu_MapSave( char * apFileName );

struct sTileMap *	EditMenu_GetpTileMap( void );

U32					EditMenu_GetMapX( void );
U32					EditMenu_GetMapY( void );
const char *		EditMenu_GetpMapFileName( void );


/* ################################################################################ */

#endif	/* INCLUDED_EDITMENU_H */
