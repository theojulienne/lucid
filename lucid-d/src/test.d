import lucid.all;

import std.stdio;

void dumpTypes( Type t, int indent=0 )
{
	char[] prefix = "";
	for ( int a = 0; a < indent; a++ ) prefix ~= "    ";
	
	writefln( "%s%s", prefix, t );
	
	writefln( "%s  Event descriptors:", prefix );

	foreach ( e; t.events )
	{
		writefln( "%s    %s", prefix, e );
	}

	if ( t.subtypes.length == 0 )
		return;
	
	writefln( "%s  Derived types:", prefix );
	
	foreach ( sub; t.subtypes )
	{
		dumpTypes( sub, indent+1 );
	}
}

void main( )
{
	Application.init( );
	
	Type base = Type.get( "BaseObject" );
	
	assert( base !is null, "Base object type could not be found!" );
	
	dumpTypes( base );
	
	writefln( "Now lets attempt to do some stuff with objects as if we were in C..." );
	
	Button_ID oref = Button_create( "first" );
	
	writefln( "Object was created through C API, ref: %s", oref );
	
	writefln( "Now lets send that object back to an object's constructor..." );
	
	Button_ID oref2 = Button_create_with_image( "second", oref );
	
	writefln( "Time to unref the C copy of the second object..." );
	
	object_unref( oref2 );
	
	writefln( "Cheating and running a full collect" );
	
	std.gc.fullCollect( );
	
	writefln( "Running application..." );
	
	Application.run( );
}
