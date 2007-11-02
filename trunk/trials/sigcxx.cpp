#include <sigc++/sigc++.h>

typedef sigc::slot_base Slot;

static void slot_disconnect(Slot * slot)
{
	if(slot)
		slot->disconnect();
}

static void * _destroy_fn(void * user_data)
{
	printf("%s(%p)\n", __FUNCTION__, user_data);
	return NULL;
}

static void hello(void * user_data)
{
	printf("Hello, World! [%p]\n", user_data);
}

static Slot * connect_it(sigc::signal<void> * sig, void (* signal_fn) (void *), 
	void * user_data, void * (* val_free_fn) (void *))
{
	sigc::connection c = sig->connect(sigc::bind(sigc::ptr_fun(signal_fn), user_data));	
	Slot ** slot = (Slot **)&c;
	if(val_free_fn)	
		(* slot)->add_destroy_notify_callback(user_data, val_free_fn);
	return * slot;
}

class Foo: public sigc::trackable
{
public:
	Foo();
	virtual ~Foo();
	void Hello();
};

Foo::Foo()
{
	printf("%s\n", __FUNCTION__);
}

Foo::~Foo()
{
	printf("%s\n", __FUNCTION__);
}

void Foo::Hello()
{
	printf("%s(%p)\n", __FUNCTION__, this);
}

int main(int argc, char ** argv)
{
	sigc::signal<void> * sig = new sigc::signal<void>();
	Foo * foo = new Foo();

	for(int i = 1; i < 4; i++)
		connect_it(sig, hello, (void *)i, _destroy_fn);	

	sig->connect(sigc::mem_fun(foo, &Foo::Hello));

	//slot_disconnect(s);

	delete foo;
	
	sig->emit();

	delete sig;
	return 0;
}

