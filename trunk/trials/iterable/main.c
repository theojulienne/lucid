#include "liter.h"

typedef void Foo;

Foo *	foo_create();
void	foo_destroy(Foo * self);
LIter   foo_get_iter(Foo * self);

int main(int argc, char ** argv)
{
	Foo * foo = foo_create();
	LIter it = foo_get_iter(foo);	
		
	for(; lt_iter_move_next(&it);)
		printf("%d\n", * (int *)lt_iter_current(&it));
	
	lt_iter_destroy(&it);
	foo_destroy(foo);

	return 0;
}
