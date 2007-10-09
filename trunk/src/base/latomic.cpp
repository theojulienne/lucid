#include <lucid-base.h>

#if defined(REAL_GLIB)
#elif defined(_WIN32)
#elif defined(__i386__) && define(_MAC) || !defined(REAL_GLIB)

// these could really be better..

int lt_atomic_get_int(volatile int * ptr)
{
	return *ptr;
}

void lt_atomic_set_int(volatile int * ptr, int new_val)
{
	*ptr = new_val;
}

void lt_atomic_add_int(volatile int * ptr, int val)
{
	__asm__ __volatile__ ("lock; addl %1, %0"
		: "=m" (*ptr) 
		: "ir" (val), "m" (*ptr));
}

int  lt_atomic_add_xfer_int(volatile int * ptr, int val)
{
	int res;

	__asm__ __volatile__ ("lock; xaddl %0, %1"
        	: "=r" (res), "=m" (*ptr) 
       		: "0" (val), "m" (*ptr));

    	return res;
}

bool_t lt_atomic_compare_xfer_int(volatile int	* ptr, int old_val, int new_val)
{
	int res;
 
    	__asm__ __volatile__ ("lock; cmpxchgl %2, %1"
        	: "=a" (res), "=m" (*ptr)
		: "r" (new_val), "m" (*ptr), "0" (old_val)); 

    	return res == old_val;
}

/*
#elif defined(__ppc__) && defined(_MAC)
#error "FIXME: someone should implement and test PPC atomic routines"
*/
#else

static LMutex _atomic_mutex = LT_MUTEX_STATIC_INIT;


int lt_atomic_get_int(volatile int * ptr)
{
	int res;
	
	lt_mutex_lock (&_atomic_mutex);
	res = *ptr;
	lt_mutex_unlock (&_atomic_mutex);

	return res;
}

void lt_atomic_set_int(volatile int * ptr, int new_val)
{
	lt_mutex_lock (&_atomic_mutex);
	*ptr = new_val;
	lt_mutex_unlock (&_atomic_mutex);
}

void lt_atomic_add_int(volatile int * ptr, int val)
{
	lt_mutex_lock (&_atomic_mutex);
   	*ptr += val;
    	lt_mutex_unlock (&_atomic_mutex);
}

int lt_atomic_add_xfer_int(volatile int * ptr, int val)
{
    	int res;
    
    	lt_mutex_lock (&_atomic_mutex);
    	res = *ptr;
    	*ptr += val;
    	lt_mutex_unlock (&_atomic_mutex);

    	return res;
}

int lt_atomic_compare_xfer_int(volatile int * ptr, int old_val, int new_val)
{
    	int res;

    	lt_mutex_lock (&_atomic_mutex);

    	if (*ptr == old_val)
    	{
        	res = TRUE;
        	*ptr = new_val;
    	}
    	else
        	res = FALSE;
    
    	lt_mutex_unlock (&_atomic_mutex);
    	return res;
}

#endif

