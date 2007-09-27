using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Lucid.Base
{
	public class LType
	{
		[DllImport(Global.BaseLib)]
		internal static extern string lt_type_get_name(IntPtr raw);	

		[DllImport(Global.BaseLib)]
		internal static extern IntPtr lt_type_from_name(string type_name);	

		internal static LType LoadType(string type_name)
		{
			return null;
		}
	}
}
