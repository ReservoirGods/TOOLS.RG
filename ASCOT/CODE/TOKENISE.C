/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"TOKENISE.H"


/* ###################################################################################
#  ENUMS
################################################################################### */

enum
{
	eARGS_MODE_SCAN,
	eARGS_MODE_COPY,
	eARGS_MODE_CLIP,
	eARGS_MODE_END,
};

enum
{
	eARGS_TYPE_TEXT,
	eARGS_TYPE_WHITESPACE,
	eARGS_TYPE_RETURN,
	eARGS_TYPE_NULL,
};


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

U32		Args_Build( sArgs * apArgs,const char * apString );
U16		Args_GetType( const char aChar );
void	Args_Debug( sArgs * apArgs );
U8		Char_IsReturn( const char aChar );


/* ###################################################################################
#  CODE
################################################################################### */


/*-----------------------------------------------------------------------------------*
* FUNCTION : Args_Build( sArgs * apArgs,const char * apString )
* ACTION   : Args_Build
* CREATION : 21.01.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	Args_Build( sArgs * apArgs,const char * apString )
{
	U32	lOffset;
	U32	lLast;
	U16	lMode;
	U16	lType;
	U16	lDstIndex;

/*	Build_SteemPrint( "Args_Build()" );*/

	apArgs->mArgCount = 0;

	lDstIndex = 0;
	lOffset   = 0;
	lLast     = 1;
	lMode     = eARGS_MODE_SCAN;

	apArgs->mLineLen = 0;
	apArgs->mRawSpace[ 0 ] = 0;

	while( lMode != eARGS_MODE_END )
	{
		lType = Args_GetType( apString[ lOffset ] );

		if( (lLast != lOffset) && (lMode != eARGS_MODE_CLIP) )
		{
			lLast = lOffset;
			if( lLast < dARGS_SPACE_LIMIT )
			{
				apArgs->mRawSpace[ lLast   ] = apString[ lOffset ];
				apArgs->mRawSpace[ lLast+1 ] = 0;
				apArgs->mLineLen++;
			}
		}
/*		Build_SteemPrintfLine3( "Mode %d Offset %ld Type %d", lMode, lOffset, lType );*/
		switch( lMode )
		{

		case	eARGS_MODE_SCAN:
			switch( lType )
			{
			case	eARGS_TYPE_TEXT:
				apArgs->mpArgs[ apArgs->mArgCount ] = &apArgs->mArgSpace[ lDstIndex ];
				apArgs->mArgCount++;
				lMode = eARGS_MODE_COPY;
				break;
			case	eARGS_TYPE_WHITESPACE:
				lOffset++;
				break;
			case	eARGS_TYPE_RETURN:
				lMode = eARGS_MODE_CLIP;
				lOffset++;
				break;
			case	eARGS_TYPE_NULL:
				apArgs->mArgSpace[ lDstIndex++ ] = 0;
				lMode = eARGS_MODE_END;
				lOffset++;
				break;
			}
			break;

		case	eARGS_MODE_COPY:
			switch( lType )
			{
			case	eARGS_TYPE_TEXT:
				apArgs->mArgSpace[ lDstIndex++ ] = apString[ lOffset ];
				lOffset++;
				break;
			default:
				apArgs->mArgSpace[ lDstIndex++ ] = 0;
				lMode = eARGS_MODE_SCAN;
				break;
			}
			break;

		case	eARGS_MODE_CLIP:
			if( eARGS_TYPE_RETURN != lType )
			{
				lMode = eARGS_MODE_END;
			}
			else
			{
				lOffset++;
			}
			break;

		}
	}

	Args_Debug( apArgs );
	return( lOffset );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Args_GetType( const char aChar )
* ACTION   : Args_GetType
* CREATION : 21.01.2003 PNK
*-----------------------------------------------------------------------------------*/

U16	Args_GetType( const char aChar )
{
	U16	lType;

	if( !aChar )
	{
		lType = eARGS_TYPE_NULL;
	}
	else if( (aChar==0xA) || (aChar==0xD) )
	{
		lType = eARGS_TYPE_RETURN;
	}
	else if( (aChar <= ' ') || (aChar == ',') )
	{
		lType = eARGS_TYPE_WHITESPACE;
	}
	else
	{
		lType = eARGS_TYPE_TEXT;
	}

	return( lType );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Args_Debug( sArgs * apArgs )
* ACTION   : Args_Debug
* CREATION : 22.01.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Args_Debug( sArgs * apArgs )
{
	U16	i;

/*	Build_SteemPrintfLine1( "Args count %d", apArgs->mArgCount );*/
	for( i=0; i<apArgs->mArgCount; i++ )
	{
/*		Build_SteemPrintfLine2( " %d) %s",i, apArgs->mpArgs[ i ] );*/
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Char_IsReturn( const char aChar )
* ACTION   : Char_IsReturn
* CREATION : 22.01.2003 PNK
*-----------------------------------------------------------------------------------*/

U8	Char_IsReturn( const char aChar )
{
	U8	lRes;
	
	if( (0xD == aChar) || (0xA == aChar) )
	{
		lRes = 1;
	}
	else
	{
		lRes = 0;
	}

	return( lRes );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : String_GrabNumber( const char * apTxt )
* ACTION   : String_GrabNumber
* CREATION : 22.01.2003 PNK
*-----------------------------------------------------------------------------------*/

S32	String_GrabNumber( const char * apTxt )
{
	S32	lVal;
	S32	lSign;
	U8	lMode;

	while( *apTxt == ' ' )
	{
		apTxt++;
	}

	if( *apTxt == '-' )
	{
		lSign = -1;
		apTxt++;
	}
	else
	{
		lSign = 1;
	}

	lMode = 0;
	if( *apTxt == '$' )
	{
		lMode = 1;
		apTxt++;
	}
	if( apTxt[ 1 ] == 'x' )
	{
		lMode = 1;
		apTxt += 2;
	}

	lVal = 0;
	if( lMode )
	{
		while( 
				( (*apTxt >= '0') && (*apTxt <= '9') )
			||	( (*apTxt >= 'a') && (*apTxt <= 'f') )
			||	( (*apTxt >= 'A') && (*apTxt <= 'F') )
			)
		{
			lVal *= 16L;
			if( (*apTxt >= '0') && (*apTxt <= '9') )
			{
				lVal += *apTxt - '0';
			}
			else if( (*apTxt >= 'a') && (*apTxt <= 'f') )
			{
				lVal += (*apTxt - 'a')+10;
			}
			else if( (*apTxt >= 'A') && (*apTxt <= 'F') )
			{
				lVal += (*apTxt - 'A')+10;
			}
			apTxt++;
		}
	}
	else
	{
		while( (*apTxt >= '0') && (*apTxt <= '9') )
		{
			lVal *= 10L;
			lVal += *apTxt - '0';
			apTxt++;
		}
	}

	lVal *= lSign;
	return( lVal );
}


/* ################################################################################ */
