#include <stdio.h>

#define LT_DEFINE_ENUM(enum_type,func_prefix) \
extern lt_enum_def_t lt_ ## enum_type ## _enum_lookup[]; \
const char * func_prefix ## _to_string( enum enum_type val ) \
{ \
	int a; \
	for ( a = 0; lt_ ## enum_type ## _enum_lookup[a].strval != NULL; a++ ) \
	{ \
		if ( lt_ ## enum_type ## _enum_lookup[a].value == val ) \
			return lt_ ## enum_type ## _enum_lookup[a].strval; \
	} \
	return NULL; \
} \
enum enum_type func_prefix ## _from_string( const char *strval ) \
{ \
	int a; \
	for ( a = 0; lt_ ## enum_type ## _enum_lookup[a].strval != NULL; a++ ) \
	{ \
		if ( lt_ ## enum_type ## _enum_lookup[a].strval == strval ) \
			return lt_ ## enum_type ## _enum_lookup[a].value; \
	} \
	return -1; \
} \
int func_prefix ## _get_num( ) \
{ \
	int a; \
	for ( a = 0; lt_ ## enum_type ## _enum_lookup[a].strval != NULL; a++ ) \
		;\
	return a; \
} \
enum enum_type func_prefix ## _from_index( int index ) \
{ \
	return lt_ ## enum_type ## _enum_lookup[index].value; \
} \
lt_enum_def_t lt_ ## enum_type ## _enum_lookup[] =

typedef struct
{
	int value;
	char *strval;
} lt_enum_def_t;

enum Test
{
	lFoo = 22,
	lBar = 64,
	lBaz = 445,
};

LT_DEFINE_ENUM( Test, test )
{
	{ lFoo, "lFoo" },
	{ lBar, "lBar" },
	{ lBaz, "lBaz" },
	{ 0, NULL },
};

int main( int argc, char *argv[] )
{
	int a;
	
	printf( "enum values to strings:\n" );
	printf( "%d -> %s\n", lFoo, test_to_string( lFoo ) );
	printf( "%d -> %s\n", lBar, test_to_string( lBar ) );
	printf( "%d -> %s\n", lBaz, test_to_string( lBaz ) );
	printf( "%d -> %s\n", 0xDEADBEEF, test_to_string( 0xDEADBEEF ) );
	
	printf( "\n" );
	
	printf( "enum values from strings:\n" );
	printf( "%s -> %d\n", "lFoo", test_from_string( "lFoo" ) );
	printf( "%s -> %d\n", "lBar", test_from_string( "lBar" ) );
	printf( "%s -> %d\n", "lBaz", test_from_string( "lBaz" ) );
	printf( "%s -> %d\n", "lFake", test_from_string( "lFake" ) );
	
	printf( "\n" );
	
	printf( "iterating through the enum:\n" );
	for ( a = 0; a < test_get_num(); a++ )
	{
		int val = test_from_index( a );
		printf( "%d -> %d -> %d\n", a, val, test_to_string( val ) );
	}
	
	return 0;
}