module lucid.event_object;

public import lucid.base_object;

import std.stdio;
import std.traits;

struct EventDescriptorS
{
	string name;
	EventObject _delegate;
	
	void opAddAssign( int a )
	{
		_delegate.addhandler( name, a );
	}
	
	string toString( )
	{
		return "EventDescriptorS<" ~ name ~ ">";
	}
}

class EventObject : BaseObject
{
	int a = 0;
	
	EventDescriptorS onmousedown = EventDescriptorS( "mousedown" );
	
	this( )
	{
		super( );
		
		auto tt = FieldTypeTuple!(typeof(this));
		foreach (i, v; tt) {
			static if ( is(typeof(v) == EventDescriptorS) )
			{
				this.tupleof[i]._delegate = this;
				writefln( "%s, %s", i, v );
			}
		}
		
		writefln( "%s", this.onmousedown );
		
		this.onmousedown += 1;
	}
	
	void addhandler( string a, int b )
	{
		writefln( "Addhandler: %s, %s", a, b );
	}
}

mixin DeclareLucidType!(
	EventObject,
	
	DefInit!( void function() ),
	
	DefEvent!( "created" ),
	DefEvent!( "destroyed" )
);
