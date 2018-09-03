#include    "ExpressionEvaluator.H"

namespace	ExpressionEvaluator
{

	int    EE_Evaluate_UnaryMinus( int aValue0, int aValue1 )
	{
		return( -aValue0 );
	}

	int    EE_Evaluate_BitwiseNot( int aValue0, int aValue1 )
	{
		return( ~aValue0 );
	}

	int    EE_Evaluate_LogicalNot( int aValue0, int aValue1 )
	{
		int lRes = ( 0 == aValue0 ) ? 1 : 0;
		return( lRes );
	}

	int    EE_Evaluate_Multiply( int aValue0, int aValue1 )
	{
		return( aValue0 * aValue1 );
	}

	int    EE_Evaluate_Divide( int aValue0, int aValue1 )
	{
		return( aValue0 / aValue1 );
	}

	int    EE_Evaluate_Modulus( int aValue0, int aValue1 )
	{
		return( aValue0 % aValue1 );
	}

	int    EE_Evaluate_Plus( int aValue0, int aValue1 )
	{
		return( aValue0 + aValue1 );
	}

	int    EE_Evaluate_Minus( int aValue0, int aValue1 )
	{
		return( aValue0 - aValue1 );
	}

	int    EE_Evaluate_ShiftLeft( int aValue0, int aValue1 )
	{
		return( aValue0 << aValue1 );
	}

	int    EE_Evaluate_ShiftRight( int aValue0, int aValue1 )
	{
		return( aValue0 >> aValue1 );
	}

	int EE_Evaluate_Greater( int aValue0, int aValue1 )
	{
		int lRes = ( aValue0 > aValue1 ) ? 1 : 0;
		return( lRes );
	}

	int EE_Evaluate_GreaterEqual( int aValue0, int aValue1 )
	{
		int lRes = ( aValue0 >= aValue1 ) ? 1 : 0;
		return( lRes );
	}

	int EE_Evaluate_Less( int aValue0, int aValue1 )
	{
		int lRes = ( aValue0 < aValue1 ) ? 1 : 0;
		return( lRes );
	}

	int EE_Evaluate_LessEqual( int aValue0, int aValue1 )
	{
		int lRes = ( aValue0 <= aValue1 ) ? 1 : 0;
		return( lRes );
	}

	int EE_Evaluate_Equal( int aValue0, int aValue1 )
	{
		int lRes = ( aValue0 == aValue1 ) ? 1 : 0;
		return( lRes );
	}

	int EE_Evaluate_NotEqual( int aValue0, int aValue1 )
	{
		int lRes = ( aValue0 != aValue1 ) ? 1 : 0;
		return( lRes );
	}

	int EE_Evaluate_BitwiseAnd( int aValue0, int aValue1 )
	{
		return( aValue0 & aValue1 );
	}

	int EE_Evaluate_BitwiseOr( int aValue0, int aValue1 )
	{
		return( aValue0 | aValue1 );
	}

	int EE_Evaluate_BitwiseXor( int aValue0, int aValue1 )
	{
		return( aValue0 ^ aValue1 );
	}

	int EE_Evaluate_LogicalAnd( int aValue0, int aValue1 )
	{
		int lRes = ( (0!=aValue0) && (0!=aValue1) ) ? 1 : 0;
		return( lRes );
	}

	int EE_Evaluate_LogicalOr( int aValue0, int aValue1 )
	{
		int lRes = ( (0!=aValue0) || (0!=aValue1) ) ? 1 : 0;
		return( lRes );
	}


