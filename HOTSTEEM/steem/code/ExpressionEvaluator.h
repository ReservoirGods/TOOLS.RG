#ifndef INCLUDED_EXPRESSIONEVALUATOR_H
#define INCLUDED_EXPRESSIONEVALUATOR_H

namespace	ExpressionEvaluator
{


	typedef enum eEE_Association
	{
		eEE_Association_Left,
		eEE_Association_Right,
	} eEE_Association;


	typedef struct sEE_Operator
	{
		const char *	mpName;
		int				mPrecedence;
		int				mOperandCount;
		eEE_Association	mAssociation;
		int				(*mfEvaluator)( int aValue0, int aValue1 );
	} sEE_Operator;


	typedef struct sEE_Token
	{
		const sEE_Operator *	mpOperator;
		int						mValue;
		int						(*mfGetValue)( const struct sEE_Token * apToken );
	} sEE_Token;

	enum eEE_Expression_Stack
	{
		eEE_Expression_Stack_LIMIT = 64
	};

	typedef struct sEE_Expression
	{
		int			mTokenStackOffset;
		sEE_Token	mTokenStack[ eEE_Expression_Stack_LIMIT ];

	} sEE_Expression;

	void  EE_Expression_Compile( const char * apString, sEE_Expression * apExpression );
	int EE_Expression_Evaluate( const sEE_Expression * apExpression );

	extern const char * (*gfBkptValueGetterBuild)( struct sEE_Token * apToken, const char * apName );


}

#endif // !INCLUDED_EXPRESSIONEVALUATOR_H
