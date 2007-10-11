#ifndef __LUCID_LALLOCATOR_H__
#define	__LUCID_LALLOCATOR_H__

#include <glib.h>

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