	sEE_Operator      mEE_Operators[] =
	{
		{ "_", 3,	1, eEE_Association_Right, EE_Evaluate_UnaryMinus	},
		{ "!", 3,	1, eEE_Association_Right, EE_Evaluate_LogicalNot	},
		{ "~", 3,	1, eEE_Association_Right, EE_Evaluate_BitwiseNot	},

		{ "*", 5,	2, eEE_Association_Left, EE_Evaluate_Multiply		},
		{ "/", 5,	2, eEE_Association_Left, EE_Evaluate_Divide			},
		{ "%", 5,	2, eEE_Association_Left, EE_Evaluate_Modulus		},

		{ "+",	6,	2, eEE_Association_Left, EE_Evaluate_Plus			},
		{ "-",	6,	2, eEE_Association_Left, EE_Evaluate_Minus			},

		{ "<<",	6,	2, eEE_Association_Left, EE_Evaluate_ShiftLeft		},
		{ ">>",	6,	2, eEE_Association_Left, EE_Evaluate_ShiftRight		},

		{ ">",	8,	2, eEE_Association_Left, EE_Evaluate_Greater		},
		{ ">=",	8,	2, eEE_Association_Left, EE_Evaluate_GreaterEqual	},
		{ "<",	8,	2, eEE_Association_Left, EE_Evaluate_Less			},
		{ "<=",	8,	2, eEE_Association_Left, EE_Evaluate_LessEqual		},

		{ "==",	9,	2, eEE_Association_Left, EE_Evaluate_Equal			},
		{ "!=",	9,	2, eEE_Association_Left, EE_Evaluate_NotEqual		},

		{ "&",	10,	2, eEE_Association_Left, EE_Evaluate_BitwiseAnd		},
		{ "^",	11,	2, eEE_Association_Left, EE_Evaluate_BitwiseXor		},
		{ "|",	12,	2, eEE_Association_Left, EE_Evaluate_BitwiseOr		},

		{ "&&",	13,	2, eEE_Association_Left, EE_Evaluate_LogicalAnd		},
		{ "||",	14,	2, eEE_Association_Left, EE_Evaluate_LogicalOr		},

		{ "(",	13,	0, eEE_Association_Left, 0							},
		{ ")",	14,	0, eEE_Association_Left, 0							},

	};


	typedef enum eEE_Token_Type
	{
		eEE_Token_Type_None,
		eEE_Token_Type_Number,
		eEE_Token_Type_Operator,
	} eEE_Token_Type;



	int    EE_Token_GetValue( const sEE_Token * apToken )
	{
		return( apToken->mValue );
	}

	/*
	overridable function, allows you to add your own getter (for register values for example)
	*/
	const char * (*gfBkptValueGetterBuild)( struct sEE_Token * apToken, const char * apName ) = 0;


	typedef struct sLineParser
	{
		const char *	mpString;
		int				mOffset;
	} sLineParser;


	int    EE_GetOperatorCount( void )
	{
		return( sizeof(mEE_Operators) / sizeof( mEE_Operators[ 0 ] ) );
	}


	const sEE_Operator * EE_Operator_Find( sLineParser * apParser )
	{
		char lChar = apParser->mpString[ apParser->mOffset ];
		char lCharNext = apParser->mpString[ apParser->mOffset + 1 ];

		const sEE_Operator * lpBestOp = 0;

		int i;
		for( i=0; i<EE_GetOperatorCount(); i++ )
		{
			const sEE_Operator * lpOp = &mEE_Operators[ i ];
			if( lpOp->mpName[ 0 ] == apParser->mpString[ apParser->mOffset ] )
			{
				if( 0 == lpOp->mpName[ 1 ] )
				{
					lpBestOp = lpOp;
				}
				if( lpOp->mpName[ 1 ] == apParser->mpString[ apParser->mOffset + 1 ] )
				{
					apParser->mOffset += 2;
					return( lpOp );
				}
			}
		}

		if( lpBestOp )
		{
			apParser->mOffset++;
		}
		return( lpBestOp );
	}


	int EE_GetCharValueHex( char aChar )
	{
		if( (aChar >= '0') && (aChar <= '9') )
		{
			return( aChar - '0' );
		}
		if( (aChar >= 'a') && (aChar <= 'f') )
		{
			return( ( aChar - 'a') + 0xA );
		}
		if( (aChar >= 'A') && (aChar <= 'F') )
		{
			return( ( aChar - 'A') + 0xA );
		}
		return( -1 );
	}

	enum eEE_Evaluation_Stack
	{
		eEE_Evaulation_Stack_LIMIT = 64
	};


