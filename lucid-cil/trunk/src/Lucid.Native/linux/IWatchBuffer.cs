using System;
using System.Collections.Generic;

namespace Lucid
{
    internal interface IWatchBuffer: IDisposable
    {
        WatchEvent this [int index]
        {
            get;
        }

        int Count
        {
            get;
        }   
    }    
}
