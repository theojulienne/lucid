#include "liter.h"

class Foo
{
public:
	Foo();
	~Foo();
	LT_DEFINE_ITER(Foo, int);
private:
	int * m_int;
};

extern "C"
{
Foo *	foo_create();
void	foo_destroy(Foo * self);
LIter<int> foo_get_iter(Foo * self);	
}

Foo::Foo(): m_int(NULL)
{
}

Foo::~Foo()
{
	if(this->m_int)
		free(this->m_int);
}

bool Foo::MoveNext(Foo * self)
{
	if(self->m_int == NULL)
	{
		self->m_int = (int *)malloc(sizeof(int));
		* self->m_int = 0;
		return true;
	}

	if(* self->m_int < 10)
	{
		* self->m_int += 1;	
		return true;
	}
	else
	{
		free(self->m_int);
		self->m_int = NULL;	
		return false;
	}
}

int * Foo::Current(Foo * self)
{
	return self->m_int;
}

/*LIter<> Foo::GetIter()	
{
	
}*/

Foo *	foo_create()
{
	return new Foo();
}

void	foo_destroy(Foo * self)
{
	delete self;
}

LIter<int> foo_get_iter(Foo * self)
{
	return self->GetIter();
}

