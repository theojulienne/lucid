#include <stdio.h>

typedef struct
{
	const char *name;
	const char *(*to_string)( int val );
	int (*from_string)( const char *val );
	int (*get_num)( );
	int (*from_index)( int index );
} lt_enum_type_info;

const char * lt_enum_get_name( lt_enum_type_info *type )
{
	return type->name;
}

const char * lt_enum_to_string( lt_enum_type_info *type, int val )
{
	return type->to_string( val );
}

int lt_enum_from_string( lt_enum_type_info *type, const char * val )
{
	return type->from_string( val );
}

int lt_enum_get_num( lt_enum_type_info *type )
{
	return type->get_num( );
}

int lt_enum_from_index( lt_enum_type_info *type, int index )
{
	return type->from_index( index );
}

#define LT_DEFINE_ENUM(enum_type, func_prefix) \
static lt_enum_def_t lt_ ## enum_type ## _enum_lookup[]; \
static lt_enum_type_info lt_ ## func_prefix ## _enum_type_info; \
lt_enum_type_info * func_prefix ## _get_info( ) \
{ \
	return &lt_ ## func_prefix ## _enum_type_info;\
} \
static const char * func_prefix ## _to_string( enum enum_type val ) \
{ \
	int a; \
	for ( a = 0; lt_ ## enum_type ## _enum_lookup[a].strval != NULL; a++ ) \
	{ \
		if ( lt_ ## enum_type ## _enum_lookup[a].value == val ) \
			return lt_ ## enum_type ## _enum_lookup[a].strval; \
	} \
	return NULL; \
} \
static enum enum_type func_prefix ## _from_string( const char *strval ) \
{ \
	int a; \
	for ( a = 0; lt_ ## enum_type ## _enum_lookup[a].strval != NULL; a++ ) \
	{ \
		if ( !strcmp( lt_ ## enum_type ## _enum_lookup[a].strval, strval ) ) \
			return lt_ ## enum_type ## _enum_lookup[a].value; \
	} \
	return -1; \
} \
static int func_prefix ## _get_num( ) \
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
static enum enum_type func_prefix ## _from_index( int index ) \
{ \
	return lt_ ## enum_type ## _enum_lookup[index].value; \
} \
static lt_enum_type_info lt_ ## func_prefix ## _enum_type_info = { \
	#enum_type, \
	(const char *(*)(int)) func_prefix ## _to_string, \
	(int(*)(const char *)) func_prefix ## _from_string, \
	func_prefix ## _get_num, \
	(int(*)(int))func_prefix ## _from_index, \
}; \
static lt_enum_def_t lt_ ## enum_type ## _enum_lookup[] =

#define LT_E(name) { name, #name }
#define LT_ENUM_END { 0, NULL }

typedef struct
{
	int value;
	char *strval;
} lt_enum_def_t;

enum LTestFlags
{
	LT_TEST_FLAGS_FOO = 22,
	LT_TEST_FLAGS_BAR = 64,
	LT_TEST_FLAGS_BAZ = 445,
};

LT_DEFINE_ENUM(LTestFlags, lt_test_flags)
{
	LT_E(LT_TEST_FLAGS_FOO),
	LT_E(LT_TEST_FLAGS_BAR),
	LT_E(LT_TEST_FLAGS_BAZ),
	LT_ENUM_END
};

int main( int argc, char *argv[] )
{
	int a;
	lt_enum_type_info * foo = lt_test_flags_get_info( );
	
	printf("enum values to strings:\n" );
	printf("%d -> %s\n", LT_TEST_FLAGS_FOO, 
		lt_enum_to_string(foo, LT_TEST_FLAGS_FOO));
	printf("%d -> %s\n", LT_TEST_FLAGS_BAR, 
		lt_enum_to_string(foo, LT_TEST_FLAGS_BAR));
	printf("%d -> %s\n", LT_TEST_FLAGS_BAZ, 
		lt_enum_to_string(foo, LT_TEST_FLAGS_BAZ));

	printf("%d -> %s\n", 0xDEADBEEF, lt_enum_to_string(foo, 0xDEADBEEF));
	
	printf("\n");
	
	printf("enum values from strings:\n");
	printf("%s -> %d\n", "LT_TEST_FLAGS_FOO", 
		lt_enum_from_string(foo, "LT_TEST_FLAGS_FOO"));
	printf("%s -> %d\n", "LT_TEST_FLAGS_BAR", 
		lt_enum_from_string(foo, "LT_TEST_FLAGS_BAR"));
	printf("%s -> %d\n", "LT_TEST_FLAGS_BAZ", 
		lt_enum_from_string(foo, "LT_TEST_FLAGS_BAZ"));
	printf("%s -> %d\n", "LT_TEST_FLAGS_FAKE", 
		lt_enum_from_string(foo, "LT_TEST_FLAGS_FAKE"));
	
	printf("\n");
	
	printf("iterating through the enum:\n");
	for (a = 0; a < lt_enum_get_num(foo); a++)
	{
		int val = lt_enum_from_index(foo, a);
		printf("%d -> %d -> %s\n", a, val, lt_enum_to_string(foo, val));
	}
	
	printf("\n");

	return 0;
}

