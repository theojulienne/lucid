#include <lucid-base.h>
#include <unistd.h>

//None of this makes sense... :-P
//It will be replaced with an example using a thread message queue. 

LMutex * _mutex1;
LThreadStorage * _tls1;

LCond * _dcond;   
LMutex * _dmutex; 
void * _data;

static void push_data (void * data)
{
	g_mutex_lock (_dmutex);
	_data = data;
	lt_cond_signal (_dcond);
	lt_mutex_unlock (_dmutex);
}

static void * pop_data ()
{
	void * data;

	lt_mutex_lock (_dmutex);
  	while (! _data)
      		lt_cond_wait (_dcond, _dmutex);
  	data = _data;
  	_data = NULL;
  	lt_mutex_unlock (_dmutex);
  	return data;
}

static void * _main1(void * user_data)
{
	int i;

	lt_thread_storage_set(_tls1, GINT_TO_POINTER(0xc0ffee));
	g_print("%p: %p\n", (void *)lt_thread_self(), lt_thread_storage_get(_tls1)); 
	
	lt_mutex_lock(_mutex1);

	for(i = 0; i < 3; i++)
	{
		push_data(GINT_TO_POINTER(i));
		sleep(1);
	}

	lt_thread_exit(NULL);
	return NULL;
}	

int main(int argc, char ** argv)
{
	int i;

	LThread t1;
	
	_mutex1 = lt_mutex_create();
	_dmutex = lt_mutex_create();
	_dcond = lt_cond_create();
	_data = NULL;

	_tls1 = lt_thread_storage_create(NULL);	
	lt_thread_storage_set(_tls1, GINT_TO_POINTER(0xdeadbeef));

	g_print("%p: %p\n", (void *)lt_thread_self(), lt_thread_storage_get(_tls1)); 

	lt_mutex_lock(_mutex1);

	g_assert(lt_thread_create(&t1, _main1, NULL, TRUE));

	lt_mutex_unlock(_mutex1);

	for(i = 0; i < 2; i++)
	{
		g_print("%s: pop_data() = %d\n", __FUNCTION__, GPOINTER_TO_INT(pop_data()));
	}

	lt_thread_join(t1);

	lt_thread_storage_destroy(_tls1);

	return 0;
}

