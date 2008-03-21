namespace Lucid

import System


callable WatchHandler(source, handle as OSHandle, events as WatchEventKind)
callable TimeoutHandler(source)

interface IEventLoop(IDisposable):
    def AddWatch(handle as OSHandle, events as WatchEventKind, handler as WatchHandler) as object   
    def UpdateWatch(source, events as WatchEventKind)
    def AddTimeout(timeout as uint, handler as TimeoutHandler) as object
    def RemoveSource(source)       
    def Run()
    def RunIteration(block as bool) as bool
    def Quit()

    Pending as bool:
        get

