#include <boost/signals.hpp>

static void hello()
{
	printf("Hello, World!\n");
}

int main(int argc, char ** argv)
{
	boost::signal<void ()> * sig = new boost::signal<void ()>();
	
	sig->connect(&hello);

	sig();

	delete sig;

	return 0;
}
