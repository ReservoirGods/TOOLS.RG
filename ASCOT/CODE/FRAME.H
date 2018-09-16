#ifndef	INCLUDED_FRAME_H
#define	INCLUDED_FRAME_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>
#include	<GODLIB\HASHLIST\HASHLIST.H>


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct
{
	sHashListItem	mHLI;
	U32				mHashName;
	U16				mWidth;
	U16				mHeight;
	U8				mAlphaChar;
	U8 *			mpText;
} sFrame;


typedef	struct
{
	U32			mFrameCount;
	sFrame *	mpFrames;
} sFrameReel;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void		FrameManager_Init( void );
void		FrameManager_DeInit( void );
sFrame *	FrameManager_GetpFrame( const char * apText );


/* ################################################################################ */

#endif	/* INCLUDED_FRAME_H */
