#ifndef __LUCID_LMUTEX_H__
#define __LUCID_LMUTEX_H__

#ifdef _WIN32

#include <windows.h>

typedef CRITICAL_SECTION LMutex;

#define	lt_mutex_create		InitializeCriticalSection
#define	lt_mutex_lock		EnterCriticalSection
#define lt_mutex_unlock		LeaveCriticalSection
#define	lt_mutex_destroy	DeleteCriticalSection

#else

#include <pthread.h>

typedef pthread_mutex_t LMutex;

#define lt_mutex_create(mutex) pthread_mutex_init(mutex, NULL)
#define	lt_mutex_lock		pthread_mutex_lock
#define lt_mutex_unlock		pthread_mutex_unlock
#define lt_mutex_destroy		pthread_mutex_destroy

#endif

#endif
	
			
