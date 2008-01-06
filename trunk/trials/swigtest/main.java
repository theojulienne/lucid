//import Foo;
//import Foot;

public class main {
	static {
		System.loadLibrary("vtest");
	}
	
	public static void jbaz( Foo f, int a, int b )
	{
		f.bar( a, b );
		f.boo( a, b );
		f.bee( a, b );
	}
	
	public static void main( String argv[] ) {
		Foot f = new Foot();
		vtest.baz( f, 3, 4 );
		jbaz( f, 1, 2 );
		
		Foo g = vtest.makeAFoo( );
		vtest.baz( g, 5, 6 );
		jbaz( g, 7, 8 );
	}
}