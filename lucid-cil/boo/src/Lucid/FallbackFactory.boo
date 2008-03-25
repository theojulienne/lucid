namespace Lucid

import System
import System.IO


internal class FallbackLogger(ILogger):
    
    def WriteDebug(message as string, args as (object)):
        Console.Out.WriteLine(message, args)
        
    def WriteWarning(message as string, args as (object)):
        Console.Error.WriteLine(message, args)


internal class FallbackFactory(INativeFactory):
    
    static Logger = FallbackLogger()
    
    def GetLoop() as IEventLoop:
        raise NotImplementedException()
        
    def GetLogger() as ILogger:
        return Logger
            
    def CreateStream(handle as OSHandle, owns_handle as bool) as Stream:
        raise NotImplementedException()
        
    def CreateWaitEvent() as IWaitEvent:
        raise NotImplementedException()

