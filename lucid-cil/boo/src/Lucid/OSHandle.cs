using System;
using System.Runtime.InteropServices;

namespace Lucid
{
    [StructLayout (LayoutKind.Sequential)]
    public struct OSHandle
    {
        public OSHandle(IntPtr handle)
        {
            Handle = handle;
            HandleKind = HandleKind.FD;
        }

        public OSHandle(IntPtr handle, HandleKind kind)
        {
            Handle = handle;
            HandleKind = kind;
        }
        
        public override string ToString()
        {
            return String.Format("{0:x2} [{1}]", Handle, HandleKind);
        }

        public static implicit operator IntPtr(OSHandle handle)
        {
            return handle.Handle;
        }

        public IntPtr Handle;
        public HandleKind HandleKind;   
    }
}
