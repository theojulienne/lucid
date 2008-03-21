namespace Lucid

import System
import System.Threading

#FIXME: static classes in Boo?
abstract class Global:

    private static main as Thread = null

    public static Thread MainThread:
        internal set:
            main = value
        get:
            return main
