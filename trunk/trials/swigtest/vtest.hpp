class Foo
{
public:
	void bee( int a, int b );
	virtual void boo( int a, int b );
	virtual void bar( int a, int b )=0;
};

void baz( Foo *f, int a, int b );

Foo *makeAFoo( );
