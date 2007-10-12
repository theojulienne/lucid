#ifndef __LUCID_LALLOCATOR_H__
#define	__LUCID_LALLOCATOR_H__

#include <glib.h>

//This way we have a consistent memory allocation scheme.
//We also need to incorporate our "synch block" pointer into the class tree somewhere. Here would 
//make sense so far, but we will need to come up with a better name. (LCoreMixin?)

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
		g_free(addr);
      	}
};
 
#endif
