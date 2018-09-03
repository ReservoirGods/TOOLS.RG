#include	<windows.h>
#include	"conditions.h"
#include	"ExpressionEvaluator.H"
#include	"steemh.h"

namespace	ExpressionEvaluator
{

	typedef enum eEE_Register
	{
		eEE_Register_A0,
		eEE_Register_A1,
		eEE_Register_A2,
		eEE_Register_A3,
		eEE_Register_A4,
		eEE_Register_A5,
		eEE_Register_A6,
		eEE_Register_A7,

		eEE_Register_D0,
		eEE_Register_D1,
		eEE_Register_D2,
		eEE_Register_D3,
		eEE_Register_D4,
		eEE_Register_D5,
		eEE_Register_D6,
		eEE_Register_D7,

		eEE_Register_CCR,
		eEE_Register_SSP,
		eEE_Register_SR,
		eEE_Register_USP,

	}eEE_Register;

	typedef struct sEE_Register_Info
	{
		const char * mpName;
		eEE_Register	mRegister;
	} sEE_Register_Info;

	sEE_Register_Info mBkptRegisterInfos[]=
	{
		{"a0", eEE_Register_A0 },
		{"a1", eEE_Register_A1 },
		{"a2", eEE_Register_A2 },
		{"a3", eEE_Register_A3 },
		{"a4", eEE_Register_A4 },
		{"a5", eEE_Register_A5 },
		{"a6", eEE_Register_A6 },
		{"a7", eEE_Register_A7 },

		{"d0",eEE_Register_D0 },
		{"d1",eEE_Register_D1 },
		{"d2",eEE_Register_D2 },
		{"d3",eEE_Register_D3 },
		{"d4",eEE_Register_D4 },
		{"d5",eEE_Register_D5 },
		{"d6",eEE_Register_D6 },
		{"d7",eEE_Register_D7 },

		{"ccr",eEE_Register_CCR },
		{"ssp",eEE_Register_SSP },
		{"sr",eEE_Register_SR },
		{"usp",eEE_Register_USP },

	};

	int    EE_68K_GetRegisterCount( void )
	{
		return( sizeof(mBkptRegisterInfos) / sizeof( mBkptRegisterInfos[ 0 ] ) );
	}

	char	EE_68K_GetCharLower( char aChar )
	{
		if( (aChar >= 'A') && (aChar <= 'Z') )
		{
			return( aChar - ('A'-'a') );
		}
		return( aChar );
	}

	int		EE_68K_Register_GetValue( const struct sEE_Token * apToken )
	{
		switch( apToken->mValue )
		{
		case  eEE_Register_A0:
			return( r[8] );
			break;
		case  eEE_Register_A1:
			return( r[9] );
			break;
		case  eEE_Register_A2:
			return( r[10] );
			break;
		case  eEE_Register_A3:
			return( r[11] );
			break;
		case  eEE_Register_A4:
			return( r[12] );
			break;
		case  eEE_Register_A5:
			return( r[13] );
			break;
		case  eEE_Register_A6:
			return( r[14] );
			break;
		case  eEE_Register_A7:
			return( r[15] );
			break;

		case  eEE_Register_D0:
			return( r[0] );
			break;
		case  eEE_Register_D1:
			return( r[1] );
			break;
		case  eEE_Register_D2:
			return( r[2] );
			break;
		case  eEE_Register_D3:
			return( r[3] );
			break;
		case  eEE_Register_D4:
			return( r[4] );
			break;
		case  eEE_Register_D5:
			return( r[5] );
			break;
		case  eEE_Register_D6:
			return( r[6] );
			break;
		case  eEE_Register_D7:
			return( r[7] );
			break;

		case  eEE_Register_CCR:
			return( sr & 0xFF );
			break;
		case  eEE_Register_SSP:
			return( r[15] );
			break;
		case  eEE_Register_SR:
			return( sr );
			break;
		case  eEE_Register_USP:
			return( r[15] );
			break;
		}
		return( 0 );
	}


	const char * EE_68K_ValueGetter( struct sEE_Token * apToken, const char * apName )
	{
		int i;

		for( i=0; i<EE_68K_GetRegisterCount(); i++ )
		{
			int j = 0;
			const sEE_Register_Info * lpInfo = &mBkptRegisterInfos[ i ];
			while( lpInfo->mpName[ j ] && ( lpInfo->mpName[ j ] == EE_68K_GetCharLower( apName[ j ] ) ) )
			{
				  j++;
			}
			if( 0 == lpInfo->mpName[ j ] )
			{
				  apToken->mValue = lpInfo->mRegister;
				  apToken->mfGetValue =EE_68K_Register_GetValue;
				  return( &apName[ j ] );
			}
		}

		return( 0 );
	}

	void  EE_68K_Init( void )
	{
		gfBkptValueGetterBuild = EE_68K_ValueGetter;
	}

	void  EE_68K_DeInit( void )
	{
	}

	void  EE_Expression_68K_Compile( const char * apString, struct sEE_Expression * apExpression )
	{
		gfBkptValueGetterBuild = EE_68K_ValueGetter;
		EE_Expression_Compile( apString, apExpression );
	}


}