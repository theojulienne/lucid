import System
import System.IO

import Lucid

class EvtLoopTest:

    s as Stream = null
    tick = 0

    def constructor():
        h = OSHandle(cast(IntPtr, 0))
        self.s = OSHandle.CreateStream(h, false)
        Application.AddWatch(h, WatchEventKind.In, OnDataReceived) 
        Application.AddTimeout(850, OnTimeout)

    def OnTimeout(source):
        self.tick += 1
        if tick > 3:
            print("No more Moo's.. :-(")    
            Application.RemoveSource(source)
            return
        print("Moo! ${self.tick}")    

    def OnDataReceived(source, handle as OSHandle, events as WatchEventKind):
        b = self.s.ReadByte()
        if b == cast(int, char('q')):
            Application.Quit()


def Main(args as (string)):
    Application.Init(args)
    Application.Debug = true
    
    print("Press Q+<return> to exit")
    EvtLoopTest()
   
    Application.Run()

