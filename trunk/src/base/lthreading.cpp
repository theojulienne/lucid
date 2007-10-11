#include <lucid-base.h>

#include <pthread.h>

#define THREAD_ASSERT(x)	G_STMT_START { if(x == 0) ; else g_error("%s: %s\n", __FUNCTION__, strerror(x)); }

extern "C"
{

LMutex *  lt_mutex_create();
void 	lt_mutex_lock(LMutex * mutex);
void 	lt_mutex_unlock(LMutex * mutex);
bool_t	lt_mutex_trylock(LMutex * mutex);
void	lt_mutex_destroy(LMutex * mutex);

//LCond

LCond *  lt_cond_create();
void	lt_cond_signal(LCond * cond);
void 	lt_cond_broadcast(LCond * cond);
void	lt_cond_wait(LCond * cond, LMutex * mutex);
void	lt_cond_destroy(LCond * cond);

//LThreadStorage

LThreadStorage *  lt_thread_storage_create(void (* val_free_fn) (void *));
void 	lt_thread_storage_set(LThreadStorage * thread_storage, void * key);
void *	lt_thread_storage_get(LThreadStorage * thread_storage);
void    lt_thread_storage_destroy(LThreadStorage * thread_storage);

bool_t	   lt_thread_create(LThread * thread, void * (* thread_func) (void *), void * user_data, bool_t joinable);
void	   lt_thread_exit(void * ret_val);
void *	   lt_thread_join(LThread thread);			
LThread    lt_thread_self();

void	 _lt_once_run(LOnce * once, void * (* init_func) (void *), void * user_data);

}

//LOnce
LMutex _lt_once_mutex;
LCond _lt_once_cond;

LMutex::LMutex(): m_impl(NULL)
{
	THREAD_ASSERT(pthread_mutex_init((pthread_mutex_t *)&this->m_impl, NULL));
}

LMutex::~LMutex()
{
	THREAD_ASSERT(pthread_mutex_destroy((pthread_mutex_t *)&this->m_impl));
}

void	LMutex::Lock()
{
	THREAD_ASSERT(pthread_mutex_lock((pthread_mutex_t *)&this->m_impl));
}

void 	LMutex::Unlock()
{
	THREAD_ASSERT(pthread_mutex_unlock((pthread_mutex_t *)&this->m_impl));
}

bool_t	LMutex::Trylock()
{
	return pthread_mutex_trylock((pthread_mutex_t *)&this->m_impl) == 0;
}

LMutex * lt_mutex_create()
{
	//FIXME - What is our stance on the new operator? We should probably come up with our own 'lt_new' that checks. 
	// See lallocator.hpp
	return new LMutex();
}

void	lt_mutex_lock(LMutex * self)
{
	LT_CALL_SELF_CPP(Lock);
}

bool_t	lt_mutex_trylock(LMutex * self)
{
	LT_RETURN_CALL_SELF_CPP(Trylock, FALSE);
}

void	lt_mutex_unlock(LMutex * self)
{
	LT_CALL_SELF_CPP(Unlock);
}

void	lt_mutex_destroy(LMutex * self)
{
	g_return_if_fail(self != NULL);
	delete self;
}	

LCond::LCond()
{
	THREAD_ASSERT(pthread_cond_init((pthread_cond_t *)&this->m_impl, NULL));
}

LCond::~LCond()
{
	THREAD_ASSERT(pthread_cond_destroy((pthread_cond_t *)&this->m_impl));
}

void	LCond::Signal()
{
	THREAD_ASSERT(pthread_cond_signal((pthread_cond_t *)&this->m_impl));
}

void 	LCond::Broadcast()
{
	THREAD_ASSERT(pthread_cond_broadcast((pthread_cond_t *)&this->m_impl));
}

void	LCond::Wait(LMutex * mutex)
{
	THREAD_ASSERT(pthread_cond_wait((pthread_cond_t *)&this->m_impl, (pthread_mutex_t *)&mutex->m_impl));
}

LCond * lt_cond_create()
{
	return new LCond();
}

void	lt_cond_signal(LCond * self)
{
	LT_CALL_SELF_CPP(Signal);
}

void	lt_cond_broadcast(LCond * self)
{
	LT_CALL_SELF_CPP(Broadcast);
}

void	lt_cond_wait(LCond * self, LMutex * mutex)
{
	LT_CALL_SELF_CPP(Wait, mutex);
}

void	lt_cond_destroy(LCond * self)
{
	g_return_if_fail(self != NULL);
	delete self;
}

LThreadStorage::LThreadStorage(void (* val_free_fn) (void *))
{
	THREAD_ASSERT(pthread_key_create((pthread_key_t *)&this->m_impl, val_free_fn));
}

LThreadStorage::~LThreadStorage()
{
	THREAD_ASSERT(pthread_key_delete((pthread_key_t)this->m_impl));
}

void LThreadStorage::Set(void * key)
{
	THREAD_ASSERT(pthread_setspecific((pthread_key_t)this->m_impl, key));
}

void * LThreadStorage::Get()
{
	return pthread_getspecific((pthread_key_t)this->m_impl);
}

LThreadStorage *  lt_thread_storage_create(void (* val_free_fn) (void *))
{
	return new LThreadStorage(val_free_fn); 
}

void 	lt_thread_storage_set(LThreadStorage * self, void * key)
{
	LT_CALL_SELF_CPP(Set, key); 
}

void *	lt_thread_storage_get(LThreadStorage * self)
{
	LT_RETURN_CALL_SELF_CPP(Get, NULL);
}

void	lt_thread_storage_destroy(LThreadStorage * self)
{
	g_return_if_fail(self != NULL);
	delete self;
}

bool_t    lt_thread_create(LThread * thread, void * (* thread_func) (void *), void * user_data, bool_t joinable)
{
	pthread_attr_t attr;	
	g_return_val_if_fail(thread != NULL, FALSE);	
	THREAD_ASSERT(pthread_attr_init(&attr));
	THREAD_ASSERT(pthread_attr_setdetachstate(&attr, joinable ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED));
	THREAD_ASSERT(pthread_create((pthread_t *)thread, &attr, thread_func, user_data));
	THREAD_ASSERT(pthread_attr_destroy(&attr));
	return TRUE;
}

void	   lt_thread_exit(void * ret_val)
{
	pthread_exit(ret_val);
}

void *	   lt_thread_join(LThread thread)
{
	void * ret_val;
	THREAD_ASSERT(pthread_join((pthread_t)thread, &ret_val));
	return ret_val;
}

LThread    lt_thread_self()
{
	return (LThread)pthread_self();
}
void _lt_once_run(LOnce * once, void * (* init_func) (void *), void * user_data)
{
	_lt_once_mutex.Lock();
	
	while(once->state == LT_ONCE_STATE_DOING)
		_lt_once_cond.Wait(&_lt_once_mutex);
	
	if(once->state != LT_ONCE_STATE_COMPLETE)
	{
		once->state = LT_ONCE_STATE_DOING;
		
		_lt_once_mutex.Lock();
		once->ret_val = init_func(user_data);
		_lt_once_mutex.Unlock();

		once->state = LT_ONCE_STATE_COMPLETE;
		_lt_once_cond.Broadcast();
	}		

	_lt_once_mutex.Unlock();
}

