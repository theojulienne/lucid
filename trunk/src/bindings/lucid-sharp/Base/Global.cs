using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Lucid.Base
{
	internal static class Global
	{
		[DllImport(Global.BaseLib)]
		internal static extern void lt_type_init();

		public const string GraphicsLib = "lt-graphics";
        	public const string BaseLib = "lt-base";

		public static LModule BaseModule = null;
		
		static Global()
		{
			//TODO- Decide whether this should be in Application.Init() or not.
			lt_type_init();		
			BaseModule = LModule.Load(BaseLib);
		}
	}
}
