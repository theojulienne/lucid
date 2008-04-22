namespace Lucid.Native

import System
import System.IO

import Mono.Unix

import Lucid

import Qyoto

class QyotoNative(INativeFactory):
    
    private static singleton as INativeFactory = QyotoNative()
    
    static def Get() as INativeFactory:
        return singleton

    def GetLoop() as IEventLoop:
        return QAppLoop.Get()

    def GetLogger() as ILogger:
        return QLogger.Singleton

    def CreateStream(handle as OSHandle, owns_handle as bool) as Stream:
        if handle.HandleKind == HandleKind.FD:
            return UnixStream(cast(int, handle.Handle), owns_handle)
        else:
            raise ArgumentException(String.Format("Handle of type \"{0}\" not supported by {1}", handle.HandleKind, GetType()))
        
        //FIXME: WHERE DID WaitEventPipe.cs GO!?
    def CreateWaitEvent() as IWaitEvent:
        raise NotSupportedException()        
        return null

//Has to be done here for some reason.. (?)
[assembly: NativeFactory(typeof(Lucid.Native.QyotoNative)) ]

