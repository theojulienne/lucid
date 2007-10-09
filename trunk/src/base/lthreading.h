#ifndef __LUCID_LTHREADING_H__
#define __LUCID_LTHREADING_H__

#include <ltypes.h>
#include <pthread.h>

//FIXME - Add the use of the LError API in other parts of the threading system ?

G_BEGIN_DECLS

//LMutex
typedef pthread_mutex_t LMutex;

#define LT_MUTEX_STATIC_INIT	PTHREAD_MUTEX_INITIALIZER

void 	lt_mutex_create(LMutex * mutex);
void 	lt_mutex_lock(LMutex * mutex);
void 	lt_mutex_unlock(LMutex * mutex);
bool_t	lt_mutex_trylock(LMutex * mutex);
void	lt_mutex_destroy(LMutex * mutex);

//LCond

typedef pthread_cond_t LCond;

#define	LT_COND_STATIC_INIT	PTHREAD_COND_INITIALIZER

void	lt_cond_create(LCond * cond);
void	lt_cond_signal(LCond * cond);
void 	lt_cond_broadcast(LCond * cond);
void	lt_cond_wait(LCond * cond, LMutex * mutex);
void	lt_cond_destroy(LCond * cond);

typedef pthread_key_t LThreadStorage;

void	lt_thread_storage_create(LThreadStorage * thread_storage, void (* val_free_fn) (void *));
void 	lt_thread_storage_set(LThreadStorage thread_storage, void * key);
void *	lt_thread_storage_get(LThreadStorage thread_storage);
void    lt_thread_storage_destroy(LThreadStorage thread_storage);

typedef pthread_t LThread;

bool_t	   lt_thread_create(LThread * thread, void * (* thread_func) (void *), void * user_data, bool_t joinable);
void	   lt_thread_exit(void * ret_val);
void *	   lt_thread_join(LThread thread);			
LThread    lt_thread_self();

G_END_DECLS

#endif