	typedef struct sEE_Evaluation
	{
		int						mNumberStackOffset;
		int						mNumberStack[ eEE_Evaulation_Stack_LIMIT ];
		int						mOpStackOffset;
		const sEE_Operator *	mpOperatorStack[ eEE_Evaulation_Stack_LIMIT ];

	} sEE_Evaluation;


	void  EE_Evaluation_Opstack_Push( sEE_Evaluation * apEvaluation, const sEE_Operator * apOp )
	{
		if( apEvaluation->mOpStackOffset < eEE_Evaulation_Stack_LIMIT )
		{
			apEvaluation->mpOperatorStack[ apEvaluation->mOpStackOffset ] = apOp;
			apEvaluation->mOpStackOffset++;
		}
	}


	const sEE_Operator *    EE_Evaluation_Opstack_Pop( sEE_Evaluation * apEvaluation )
	{
		const sEE_Operator * lpOp = 0;
		if( apEvaluation->mOpStackOffset > 0 )
		{
			apEvaluation->mOpStackOffset--;
			lpOp = apEvaluation->mpOperatorStack[ apEvaluation->mOpStackOffset ];
		}
		return( lpOp );
	}


	void  EE_Evaluation_Numstack_Push( sEE_Evaluation * apEvaluation, int aNumber )
	{
		if( apEvaluation->mNumberStackOffset < eEE_Evaulation_Stack_LIMIT )
		{
			apEvaluation->mNumberStack[ apEvaluation->mNumberStackOffset ] = aNumber;
			apEvaluation->mNumberStackOffset++;
		}
	}


	int    EE_Evaluation_Numstack_Pop( sEE_Evaluation * apEvaluation )
	{
		int lNumber = 0;
		if( apEvaluation->mNumberStackOffset > 0 )
		{
			apEvaluation->mNumberStackOffset--;
			lNumber = apEvaluation->mNumberStack[ apEvaluation->mNumberStackOffset ];
		}
		return( lNumber );
	}


	void  EE_Expression_Tokenstack_Push( sEE_Expression * apExpression, sEE_Token * apToken )
	{
		if( apExpression->mTokenStackOffset < eEE_Expression_Stack_LIMIT )
		{
			apExpression->mTokenStack[ apExpression->mTokenStackOffset ] = *apToken;
			apExpression->mTokenStackOffset++;
		}
	}

	void  EE_Expression_Op_Perform( sEE_Evaluation * apEvaluation )
	{
		const sEE_Operator * lpOp = 0;

		lpOp = EE_Evaluation_Opstack_Pop( apEvaluation );
		if( lpOp->mOperandCount )
		{
			int lN0 = 0;
			int lN1 = 0;
			int lResult = 0;

			lN0 = EE_Evaluation_Numstack_Pop( apEvaluation );

			if( lpOp->mOperandCount > 1 )
			{
				lN1 = lN0;
				lN0 = EE_Evaluation_Numstack_Pop( apEvaluation );
			}

			lResult = lpOp->mfEvaluator( lN0, lN1 );
			EE_Evaluation_Numstack_Push( apEvaluation, lResult );
		}
	}


	void  EE_Shunt_Op( sEE_Evaluation * apEvaluation, const sEE_Operator * apOp )
	{
		if( '(' == apOp->mpName[ 0 ] )
		{
			EE_Evaluation_Opstack_Push( apEvaluation, apOp );
		}
		else if ( ')' == apOp->mpName[ 0 ] )
		{
			while( (apEvaluation->mOpStackOffset > 0) && ('(' != apEvaluation->mpOperatorStack[ apEvaluation->mOpStackOffset - 1 ]->mpName[ 0 ] ) )
			{
				EE_Expression_Op_Perform( apEvaluation );
			}
		}
		else
		{
			int lPrec = apOp->mPrecedence;

			if( eEE_Association_Left == apOp->mAssociation )
			{
				lPrec++;
			}

			while( (apEvaluation->mOpStackOffset > 0) && ( lPrec > apEvaluation->mpOperatorStack[ apEvaluation->mOpStackOffset - 1 ]->mPrecedence ) )
			{
				EE_Expression_Op_Perform( apEvaluation );
			}

			EE_Evaluation_Opstack_Push( apEvaluation, apOp );
		}
	}


