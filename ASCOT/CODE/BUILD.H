#ifndef	INCLUDED_BUILD_H
#define	INCLUDED_BUILD_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\BASE\BASE.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

/*#define	dBUILD_MONST*/
/*#define	dBUILD_EXCEPTIONSCREEN_ACTIVE*/
/*#define	dBUILD_STEEMPRINT_ACTIVE*/

#ifdef	dBUILD_STEEMPRINT_ACTIVE

#define	Build_SteemPrint( a )	*(U32*)0xFFFFC1F0L = (U32)a

#define	Build_SteemPrintfLine1( apTxt, apVal0 )									sprintf( gBuildString, apTxt, apVal0 );									Build_SteemPrint( gBuildString )
#define	Build_SteemPrintfLine2( apTxt, apVal0, apVal1 )							sprintf( gBuildString, apTxt, apVal0, apVal1 );							Build_SteemPrint( gBuildString )
#define	Build_SteemPrintfLine3( apTxt, apVal0, apVal1, apVal2 )					sprintf( gBuildString, apTxt, apVal0, apVal1, apVal2 );					Build_SteemPrint( gBuildString )
#define	Build_SteemPrintfLine4( apTxt, apVal0, apVal1, apVal2, apVal3 )			sprintf( gBuildString, apTxt, apVal0, apVal1, apVal2, apVal3 );			Build_SteemPrint( gBuildString )
#define	Build_SteemPrintfLine5( apTxt, apVal0, apVal1, apVal2, apVal3, apVal4 )	sprintf( gBuildString, apTxt, apVal0, apVal1, apVal2, apVal3, apVal4 ); Build_SteemPrint( gBuildString )

#else

#define	Build_SteemPrint( a )

#define	Build_SteemPrintfLine1( apTxt, apVal0 )
#define	Build_SteemPrintfLine2( apTxt, apVal0, apVal1 )
#define	Build_SteemPrintfLine3( apTxt, apVal0, apVal1, apVal2 )
#define	Build_SteemPrintfLine4( apTxt, apVal0, apVal1, apVal2, apVal3 )
#define	Build_SteemPrintfLine5( apTxt, apVal0, apVal1, apVal2, apVal3, apVal4 )

#endif


/* ###################################################################################
#  STRUCTS
################################################################################### */


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

extern	char	gBuildString[ 512 ];

/* ################################################################################ */

#endif	/* INCLUDED_BUILD_H */
