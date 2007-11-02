#include <boost/function.hpp>
#include <boost/bind.hpp>

class A
{
public:
	void Foo()
	{
		printf("%s()\n", __FUNCTION__);
	}
};

static void foo()
{
	printf("%s()\n", __FUNCTION__);
}

int main(int argc, char ** argv)
{
	A a;
	boost::function<void ()> f;
	f = boost::bind(&A::Foo, &a);
	f();
	//f = &foo;
	//f();
	return 0;
}
