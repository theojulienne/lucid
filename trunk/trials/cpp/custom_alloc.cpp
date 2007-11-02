#include <stdio.h>
#include <stdlib.h>

/*
class LAllocator
{
public:
	void *	operator new(size_t, void * addr)
      	{
        	return addr;
      	}
    
	void * operator new(size_t size)
      	{
        	return g_malloc(size);
      	}
    
	void  operator delete(void * addr)
      	{
        	if(addr)
			g_free(addr);
      	}
};

*/

class A
{
public:
	void operator () ()
	{
		printf("%s()\n", __FUNCTION__);
	}	
};

int main(int argc, char ** argv)
{
//	LAllocatorMixin * a = new LAllocatorMixin();
//	delete a;
	A a;
	a();
	return 0;
}

