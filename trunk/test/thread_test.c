#include <lucid-base.h>
#include <unistd.h>

LMutex _mutex1 = LT_MUTEX_STATIC_INIT;

static void * _main1(void * user_data)
{
	sleep(1);
	lt_thread_exit(NULL);
	return NULL;
}	

int main(int argc, char ** argv)
{
	LThread t1;
	
	lt_mutex_lock(&_mutex1); lt_mutex_trylock(&_mutex1);

	g_assert(lt_thread_create(&t1, _main1, NULL, TRUE));

	lt_thread_join(t1);

	return 0;
}
