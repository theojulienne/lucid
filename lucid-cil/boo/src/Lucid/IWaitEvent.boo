namespace Lucid

import System


//TODO: Is this complete and/or does it make sense?
interface IWaitEvent(IDisposable):
    def Set()
    def Reset()
    
    Handle as OSHandle:
        get
        
    
