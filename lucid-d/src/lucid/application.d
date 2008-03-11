module lucid.application;

extern (C)
{
	void lucid_native_application_init( );
	void lucid_native_application_run( );
}

class Application
{
	static void init( string args )
	{
		lucid_native_application_init( );
	}
	
	static void init( )
	{
		init( null );
	}
	
	static void run( )
	{
		lucid_native_application_run( );
	}
}
