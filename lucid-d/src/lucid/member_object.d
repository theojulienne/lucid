module lucid.member_object;

import lucid.event_object;
 
class MemberObject : EventObject
{
	MemberObject parent;
	
	this( MemberObject p )
	{
		super( );
		parent = p;
	}
	
	this( )
	{
		this( null );
	}
}

mixin DeclareLucidType!(
	MemberObject,
	
	DefInit!( void function(MemberObject) ),
	
	DefEvent!( "parent_attached" )
);
