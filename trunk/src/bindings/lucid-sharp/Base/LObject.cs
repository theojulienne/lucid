using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Lucid.Base
{
	public class LObject: LBase
	{
		[DllImport(Global.BaseLib)]
		private static extern IntPtr lt_object_get_type(IntPtr raw);

		private static readonly Dictionary<string,Type> _claro_types = GetTypes();
		private static Dictionary<string,Type> GetTypes()
		{
			Dictionary<string,Type> hash = new Dictionary<string,Type>();
			foreach(Type t in typeof(Object).Assembly.GetTypes())
				hash[t.Name] = t;
			return hash;
		}

		private static Dictionary<IntPtr, WeakReference> _objects = 
			new Dictionary<IntPtr, WeakReference>();

		internal static LObject Wrap(IntPtr raw)
		{
			if(raw == IntPtr.Zero)
				return null;

			WeakReference weak_ref = _objects[raw];
			
			if(weak_ref != null && weak_ref.IsAlive)
				return weak_ref.Target as LObject;
			else
			{
				LObject obj = CreateObject(raw);
				if(obj == null)
					return null;
				_objects[raw] = new WeakReference(obj);	
				return obj;
			}
		}

		private static LObject CreateObject(IntPtr raw)
		{
			Type type = null;
			try
			{
				IntPtr _type = lt_object_get_type(raw);
				if(_type == IntPtr.Zero) 
					throw new InvalidCastException(); //FIXME
				string name = LType.lt_type_get_name(_type);				
				type = _claro_types[name]; 
			}
			catch(System.Collections.Generic.KeyNotFoundException)
			{
				//TODO- Decide on our behavior here (?)
				type = typeof(LObject);
			}
			return type.GetConstructor(new Type[] { typeof(IntPtr) }).
				Invoke(new Object[] { raw }) as LObject;
		}

		public LObject(IntPtr handle): base(handle)
		{
		}

		~LObject()
		{
			_objects.Remove(Handle);
		}
	}
}

