#include <stdio.h>

#include "vtest.hpp"

void Foo::bar( int a, int b )
{
	printf( "Foo::bar(%d,%d) called in C++\n", a, b );
}

void baz( Foo *f, int a, int b )
{
	f->bar( a, b );
}