	int    EE_Token_Build( sLineParser * apParser, sEE_Token * apToken )
	{
		char lChar;

		apToken->mpOperator = 0;
		apToken->mValue = 0;
		apToken->mfGetValue = EE_Token_GetValue;

		lChar = apParser->mpString[ apParser->mOffset ];

		if( lChar )
		{
			while( (' ' == lChar )  || ('\t' == lChar ) )
			{
				apParser->mOffset++;
				lChar = apParser->mpString[ apParser->mOffset ];;
			}

			if( lChar )
			{
				/* number parsing */

				char lCharNext = apParser->mpString[ apParser->mOffset + 1 ];

				/* hex parsing */
				if(  ( ('0' == lChar) && ( ('x' == lCharNext) || ('X' == lCharNext) ) ) || ( '$' == lChar) )
				{
					int lVal;

					if( '$' == lChar )
					{
						apParser->mOffset += 1;
					}
					else
					{
						apParser->mOffset += 2;
					}


					lVal = 0;
					while( lVal >= 0 )
					{
						apToken->mValue <<= 4;
						apToken->mValue += lVal;
						apParser->mOffset++;
						lVal = EE_GetCharValueHex( apParser->mpString[ apParser->mOffset ] );
					}
					return( 1 );
				}
				else if( (lChar >= '0') && (lChar <= '9') )
				{
					apToken->mValue = lChar - '0';

					lChar = lCharNext;
					apParser->mOffset++;

					while( (lChar >= '0') && (lChar <= '9') )
					{
						apToken->mValue *= 10;
						apToken->mValue += (lChar - '0');
						apParser->mOffset++;
						lChar = apParser->mpString[ apParser->mOffset ];
					}
					return( 1 );
				}
				else
				{
					apToken->mpOperator = EE_Operator_Find( apParser );
					if( !apToken->mpOperator && gfBkptValueGetterBuild )
					{
						const char * lpNext = gfBkptValueGetterBuild( apToken, &apParser->mpString[ apParser->mOffset ] );
						if( lpNext )
						{
							apParser->mOffset = lpNext - apParser->mpString;
						}
						return( (int)lpNext );
					}
					return( (int)apToken->mpOperator );
				}
			}
		}
		return( 0 );
	}


	void  EE_Expression_Compile( const char * apString, sEE_Expression * apExpression )
	{
		sLineParser  lLineParser;
		sEE_Token  lToken;

		lLineParser.mpString = apString;
		lLineParser.mOffset = 0;

		apExpression->mTokenStackOffset = 0;

		while( EE_Token_Build( &lLineParser, &lToken) )
		{
			/* detect unary minus case */
			if( lToken.mpOperator && '-' == lToken.mpOperator->mpName[ 0 ] )
			{
				int lIndex = apExpression->mTokenStackOffset;
				if( ( 0 == lIndex ) ||
					( ( apExpression->mTokenStack[ lIndex-1].mpOperator ) && ( ')' == apExpression->mTokenStack[ lIndex-1].mpOperator->mpName[ 0 ] ) ) )
				{
					lToken.mpOperator = &mEE_Operators[ 0 ];
				}
			}
			EE_Expression_Tokenstack_Push( apExpression, &lToken );
		}
	}


	int EE_Expression_Evaluate( const sEE_Expression * apExpression )
	{
		int    i;

		sEE_Evaluation    lEvaluation;

		lEvaluation.mNumberStackOffset = 0;
		lEvaluation.mOpStackOffset = 0;

		for( i=0; i<apExpression->mTokenStackOffset; i++ )
		{
			const sEE_Token * lpToken = &apExpression->mTokenStack[ i ];

			if( lpToken->mpOperator )
			{
				EE_Shunt_Op( &lEvaluation, lpToken->mpOperator );
			}
			else
			{
				EE_Evaluation_Numstack_Push( &lEvaluation, lpToken->mfGetValue( lpToken ) );
			}
		}

		while( lEvaluation.mOpStackOffset )
		{
			EE_Expression_Op_Perform( &lEvaluation );
		}

		return( lEvaluation.mNumberStack[ 0 ] );
	}

}
