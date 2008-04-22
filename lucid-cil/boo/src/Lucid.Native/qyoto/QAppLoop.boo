namespace Lucid.Native

import System
import System.Threading
import System.Collections
import System.Collections.Generic
import System.Runtime.InteropServices
import System.Runtime.ConstrainedExecution

import Lucid

import Qyoto

class QAppLoop(QObject, IEventLoop):
    
    private static final App as QApplication = QApplication(Environment.GetCommandLineArgs())
    //private static final LoopTLS as LocalDataStoreSlot = System.Threading.Thread.AllocateDataSlot()
    private static final Singleton as IEventLoop = QAppLoop()

    // key int = FD, value (object) = [WatchHandler, QSocketNotifier or int for 'TimerId']
    private watches as Dictionary[of int, (object)] = Dictionary[of int, (object)]()
    private timers as Dictionary[of int, (object)] = Dictionary[of int, (object)]()

    [Q_SLOT]
    def WatchActivated(socket as int):
        watch = self.watches[socket]
        handler as WatchHandler = watch[0]
        noti as QSocketNotifier = watch[1]        
        
        events as WatchEventKind = MapEventsL(noti.type())
        handle as OSHandle = OSHandle(cast(IntPtr, socket))

        handler(watch, handle, events)
    
    def Dispose():
        pass

    def AddWatch(handle as OSHandle, events as WatchEventKind, handler as WatchHandler) as object:
        socket as int = cast(int, handle.Handle)
        if self.watches.ContainsKey(socket):
            raise InvalidOperationException("Handle already added to the event loop.")

        watch = array(object, 2)

        watch[0] = handler

        noti = QSocketNotifier(socket, MapEventsN(events), self)
        Connect(noti, SIGNAL("activated(int)"), self, SLOT("WatchActivated(int)"))
        watch[1] = noti
    
        self.watches[socket] = watch
        return watch
   
    def UpdateWatch(source, events as WatchEventKind):
        noti as QSocketNotifier = null
        watch as (object) = null

        try:
            noti = (source as (object))[1]
        except NullReferenceException:
            raise ArgumentException("Source key is not valid.")
    
        socket = noti.Socket()        
        try:
            watch = self.watches[socket]
        except KeyNotFoundException:
            raise ArgumentException("Source key is not valid.")

        noti.SetEnabled(false)
        noti.Dispose()
        noti = QSocketNotifier(socket, MapEventsN(events), self)
        Connect(noti, SIGNAL("activated(int)"), self, SLOT("WatchActivated(int)"))
        watch[1] = noti

    def AddTimeout(timeout as uint, handler as TimeoutHandler) as object:
        watch = array(object, 2)
        timer_id as int = self.StartTimer(cast(int, timeout))

        watch[0] = handler
        watch[1] = timer_id
    
        self.timers[timer_id] = watch
        return watch

    def RemoveSource(source):
        real_source as (object) = null

        try:
            real_source = cast((object), source)
        except NullReferenceException:
            raise ArgumentException("Source key is not valid.")
       
        if real_source[1] isa QSocketNotifier:
            noti as QSocketNotifier = cast(QSocketNotifier, real_source[1])            
            self.watches.Remove(noti.Socket())
            noti.SetEnabled(false)
            noti.Dispose()
        else:
            timer_id as int = real_source[1]
            self.timers.Remove(timer_id)
            self.KillTimer(timer_id)        

        //For garbage collection                
        real_source[0] = null
        real_source[1] = null

    def Run():
        QCoreApplication.Exec()

    def RunIteration(block as bool) as bool:
        flags = QEventLoop.ProcessEventsFlag.AllEvents
        if block:
            flags |= QEventLoop.ProcessEventsFlag.WaitForMoreEvents
        QCoreApplication.ProcessEvents(cast(uint, flags))

    def Quit():
        QCoreApplication.Exit()

    Pending as bool:
        get:
            return QCoreApplication.HasPendingEvents()

    protected override def TimerEvent(arg as QTimerEvent):
        watch = self.timers[arg.TimerId()]
        handler as TimeoutHandler = watch[0]
        handler(watch)
    
    private static def MapEventsN(events as WatchEventKind) as QSocketNotifier.TypeOf:
        val as QSocketNotifier.TypeOf = cast(QSocketNotifier.TypeOf, 0)
        if (events & WatchEventKind.In) != 0:
            val |= QSocketNotifier.TypeOf.Read
        elif (events & WatchEventKind.Out) != 0:
            val |= QSocketNotifier.TypeOf.Write
        elif ((events & WatchEventKind.Err) != 0) or ((events & WatchEventKind.Hup) != 0):
            val |= QSocketNotifier.TypeOf.Exception
        return val


    private static def MapEventsL(events as QSocketNotifier.TypeOf) as WatchEventKind:
        val as WatchEventKind = WatchEventKind.None
        if (events & QSocketNotifier.TypeOf.Read) != 0:
            val |= WatchEventKind.In;
        elif (events & QSocketNotifier.TypeOf.Write) != 0:
            val |= WatchEventKind.Out
        elif (events & QSocketNotifier.TypeOf.Exception) != 0:
            val |= WatchEventKind.Err
        return val

    // ** The real magic is this function. **
    /*
    static def Get() as IEventLoop:
        loop = System.Threading.Thread.GetData(LoopTLS)   
        if loop is null:
            loop = QAppLoop()
            System.Threading.Thread.SetData(LoopTLS, loop)
        return loop
    */

    static def Get() as IEventLoop:
        if Thread.CurrentThread is not Lucid.Global.MainThread:
            raise NotSupportedException("The Qt4 backend only supports the main thread at this time.")
        return Singleton


