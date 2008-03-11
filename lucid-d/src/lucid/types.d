module lucid.types;

import lucid.events;

import std.string;

import pyd.lib_abstract :
    symbolnameof,
    prettytypeof,
    toString,
    ParameterTypeTuple,
    ReturnType,
    minArgs,
    objToStr,
    ToString
;

typedef int ObjectID;

class WrappableObject
{
	private:
	int _wrapper_ref_count = 0;
	ObjectID objectid = -1;
	Type type;
	
	public:
	this( )
	{
		type = Type.get( this.classinfo.name );
	}
}

class Type
{
	string name;
	Type super_type;
	Type[string] subtypes;
	EventDescriptor[string] _events;
	EventList events;
	
	this( string _name, Type _sup_type )
	{
		events = new EventList;
		
		name = _name;
		super_type = _sup_type;
		
		if ( _sup_type !is null )
		{
			// we have a super type, list ourselves there
			_sup_type.subtypes[name] = this;
		}
	}
	
	
	string toString( )
	{
		return "Type<" ~ name ~ ">";
	}
	
	EventDescriptor addEventDescriptor( string name )
	{
		_events[name] = new EventDescriptor( name );
		return _events[name];
	}
	
	class EventList
	{
		int opApply( int delegate(inout EventDescriptor) dg )
		{
			int result = 0;
		
			foreach ( e; _events )
			{
				result = dg( e );
				if (result)
					break;
			}
			
			if ( super_type !is null )
			{
				foreach ( e; super_type.events )
				{
					result = dg( e );
					if (result)
						break;
				}
			}
		
			return result;
		}
	}
	
	
	static Type[string] types;
	
	static Type create( string n, Type s )
	{
		Type t = new Type( n, s );
		
		types[t.name] = t;
		
		return t;
	}
	
	static Type get( string t )
	{
		// strip off everything before the last dot in case we are passed an absolute module path
		int dot = std.string.rfind( t, '.' );
		if ( dot > -1 )
		{
			t = t[dot+1..$];
		}
		
		return types[t];
	}
	
	
	// this is a poor implementation of object ids, because...
	static WrappableObject[ObjectID] obj_maps;
	static ObjectID next_objid = 1; //  ... obviously these need to be re-used or we're maxed at 1<<31 objects during a programs entire run time
	
	static ObjectID getObjectID( WrappableObject obj, bool inc_ref=false )
	{
		if ( obj.objectid == -1 )
		{
			obj.objectid = next_objid++;
			obj_maps[obj.objectid] = obj;
		}
		
		if ( inc_ref )
			obj._wrapper_ref_count++;
		
		return obj.objectid;
	}
	
	static WrappableObject getObjectById( ObjectID objid )
	{
		if ( objid <= 0 ) return null;
		
		WrappableObject o = obj_maps[objid];
		
		return o;
	}
}

extern (C)
{
	// called when the calling C code is finished with its reference.
	void object_unref( ObjectID objid )
	{
		WrappableObject o = Type.getObjectById( objid );
		
		if ( o !is null )
		{
			if ( --o._wrapper_ref_count <= 0 )
			{
				Type.obj_maps.remove( objid );
			}
		}
	}
}

import std.stdio;

// some of the code/theory below is borrowed from pyd, because it just .. works

template is_wrapped(T) {
    bool is_wrapped = false;
}

template DeclareLucidType( T, Params... )
{
	static const char[] type_name = symbolnameof!(T);
	
	static this( )
	{
		Type base_type = null;
		
		static if (is(T B == super))
		{
			foreach (C; B)
			{
				static if (is(C == class) && !is(C == Object))
				{
					if (is_wrapped!(C))
						base_type = Type.get( symbolnameof!(C) );
	            }
				else if (is(C == interface))
				{
					std.stdio.writefln( "(FIXME) found interface: %s", symbolnameof!(C) );
				}
	        }
	    }
		
		auto t = Type.create( type_name, base_type );
		
		// remember we are a wrapped class
		is_wrapped!(T) = true;
	}
	
	mixin( "alias ObjectID " ~ type_name ~ "_ID;" );
	mixin GenerateWrapperFunctions!( T, Params );
}

template GenerateWrapperFunctions( T ) {}
template GenerateWrapperFunctions( T, Param, Params... )
{
	mixin( Param.gen_wrapper!( T ) );
	//pragma(msg,Param.gen_wrapper!( T ));
	mixin GenerateWrapperFunctions!( T, Params );
}


struct DefInit( fn_t, char[] suffix="" )
{
	alias ParameterTypeTuple!(fn_t) param_types;
	
	template wrapper_func_name( T )
	{
		static char[] wrapper_func_name( )
		{
			char[] func_name = symbolnameof!(T) ~ "_create";
			static if ( suffix != "" )
			{
				func_name ~= "_" ~ suffix;
			}
			return func_name;
		}
	}
	
	template wrapper_args( T, bool typesig )
	{
		static char[] wrapper_args( )
		{
			char[] args = "";
			
			foreach ( n, type; param_types )
			{
				static if ( n > 0 )
					args ~= ", ";
				
				static if ( typesig )
				{
					static if ( is( type == class ) )
					{
						args ~= "ObjectID ";
					}
					else
					{
						args ~= prettytypeof!(type) ~ " ";
					}
				}
				
				static if ( !typesig && is( type == class ) )
					args ~= "cast("~symbolnameof!(type)~")Type.getObjectById( t_" ~ ToString!(n) ~ " )";
				else
					args ~= "t_" ~ ToString!(n);
			}
			
			return args;
		}
	}
	
	template gen_wrapper( T )
	{
		const char[] gen_wrapper = "extern(C) " ~ symbolnameof!( T ) ~ "_ID " ~ wrapper_func_name!( T ) ~ "( " ~ wrapper_args!(T, true) ~ " ) {
			T foo = new T( " ~ wrapper_args!(T, false) ~ " );
			return cast(" ~ symbolnameof!( T ) ~ "_ID)Type.getObjectID( foo, true );
		}";
	}
}

struct DefEvent( string event_name )
{
	template gen_wrapper( T )
	{
		const char[] gen_wrapper = "
		static this( )
		{
			Type t = Type.get( \"" ~ symbolnameof!( T ) ~ "\" );
			auto ed = t.addEventDescriptor( \"" ~ event_name ~ "\" );
		}
		";
	}
}
