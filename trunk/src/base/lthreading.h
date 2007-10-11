#ifndef __LUCID_LTHREADING_H__
#define __LUCID_LTHREADING_H__

#include <ltypes.h>

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
	void * m_impl;
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
	void * m_impl;
};

class LThreadStorage
{
public:
	LThreadStorage(void (* val_free_fn) (void *));
	~LThreadStorage();
	void Set(void * key);
	void * Get();
private:
	void * m_impl;
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

