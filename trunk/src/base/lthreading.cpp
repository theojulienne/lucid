#include <lucid-base.h>

#define THREAD_ASSERT(x)	G_STMT_START { if(x == 0) ; else g_warning("%s: %s\n", __FUNCTION__, strerror(-x)); }

extern "C"
{

void 	lt_mutex_create(LMutex * mutex);
void 	lt_mutex_lock(LMutex * mutex);
void 	lt_mutex_unlock(LMutex * mutex);
bool_t	lt_mutex_trylock(LMutex * mutex);
void	lt_mutex_destroy(LMutex * mutex);

void	lt_cond_create(LCond * cond);
void	lt_cond_signal(LCond * cond);
void 	lt_cond_broadcast(LCond * cond);
void	lt_cond_wait(LCond * cond, LMutex * mutex);
void	lt_cond_destroy(LCond * cond);

void	lt_thread_storage_create(LThreadStorage * thread_storage, void (* val_free_fn) (void *));
void 	lt_thread_storage_set(LThreadStorage thread_storage, void * key);
void *	lt_thread_storage_get(LThreadStorage thread_storage);
void	lt_thread_storage_destroy(LThreadStorage thread_storage);

bool_t	   lt_thread_create(LThread * thread, void * (* thread_func) (void *), void * user_data, bool_t joinable);
void	   lt_thread_exit(void * ret_val);
void *	   lt_thread_join(LThread thread);			
LThread    lt_thread_self();

}

void 	lt_mutex_create(LMutex * mutex)
{
	THREAD_ASSERT(pthread_mutex_init(mutex, NULL));
}

void 	lt_mutex_lock(LMutex * mutex)
{
	THREAD_ASSERT(pthread_mutex_lock(mutex));
}

void 	lt_mutex_unlock(LMutex * mutex)
{
	THREAD_ASSERT(pthread_mutex_unlock(mutex));
}

bool_t	lt_mutex_trylock(LMutex * mutex)
{
	return pthread_mutex_trylock(mutex) == 0;
}

void	lt_mutex_destroy(LMutex * mutex)
{
	THREAD_ASSERT(pthread_mutex_destroy(mutex));
}

void	lt_cond_create(LCond * cond)
{
	THREAD_ASSERT(pthread_cond_init(cond, NULL));
}

void	lt_cond_signal(LCond * cond)
{
	THREAD_ASSERT(pthread_cond_signal(cond));
}

void 	lt_cond_broadcast(LCond * cond)
{
	THREAD_ASSERT(pthread_cond_broadcast(cond));
}

void	lt_cond_wait(LCond * cond, LMutex * mutex)
{
	THREAD_ASSERT(pthread_cond_wait(cond, mutex));
}

void	lt_cond_destroy(LCond * cond)
{
	THREAD_ASSERT(pthread_cond_destroy(cond));
}

void	lt_thread_storage_create(LThreadStorage * thread_storage, void (* val_free_fn) (void *))
{
	THREAD_ASSERT(pthread_key_create(thread_storage, val_free_fn));
}

void 	lt_thread_storage_set(LThreadStorage thread_storage, void * key)
{
	THREAD_ASSERT(pthread_setspecific(thread_storage, key));
}

void *	lt_thread_storage_get(LThreadStorage thread_storage)
{
	return pthread_getspecific(thread_storage);
}

void	lt_thread_storage_destroy(LThreadStorage thread_storage)
{
	THREAD_ASSERT(pthread_key_delete(thread_storage));
}

bool_t     lt_thread_create(LThread * thread, void * (* thread_func) (void *), void * user_data, bool_t joinable)
{
	pthread_attr_t attr;
	int ret;	
	g_return_val_if_fail(thread != NULL, FALSE);	
	ret = pthread_attr_init(&attr);
	THREAD_ASSERT(ret);
	ret = pthread_attr_setdetachstate(&attr, joinable ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED);
	THREAD_ASSERT(ret);
	ret = pthread_create(thread, &attr, thread_func, user_data);
	THREAD_ASSERT(pthread_attr_destroy(&attr));
	return ret == 0;
}

void	   lt_thread_exit(void * ret_val)
{
	pthread_exit(ret_val);
}

void *	   lt_thread_join(LThread thread)
{
	void * ret_val;
	THREAD_ASSERT(pthread_join(thread, &ret_val));
	return ret_val;
}

LThread    lt_thread_self()
{
	return pthread_self();
}

