namespace Lucid

import System
import System.Runtime.InteropServices

[StructLayout (LayoutKind.Sequential)]
class OSHandle(ValueType):
    
    public Handle as IntPtr
    public HandleKind as HandleKind
        
    def constructor(handle):
        Handle = handle
        HandleKind = HandleKind.FD

    def constructor(handle, kind):
        Handle = handle
        HandleKind = kind
        
    override def ToString() as string:
        return String.Format("{0:x2} [{1}]", Handle, HandleKind)

      /*  public static implicit operator IntPtr(OSHandle handle)
        {
            return handle.Handle;
        }
      */



