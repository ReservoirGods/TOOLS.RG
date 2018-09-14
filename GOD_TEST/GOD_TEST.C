#include	<GODLIB\UNITTEST\UNITTEST.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>

#define	UNIT_TESTS_LIST					\
	UNIT_TEST_ITEM( Encrypt )			\
	UNIT_TEST_ITEM( LinkedList )

typedef struct sUnitTestNamedFunctions
{
	const char * mpName;
	void( *mpFunction )( void );
} sUnitTestNamedFunctions;

#define	UNIT_TEST_ITEM( _aName )  void UnitTest##_aName( void );
UNIT_TESTS_LIST
#undef	UNIT_TEST_ITEM

#define	UNIT_TEST_ITEM( _aName )  {	#_aName, UnitTest##_aName },
struct sUnitTestNamedFunctions	gUnitTestNamedFunctions[] =
{
	UNIT_TESTS_LIST
};
#undef UNIT_TEST_ITEM

int		main( int argc, char **argv )
{
	(void)argc;
	(void)argv;

	printf( "GodLib Unit Tests\n" );
	printf( "(c) 2018 Reservoir Gods\n\n" );


#define	UNIT_TEST_ITEM( _aName )  UnitTest##_aName();
	UNIT_TESTS_LIST
#undef	UNIT_TEST_ITEM

	printf( "\nDone.\n" );
	GemDos_Cnecin();


	return 0;
}
