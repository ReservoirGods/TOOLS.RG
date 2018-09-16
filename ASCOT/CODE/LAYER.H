#ifndef	INCLUDED_LAYER_H
#define	INCLUDED_LAYER_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>

#include	"FRAME.H"
#include	"AFONT.H"


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct
{
	U16			mActiveFlag;
	U16			mFlipX;
	U16			mFlipY;
	U16			mTileX;
	U16			mTileY;
	U16			mColour;
	uS32		mX;
	uS32		mY;
	uS32		mScrollX;
	uS32		mScrollY;
	sFrame *	mpFrame;
} sLayer;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	Layer_Init( sLayer * apLayer );
void	Layer_Update( sLayer * apLayer );
void	Layer_Render( sLayer * apLayer,U16 * apScreen, sFontAscot * apFont );


/* ################################################################################ */

#endif	/* INCLUDED_LAYER_H */
