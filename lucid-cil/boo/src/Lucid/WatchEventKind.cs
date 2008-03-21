using System;

namespace Lucid
{
    [Flags]
    public enum WatchEventKind
    {
        None,
        In,
        Out,
        Err,
        Hup,
        All = (In | Out | Err | Hup)    
    }
}
