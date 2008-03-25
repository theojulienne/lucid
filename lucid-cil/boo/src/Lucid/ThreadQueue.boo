namespace Lucid

import System
import System.Collections
import System.Threading

    /*
        FIXME: This kind of functionality needs to be tied into BaseObject
               Look at the Dispatcher crap in WPF for ideas...
               (THIS IS ALL DISGUSTING TOO .. :-P)
    */
final class ThreadQueue(IDisposable):

    olock = object()
    msgs = Queue()
    disposed = false
    evt as IWaitEvent;
    loop as IEventLoop;        
    msg_src as object
       
    public MessageReceived as Action of object
        
    def constructor():
        self(Native.Factory.GetLoop())
 
    def constructor(the_loop):
        self.loop = the_loop
        self.evt = Native.Factory.CreateWaitEvent()
        self.msg_src = self.loop.AddWatch(self.evt.Handle, WatchEventKind.In, self.OnPingReceived)

    def destructor():
        self.Dispose(false)
        
    public def Dispose():
        self.Dispose(true)
        
    def Dispose(disposing as bool):
        try:
            if self.disposed:
                return
            
            if disposing:
                GC.SuppressFinalize(self)

            self.loop.RemoveSource(self.msg_src)                

            while self.msgs.Count > 0:
                Dispatch()

            self.evt.Dispose()
            self.evt = null
            self.loop = null
                
            self.disposed = true
        except ex:
            raise ex
        ensure:
            Application.WriteDebug("ThreadQueue.Dispose({0}) [Thread {1}]",
                                       disposing, Thread.CurrentThread.GetHashCode())
        
    def OnPingReceived(source, handle as OSHandle, events as WatchEventKind):  
        lock self.olock:
            if self.disposed:
                raise ObjectDisposedException("this")
            Dispatch()
            
    def Dispatch():
        if self.MessageReceived is not null:
            self.MessageReceived(msgs.Dequeue())    
        self.evt.Reset()            

    public def PostMessage(data):
        lock self.olock:
            if self.disposed:
                raise ObjectDisposedException("this")
            self.msgs.Enqueue(data)
            self.evt.Set()

