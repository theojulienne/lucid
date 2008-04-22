import System
import System.IO

import Lucid

class EvtLoopTest:

    s as Stream = null

    def constructor():
        h = OSHandle(cast(IntPtr, 0))
        self.s = OSHandle.CreateStream(h, false)
        Application.AddWatch(h, WatchEventKind.In, DataReceived) 

    def DataReceived(source, handle as OSHandle, events as WatchEventKind):
        b = self.s.ReadByte()
        if b == cast(int, char('q')):
            Application.Quit()


def Main(args as (string)):
    Application.Init(args)
    Application.Debug = true
    
    EvtLoopTest()
   
    Application.Run()
