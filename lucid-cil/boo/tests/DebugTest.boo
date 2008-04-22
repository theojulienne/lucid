import System

import Lucid

class Foo(Lucid.Object):
    pass
    
def Main(args as (string)):
    Application.Init(args)
    Application.Debug = true
    f = Foo()
    print(f)
    Application.WriteWarning("f is {0}", f)
        
        
