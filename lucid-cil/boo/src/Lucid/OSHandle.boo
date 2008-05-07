namespace Lucid

import System
import System.IO
import System.Runtime.InteropServices


[StructLayout (LayoutKind.Sequential)]
class OSHandle(ValueType):
    
    public Handle as IntPtr
    public HandleKind as HandleKind
        
    def constructor(handle as int):
        Handle = cast(IntPtr, handle)
        HandleKind = HandleKind.FD

    def constructor(handle as IntPtr):
        Handle = handle
        HandleKind = HandleKind.FD

    def constructor(handle as IntPtr, kind as HandleKind):
        Handle = handle
        HandleKind = kind
        
    override def ToString() as string:
        return String.Format("{0:x2} [{1}]", Handle, HandleKind)

    public static def CreateStream(handle as OSHandle, owns_handle as bool) as Stream:
        return Native.Factory.CreateStream(handle, owns_handle)

    //TODO
      /*  public static implicit operator IntPtr(OSHandle handle)
        {
            return handle.Handle;
        }
      */



