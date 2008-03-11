module lucid.base_object;

public import lucid.types;

class BaseObject : WrappableObject
{
	this( )
	{
		super( );
	}
}

mixin DeclareLucidType!(
	BaseObject,
	
	DefInit!( void function() )
);