#ifndef __LUCID_LTHREADING_H__
#define __LUCID_LTHREADING_H__

#include <ltypes.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

//FIXME - Add the use of the LError API in the threading system ?

#ifdef __cplusplus

#include <lallocator.hpp>

class LCond;

class LMutex: public LAllocator
{
friend class LCond;

public:
	LMutex();
	~LMutex();
	void Lock();
	bool_t Trylock();
	void Unlock();	
private:
#ifdef _WIN32
	CRITICAL_SECTION m_impl;
#else
	pthread_mutex_t m_impl;
#endif
};

class LCond
{
public:
	LCond();
	~LCond();
	void Signal();
	void Broadcast();
	void Wait(LMutex * mutex);
private:
#ifdef _WIN32
	HANDLE m_impl;
	//Simulate a condition variable
	volatile LONG m_waiting;
#else
	pthread_cond_t m_impl;
#endif
};

class LThreadStorage
{
public:
	LThreadStorage(void (* val_free_fn) (void *));
	~LThreadStorage();
	void Set(void * key);
	void * Get();
private:
#ifdef _WIN32
	DWORD m_impl;
	void (* m_val_free_fn) (void *);
#else
	pthread_key_t m_impl;
#endif
};

#else

typedef void LMutex;
typedef void LCond;
typedef void LThreadStorage;

#endif

G_BEGIN_DECLS

//LMutex

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

typedef void * LThread;

bool_t	   lt_thread_create(LThread * thread, void * (* thread_func) (void *), void * user_data, bool_t joinable);
void	   lt_thread_exit(void * ret_val);
void *	   lt_thread_join(LThread thread);			
LThread    lt_thread_self();

typedef enum
{
	LT_ONCE_STATE_NOT_DONE,
	LT_ONCE_STATE_DOING,
	LT_ONCE_STATE_COMPLETE
} LOnceState;

typedef struct
{
    	volatile LOnceState state;
	volatile void * ret_val;
} LOnce;

void	_lt_once_run(LOnce * once, void * (* init_func) (void *), void * user_data);

#define		LT_ONCE_STATIC_INIT			{ LT_ONCE_STATE_NOT_DONE, NULL }
#define 	lt_once(once, init_func, user_data)	_lt_once_run(once, init_func, user_data)

G_END_DECLS

#endif

