namespace Lucid

import System
import System.Runtime.InteropServices


[StructLayout (LayoutKind.Sequential)]
class WatchEvent(ValueType):
    
    def constructor(handle as OSHandle, events as WatchEventKind):
        self.Handle = handle
        self.Events = events

    public Handle as OSHandle
    public Events as WatchEventKind

