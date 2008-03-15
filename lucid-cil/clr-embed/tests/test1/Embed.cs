using System;
using System.Runtime.InteropServices;

namespace Embed 
{
    //Is this needed? It fails both ways..
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void BarHandler(IntPtr gc_handle);
  
    public class Foo
    {
        static Foo()
        {
            Console.WriteLine("static Foo(type = {0:x})", 
                (int)typeof(Foo).TypeHandle.Value);         
        }

        public void Bar(BarHandler h)
        {
            Console.WriteLine("Foo.Bar()");
            h(IntPtr.Zero);
        }

        public object Blick()
        {
            return new int[] { 12 };
        }

        ~Foo()
        {
            Console.WriteLine("Foo.Finalize()");
        }

        public void Quux(IntPtr fn_ptr)
        {
            Console.WriteLine("Foo.Quux()");
            BarHandler h = (BarHandler)Marshal.GetDelegateForFunctionPointer(fn_ptr, typeof(BarHandler));
            h(IntPtr.Zero);
        }
	}
}

