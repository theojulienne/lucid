using System;

namespace Lucid.GLib
{
    internal static class Global
    {
        //This code *can* be used without mono, but this avoids a .config file.
		public const string GLibDLL = "__Internal"; //"libglib-2.0-0.dll";
    }
}
