namespace Lucid

import System
import System.Threading

#FIXME: static classes in Boo?
abstract class Global:

    static private main as Thread = null

    static public MainThread as Thread:
        internal set:
            main = value
        get:
            return main
