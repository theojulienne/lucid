#include <stdio.h>

#include "vtest.hpp"

void Foo::boo( int a, int b )
{
	printf( "Foo::boo(%d,%d) called in C++\n", a, b );
}

void Foo::bee( int a, int b )
{
	printf( "Foo::bee(%d,%d) called in C++\n", a, b );
}


void baz( Foo *f, int a, int b )
{
	f->bar( a, b );
	f->boo( a, b );
	f->bee( a, b );
}


// not public:
class CFoot: public Foo
{
	void bee( int a, int b )
	{
		printf( "CFoot::bee(%d,%d) called in C++\n", a, b );
	}
	
	void boo( int a, int b )
	{
		printf( "CFoot::boo(%d,%d) called in C++\n", a, b );
	}
	
	void bar( int a, int b )
	{
		printf( "CFoot::bar(%d,%d) called in C++\n", a, b );
	}
};

Foo *makeAFoo( )
{
	return new CFoot;
}