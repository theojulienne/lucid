#include <lucid-base.h>

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

#ifdef _WIN32

//FIXME- A THREAD_ASSERT macro needs to be implemented for Windows. 
//	 But should probably be in lucid-base.h- for use in other files.

LMutex::LMutex()
{
	InitializeCriticalSection(&this->m_impl, NULL);
}

LMutex::~LMutex()
{
	DeleteCriticalSection(&this->m_impl);
}

void	LMutex::Lock()
{
	EnterCriticalSection(&this->m_impl);
}

void 	LMutex::Unlock()
{
	EnterCriticalSection(&this->m_impl);
}

bool_t	LMutex::Trylock()
{
	//FIXME
	EnterCriticalSection(&this->m_impl);
	return TRUE;
}

LCond::LCond()
{
	this->m_impl = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	this->m_waiting = 0;
}

LCond::~LCond()
{
	CloseHandle(this->m_impl);
}

void	LCond::Signal()
{
	//FIXME- ATOMIC CHECK ?
	if (this->m_waiting > 0)
        	ReleaseSemaphore(this->m_impl, 1, NULL);
}

void 	LCond::Broadcast()
{
	//FIXME- ATOMIC CHECK ?
	if (m_waiting > 0)
        	ReleaseSemaphore(this->m_impl, this->m_waiting, NULL);
}

void	LCond::Wait(LMutex * mutex)
{
	g_return_if_fail(mutex != NULL);
	lt_atomic_add_int(&this->m_waiting, 1);
       	mutex->Lock();
       	WaitForSingleObject(this->m_impl, INFINITE);
       	mutex->Unlock();
	lt_atomic_add_int(&this->m_waiting, -1);
}

LThreadStorage::LThreadStorage(void (* val_free_fn) (void *))
{
	this->m_impl = TlsAlloc();
	g_assert(this->m_impl != TLS_OUT_OF_INDEXES);
	this->m_val_free_fn = val_free_fn;
}

LThreadStorage::~LThreadStorage()
{
	if(this->m_val_free_fn)
		this->m_val_free_fn(this->Get());
	g_assert(TlsFree(this->m_impl));
}

void LThreadStorage::Set(void * key)
{
	g_assert(TlsSetValue(this->m_impl, key));
}

void * LThreadStorage::Get()
{
	return TlsGetValue(this->m_impl);
}

bool_t    lt_thread_create(LThread * thread, void * (* thread_func) (void *), void * user_data, bool_t joinable)
{
	DWORD thread_id;
        HANDLE h;

	g_return_val_if_fail(thread != NULL, FALSE);	
	
	h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_func, 
		(PVOID)user_data, 0, &thread_id);
	if(! joinable)
		CloseHandle(h);
	* thread = (LThread)h;
	return h != NULL;	
}

void	   lt_thread_exit(void * ret_val)
{
	ExitThread((DWORD)ret_val);
}

void *	   lt_thread_join(LThread thread)
{
	HANDLE h = (HANDLE)thread;
	DWORD ret_val = 0;
	WaitForSingleObject(h, INFINITE);
	GetExitCodeThread(h, &ret_val); 
       	CloseHandle(h);
	return ret_val;
}

LThread    lt_thread_self()
{
	return (LThread)GetCurrentThread();
}

#else

#define THREAD_ASSERT(x)	G_STMT_START { if(x == 0) ; else g_error("%s: %s\n", __FUNCTION__, strerror(x)); }

LMutex::LMutex()
{
	THREAD_ASSERT(pthread_mutex_init(&this->m_impl, NULL));
}

LMutex::~LMutex()
{
	THREAD_ASSERT(pthread_mutex_destroy(&this->m_impl));
}

void	LMutex::Lock()
{
	THREAD_ASSERT(pthread_mutex_lock(&this->m_impl));
}

void 	LMutex::Unlock()
{
	THREAD_ASSERT(pthread_mutex_unlock(&this->m_impl));
}

bool_t	LMutex::Trylock()
{
	return pthread_mutex_trylock(&this->m_impl) == 0;
}

LCond::LCond()
{
	THREAD_ASSERT(pthread_cond_init(&this->m_impl, NULL));
}

LCond::~LCond()
{
	THREAD_ASSERT(pthread_cond_destroy(&this->m_impl));
}

void	LCond::Signal()
{
	THREAD_ASSERT(pthread_cond_signal(&this->m_impl));
}

void 	LCond::Broadcast()
{
	THREAD_ASSERT(pthread_cond_broadcast(&this->m_impl));
}

void	LCond::Wait(LMutex * mutex)
{
	THREAD_ASSERT(pthread_cond_wait(&this->m_impl, (pthread_mutex_t *)&mutex->m_impl));
}

LThreadStorage::LThreadStorage(void (* val_free_fn) (void *))
{
	THREAD_ASSERT(pthread_key_create(&this->m_impl, val_free_fn));
}

LThreadStorage::~LThreadStorage()
{
	THREAD_ASSERT(pthread_key_delete(this->m_impl));
}

void LThreadStorage::Set(void * key)
{
	THREAD_ASSERT(pthread_setspecific(this->m_impl, key));
}

void * LThreadStorage::Get()
{
	return pthread_getspecific(this->m_impl);
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

#endif

LMutex * lt_mutex_create()
{
	//FIXME - What is our stance on the new operator? 
	// We should probably come up with our own 'lt_new' that checks. 
	// [See lallocator.hpp]
	LT_NEW_CPP(LMutex);
}

void	lt_mutex_lock(LMutex * self)
{
	LT_CALL_CPP(Lock);
}

bool_t	lt_mutex_trylock(LMutex * self)
{
	LT_RET_CALL_CPP(Trylock, FALSE);
}

void	lt_mutex_unlock(LMutex * self)
{
	LT_CALL_CPP(Unlock);
}

void	lt_mutex_destroy(LMutex * self)
{
	LT_DELETE_CPP();
}	

LCond * lt_cond_create()
{
	LT_NEW_CPP(LCond);
}

void	lt_cond_signal(LCond * self)
{
	LT_CALL_CPP(Signal);
}

void	lt_cond_broadcast(LCond * self)
{
	LT_CALL_CPP(Broadcast);
}

void	lt_cond_wait(LCond * self, LMutex * mutex)
{
	LT_CALL_CPP(Wait, mutex);
}

void	lt_cond_destroy(LCond * self)
{
	LT_DELETE_CPP();
}

LThreadStorage *  lt_thread_storage_create(void (* val_free_fn) (void *))
{
	LT_NEW_CPP(LThreadStorage, val_free_fn); 
}

void 	lt_thread_storage_set(LThreadStorage * self, void * key)
{
	LT_CALL_CPP(Set, key); 
}

void *	lt_thread_storage_get(LThreadStorage * self)
{
	LT_RET_CALL_CPP(Get, NULL);
}

void	lt_thread_storage_destroy(LThreadStorage * self)
{
	LT_DELETE_CPP();
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

