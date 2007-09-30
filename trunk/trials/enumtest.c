#include <stdio.h>

typedef struct
{
	const char *name;
	const char *(*to_string)( int val );
	int (*from_string)( const char *val );
	int (*get_num)( );
	int (*from_index)( int index );
} lt_enum_type_info;

const char *enum_get_name( lt_enum_type_info *type )
{
	return type->name;
}

const char *enum_to_string( lt_enum_type_info *type, int val )
{
	return type->to_string( val );
}

int enum_from_string( lt_enum_type_info *type, const char * val )
{
	return type->from_string( val );
}

int enum_get_num( lt_enum_type_info *type )
{
	return type->get_num( );
}

int enum_from_index( lt_enum_type_info *type, int index )
{
	return type->from_index( index );
}

#define LT_DEFINE_ENUM(enum_type,func_prefix) \
extern lt_enum_def_t lt_ ## enum_type ## _enum_lookup[]; \
extern lt_enum_type_info lt_ ## func_prefix ## _enum_type_info; \
lt_enum_type_info * func_prefix ## _get_struct( ) \
{ \
	return &lt_ ## func_prefix ## _enum_type_info;\
} \
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
		if ( !strcmp( lt_ ## enum_type ## _enum_lookup[a].strval, strval ) ) \
			return lt_ ## enum_type ## _enum_lookup[a].value; \
	} \
	return -1; \
} \
int func_prefix ## _get_num( ) \
{ \
	static int length = -1; \
	\
	if ( length != -1 ) \
		return length; \
	\
	int a; \
	for ( a = 0; lt_ ## enum_type ## _enum_lookup[a].strval != NULL; a++ ) \
		;\
	length = a; \
	return a; \
} \
enum enum_type func_prefix ## _from_index( int index ) \
{ \
	return lt_ ## enum_type ## _enum_lookup[index].value; \
} \
lt_enum_type_info lt_ ## func_prefix ## _enum_type_info = { \
	#enum_type, \
	(const char *(*)(int)) func_prefix ## _to_string, \
	(int(*)(const char *)) func_prefix ## _from_string, \
	func_prefix ## _get_num, \
	(int(*)(int))func_prefix ## _from_index, \
}; \
lt_enum_def_t lt_ ## enum_type ## _enum_lookup[] =

#define LT_E(name) { name, #name }
#define LT_ENUM_END { 0, NULL }

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
	LT_E( lFoo ),
	LT_E( lBar ),
	LT_E( lBaz ),
	LT_ENUM_END
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
		printf( "%d -> %d -> %s\n", a, val, test_to_string( val ) );
	}
	
	printf( "\n" );
	
	printf( "converting to an abstract representation:\n" );
	
	void *foo = (void *)test_get_struct( );
	printf( "enum struct at %p\n", foo );
	printf( "enum's name: %s\n", enum_get_name(foo) );
	printf( "enum's name for 22: %s\n", enum_to_string(foo,22) );
	printf( "enum's name for 23: %s\n", enum_to_string(foo,23) );
	printf( "enum's value for lFoo: %d\n", enum_from_string(foo,"lFoo") );
	printf( "enum's value for lUnknown: %d\n", enum_from_string(foo,"lUnknown") );
	printf( "iterating through the abstract enum:\n" );
	for ( a = 0; a < enum_get_num(foo); a++ )
	{
		int val = enum_from_index( foo, a );
		printf( "%d -> %d -> %s\n", a, val, enum_to_string( foo, val ) );
	}
	
	printf( "\n" );
	
	
	return 0;
}