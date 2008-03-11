module lucid.events;

class EventDescriptor
{
	string name;
	
	this( string n )
	{
		name = n;
	}
	
	string toString( )
	{
		return "EventDescriptor<" ~ name ~ ">";
	}
}
