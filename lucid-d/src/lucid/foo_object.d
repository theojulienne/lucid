module lucid.foo_object;

import lucid.base_object;
import lucid.member_object;

import std.stdio;

class Button : MemberObject
{
	string title;
	BaseObject image;
	
	this( string t )
	{
		title = t;
		writefln( "Button: %s", title );
	}
	
	this( string t, BaseObject i )
	{
		this( t );
		image = i;
		writefln( "\tcreated with an image as well: %s", i );
	}
	
	~this( )
	{
		writefln( "I'm being killed! (t: %s)", title );
	}
}

mixin DeclareLucidType!(
	Button,
	
	DefInit!( void function( string ) ),
	DefInit!( void function( string, BaseObject ), "with_image" )
);
