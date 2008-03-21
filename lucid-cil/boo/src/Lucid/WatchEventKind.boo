namespace Lucid

import System


[Flags]
enum WatchEventKind:
    None
    In
    Out
    Err
    Hup
    //FIXME: Boo doesn't support this..(?)
    //All = In | Out | Err | Hup    


