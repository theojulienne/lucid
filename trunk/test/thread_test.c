#include <lucid-base.h>
#include <unistd.h>

static void * _main1(void * user_data)
{
	sleep(1);
	lt_thread_exit(NULL);
	return NULL;
}	

int main(int argc, char ** argv)
{
	LThread t1;
	
	g_assert(lt_thread_create(&t1, _main1, NULL, TRUE));

	lt_thread_join(t1);

	return 0;
}
