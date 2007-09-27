using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Lucid.Base
{
	//TODO- Maybe all of this crud should actually go in liblt-base LModule.cpp?
	// LModule can just be a struct with the actually Handle ID as its only member. 
	// Lets make LModule no longer an LBase sub-class...I like this "keep me already loaded thing."
	// Maybe the OS will do this anyway? Also, finish implementing "LoadFromProcess()"
	sealed public class LModule: LBase, IDisposable
	{
		[DllImport(Global.BaseLib)]
		private static extern IntPtr lt_module_load(string file_name);

		[DllImport(Global.BaseLib)]
		private static extern IntPtr lt_module_get_name(IntPtr handle);

		[DllImport(Global.BaseLib)]
		private static extern IntPtr lt_module_get_symbol(IntPtr handle, string symbol_name);

		static Dictionary<string, WeakReference> _modules = new Dictionary<string, WeakReference>();

		string m_name;

		internal LModule(IntPtr handle): base(handle)
		{
			m_name = Marshal.PtrToStringAnsi(lt_module_get_name(Handle));
			if(m_name == null)
				throw new InvalidOperationException();
		}

		protected override void CleanupInternal()
		{
			if(Handle != IntPtr.Zero)
			{
				_modules.Remove(m_name);
				Console.WriteLine("LModule.CleanupInternal({0})", m_name);
			}
		}
	
		public static LModule Load(string file_name)
		{
			//TODO- LError support (?), verify existence before-hand ?
			if(_modules.ContainsKey(file_name))
			{
				WeakReference weakref = _modules[file_name];
				if(weakref.IsAlive)
					return weakref.Target as LModule;
			}
			IntPtr handle = lt_module_load(file_name);
			if(handle == IntPtr.Zero)
                                throw new ArgumentException("file_name");
			
			LModule module = new LModule(handle);
			_modules.Add(file_name, new WeakReference(module));
			return module;
		}

		public string Name
		{
			get
			{
				return m_name;
			}
		}
			
		public IntPtr GetSymbol(string symbol_name)
		{
			return lt_module_get_symbol(Handle, symbol_name);
		}		
	}
}

