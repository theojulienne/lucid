namespace Lucid

import System
import System.IO


interface INativeFactory:
    def GetLoop() as IEventLoop
    def GetLogger() as ILogger
    def CreateStream(handle as OSHandle, owns_handle as bool) as Stream
    def CreateWaitEvent as IWaitEvent
