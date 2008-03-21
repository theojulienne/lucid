namespace Lucid

import System
import System.Reflection
import System.Threading


public abstract class Application:

    static def Init(args as (string)):
        Global.MainThread = Thread.CurrentThread
        Native.Load()
            
    static def Init():
        Init(null)

    static public Debug = false

    static def WriteDebug(message, *args):
        if Debug:
            Native.Factory.GetLogger().WriteDebug(message, args)

    static def WriteWarning(message, *args):
        if Debug:
            Native.Factory.GetLogger().WriteWarning(message, args)

    static def Run():
        Native.Factory.GetLoop().Run()

    static def Quit(done as bool):
        loop = Native.Factory.GetLoop()
        loop.Quit()
        //FIXME: This should happen auto-magically per-thread
        if done:
            loop.Dispose()

    static def Quit():
        Quit(true)

    static def AddWatch(handle as OSHandle, events as WatchEventKind, handler as WatchHandler) as object: 
        return Native.Factory.GetLoop().AddWatch(handle, events, handler)

    static def AddTimeout(timeout as uint, handler as TimeoutHandler) as object:
        return Native.Factory.GetLoop().AddTimeout(timeout, handler)
                

