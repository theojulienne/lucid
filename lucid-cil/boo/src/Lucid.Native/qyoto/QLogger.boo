namespace Lucid.Native

import System

import Lucid

import Qyoto

internal class QLogger(ILogger):
    
    public static Singleton as ILogger = QLogger()

    def WriteDebug(message as string, args as (object)):
        Console.Out.WriteLine(message, *args)
        
    def WriteWarning(message as string, args as (object)):
        Console.Error.WriteLine(message, *args)


