/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\UNITTEST\UNITTEST.H>
#include	<GODLIB\DEBUGLOG\DEBUGLOG.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\MEMORY\MEMORY.H>


/* ###################################################################################
#  LIST OF TESTS
################################################################################### */

#define	UNIT_TESTS_LIST					\
	UNIT_TEST_ITEM( Base )				\
	UNIT_TEST_ITEM( Disk )				\
	UNIT_TEST_ITEM( Encrypt )			\
	UNIT_TEST_ITEM( JSON )				\
	UNIT_TEST_ITEM( Lexer )				\
	UNIT_TEST_ITEM( LinkedList )		\
	UNIT_TEST_ITEM( Registry )			\
	UNIT_TEST_ITEM( String )



/* ###################################################################################
#  TEST SCAFFOLDING
################################################################################### */


/*	Prototypes of Unit test functions*/
#define	UNIT_TEST_ITEM( _aName )  void UnitTest##_aName( void );
UNIT_TESTS_LIST
#undef	UNIT_TEST_ITEM


/*	names and function pointers to unit tests*/

typedef struct sUnitTestNamedFunctions
{
	const char * mpName;
	void( *mpFunction )( void );
} sUnitTestNamedFunctions;

#define	UNIT_TEST_ITEM( _aName )  {	#_aName, UnitTest##_aName },
struct sUnitTestNamedFunctions	gUnitTestNamedFunctions[] =
{
	UNIT_TESTS_LIST
};
#undef UNIT_TEST_ITEM


/* ###################################################################################
#  CODE
################################################################################### */

int		main( int argc, char **argv )
{
	(void)argc;
	(void)argv;

	printf( "GodLib Unit Tests\n" );
	printf( "(c) 2018 Reservoir Gods\n\n" );

	DebugLog_Init( eDebugLog_File | eDebugLog_Screen, "UNITTEST.LOG" );

	/* invoke unit tests*/
#define	UNIT_TEST_ITEM( _aName )  UnitTest##_aName();
	UNIT_TESTS_LIST
#undef	UNIT_TEST_ITEM

	Memory_ShowCurrentRecords();
	DebugLog_DeInit();

	printf( "\nDone.\n" );
	GemDos_Cnecin();


	return 0;
}
