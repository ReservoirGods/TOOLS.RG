/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"FRAME.H"

#include	"BUILD.H"
#include	"HASH.H"
#include	"TOKENISE.H"

#include	<GODLIB\MEMORY\MEMORY.H>
#include	<GODLIB\HASHLIST\HASHLIST.H>
#include	<GODLIB\ASSET\RELOCATE.H>

#include	<STRING.H>


/* ###################################################################################
#  DATA
################################################################################### */

sHashList	gFrameList;
sRelocater	gFrameRelocater;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	Frame_ParseLoad(   const char * apText,const S32 aSize );
void	Frame_ParseUnLoad( const char * apText,const S32 aSize );

U32	Frame_RelocIsType( void * apData, const U32 aSize, const U32 aID );
U32	Frame_RelocDoInit( void * apData, const U32 aSize, const U32 aID );
U32	Frame_RelocDoDeInit( void * apData, const U32 aSize, const U32 aID );

void	Frame_ItemInit( void * apItem );
void	Frame_ItemDeInit( void * apItem );


/* ###################################################################################
#  CODE
################################################################################### */


/*-----------------------------------------------------------------------------------*
* FUNCTION : FrameManager_Init( void )
* ACTION   : FrameManager_Init
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	FrameManager_Init( void )
{
	HashList_Init( &gFrameList, sizeof( sFrame ), Frame_ItemInit, Frame_ItemDeInit );	
	Relocater_Init( &gFrameRelocater, "FRM", Frame_RelocIsType, Frame_RelocDoInit, Frame_RelocDoDeInit, 0, 0 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : FrameManager_DeInit( void )
* ACTION   : FrameManager_DeInit
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	FrameManager_DeInit( void )
{
	HashList_DeInit( &gFrameList );	
	Relocater_DeInit( &gFrameRelocater );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : FrameManager_GetpFrame( const char * apText )
* ACTION   : FrameManager_GetpFrame
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

sFrame *	FrameManager_GetpFrame( const char * apText )
{
	return( (sFrame*)HashList_ItemFind( &gFrameList, HashList_BuildHash(apText) ) );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Frame_ItemInit( void * apItem )
* ACTION   : Frame_ItemInit
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Frame_ItemInit( void * apItem )
{
	sFrame *	lpFrame;
	
	lpFrame = (sFrame*)apItem;

	lpFrame->mAlphaChar = ' ';
	lpFrame->mHashName  = 0;
	lpFrame->mHeight    = 0;
	lpFrame->mpText     = 0;
	lpFrame->mWidth     = 0;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Frame_ItemDeInit( void * apItem )
* ACTION   : Frame_ItemDeInit
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Frame_ItemDeInit( void * apItem )
{
	sFrame *	lpFrame;
		
	lpFrame = (sFrame*)apItem;
	if( lpFrame->mpText )
	{
		mMEMFREE( lpFrame->mpText );
		lpFrame->mpText = 0;
	}
}



/*-----------------------------------------------------------------------------------*
* FUNCTION : Frame_RelocIsType( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Frame_RelocIsType
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Frame_RelocIsType( void * apData,const U32 aSize,const U32 aID )
{
	(void)apData;
	(void)aSize;
	(void)aID;

	return( 1 );	
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Frame_RelocDoInit( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Frame_RelocDoInit
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Frame_RelocDoInit( void * apData,const U32 aSize,const U32 aID )
{
	(void)aID;

	Frame_ParseLoad( (char*)apData, aSize );
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Frame_RelocDoDeInit( void * apData,const U32 aSize,const U32 aID )
* ACTION   : Frame_RelocDoDeInit
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Frame_RelocDoDeInit( void * apData,const U32 aSize,const U32 aID )
{
	(void)aID;
	Frame_ParseUnLoad( (char*)apData, aSize );
	return( 1 );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Frame_ParseLoad( const char * apText,const S32 aSize )
* ACTION   : Frame_ParseLoad
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Frame_ParseLoad( const char * apText,const S32 aSize )
{
	sArgs		lArgs;
	S32			lOffset;
	S32			lFrameOffset = 0;
	U16			lMode;
	U16			lWidth = 0;
	U16			lHeight = 0;
	U16			i;
	sFrame *	lpFrame = 0;
	char *		lpText = 0;


	lOffset = 0;
	lMode   = 0;
	lWidth  = 80;

	while( lOffset < aSize )
	{
		lOffset += Args_Build( &lArgs, &apText[ lOffset ] );

		switch( lMode )
		{
		case	0:
			if( lArgs.mArgCount >= 4 )
			{
				if( ( 0 == (strcmp( lArgs.mpArgs[ 0 ], "["    ) ) ) &&
					( 0 == (strcmp( lArgs.mpArgs[ 1 ], "FRAME") ) )
					)
				{
					lpFrame = (sFrame*)HashList_ItemRegister( &gFrameList, HashList_BuildHash(lArgs.mpArgs[ 2 ]) );

					lMode = 1;

				}
				else if( ( 0 == (strcmp( lArgs.mpArgs[ 0 ], "["    ) ) ) &&
					( 0 == (strcmp( lArgs.mpArgs[ 1 ], "WIDTH") ) )
					)
				{
					lWidth = (U16)String_GrabNumber( lArgs.mpArgs[ 2 ] );
				}

			}
			break;

		case	1:
			if(	(lArgs.mArgCount == 1) && (0 == (strcmp(lArgs.mpArgs[0],"{") ) ) )
			{
				lHeight         = 0;
				lFrameOffset    = lOffset;
				lpFrame->mWidth = lWidth;
				lMode           = 2;
			}
			break;

		case	2:
			if(	(lArgs.mArgCount == 1) && (0 == (strcmp(lArgs.mpArgs[0],"}") ) ) )
			{
				lpFrame->mHeight = 0;
				lpText           = mMEMCALLOC( lWidth * lHeight );
				lpFrame->mpText  = (U8*)lpText;
				lOffset          = lFrameOffset;
				lMode            = 3;

			}
			else
			{
				lHeight++;

				if( lArgs.mLineLen > lpFrame->mWidth )
				{
					lpFrame->mWidth = (U16)lArgs.mLineLen;
				}
			}
			break;

		case	3:
			if(	(lArgs.mArgCount == 1) && (0 == (strcmp(lArgs.mpArgs[0],"}") ) ) )
			{
				lMode = 0;
			}
			else
			{
				Build_SteemPrintfLine1( "FrameReel_Parse() : Insert Line: '%s'", lArgs.mRawSpace );
				for( i=0; i<lpFrame->mWidth; i++ )
				{
					if( i < lArgs.mLineLen )
					{
						if( 
							(lArgs.mRawSpace[ i ] == 0xA) ||
							(lArgs.mRawSpace[ i ] == 0xD)
							)
						{
							lpText[ i ] = ' ';
						}
						else
						{
							lpText[ i ] = lArgs.mRawSpace[ i ];
						}
					}
					else
					{
						lpText[ i ] = ' ';
					}
				}

				lpText += lpFrame->mWidth;
				lpFrame->mHeight++;
			}
			break;
	
		}
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Frame_ParseUnLoad( const char * apText,const S32 aSize )
* ACTION   : Frame_ParseUnLoad
* CREATION : 10.12.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Frame_ParseUnLoad( const char * apText,const S32 aSize )
{
	sArgs		lArgs;
	S32			lOffset;
	U16			lMode;


	lOffset = 0;
	lMode   = 0;

	while( lOffset < aSize )
	{
		lOffset += Args_Build( &lArgs, &apText[ lOffset ] );

		switch( lMode )
		{
		case	0:
			if( lArgs.mArgCount >= 4 )
			{
				if( ( 0 == (strcmp( lArgs.mpArgs[ 0 ], "["    ) ) ) &&
					( 0 == (strcmp( lArgs.mpArgs[ 1 ], "FRAME") ) )
					)
				{
					HashList_ItemUnRegister( &gFrameList, HashList_BuildHash(lArgs.mpArgs[ 2 ]) );

					lMode = 1;

				}
			}
			break;

		case	1:
			if(	(lArgs.mArgCount == 1) && (0 == (strcmp(lArgs.mpArgs[0],"{") ) ) )
			{
				lMode           = 2;
			}
			break;

		case	2:
			if(	(lArgs.mArgCount == 1) && (0 == (strcmp(lArgs.mpArgs[0],"}") ) ) )
			{
				lMode            = 0;

			}
			break;
		}

	}
	
}


/* ################################################################################ */
