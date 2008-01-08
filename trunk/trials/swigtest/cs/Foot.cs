using System;

public class Foot : Foo {
	public override void bar( int a, int b ) {
		Console.WriteLine( "Foot.bar called in C#!" );
	}
	
	public override void boo( int a, int b ) {
		Console.WriteLine( "Foot.boo called in C#!" );
	}
	
	public new void bee( int a, int b ) {
		Console.WriteLine( "Foot.bee called in C#!" );
	}
}