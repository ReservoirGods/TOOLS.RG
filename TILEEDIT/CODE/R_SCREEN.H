#ifndef	INCLUDED_R_SCREEN_H
#define	INCLUDED_R_SCREEN_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>

#include	<GODLIB\GRAPHIC\GRAPHIC.H>


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	RenderScreen_Init( void );
void	RenderScreen_Update( void );
void	RenderScreen_DeInit( void );
 
U16 *	RenderScreen_GetpPhysic( void );
U16 *	RenderScreen_GetpLogic( void );
U16 *	RenderScreen_GetpBack( void );
U16 *	RenderScreen_GetpScreenBuffer( const U16 aIndex );

U16		RenderScreen_GetPhysicIndex( void );
U16		RenderScreen_GetLogicIndex( void );

void	RenderScreen_ClearScreen( U16 * apScreen );
void	RenderScreen_CopyScreen( U16 * apSrc, U16 * apDst );

void	RenderScreen_SetFrameRate( U32 aRate );
void	RenderScreen_SetResolution( void );

void	RenderScreen_SetNTSCFlag( const U8 aFlag );
void	RenderScreen_SetWidescreenFlag( const U8 aFlag );

void	RenderScreen_SetVerticalOffset( const S16 aOffY );
void	RenderScreen_SetHorizontalOffset( const S16 aOffX );

extern	U32				gRScreenLineOffsets[ 200 ];
extern	sGraphicCanvas	gRScreenGraphic;
extern	sGraphicCanvas	gRScreenBackGraphic;

#define	RenderScreen_GetpGraphic()	(&gRScreenGraphic)
#define	RenderScreen_GetpBackGraphic()	(&gRScreenBackGraphic)

/* ################################################################################ */

#endif	/*	INCLUDED_R_SCREEN_H	*/