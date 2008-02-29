import System
import System.Runtime.InteropServices

o = Object()

gch = cast(IntPtr, GCHandle.Alloc(o))
print(gch)

gch = cast(IntPtr, GCHandle.Alloc(o))
print(gch)

