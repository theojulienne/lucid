using System;

namespace Lucid
{
    internal interface IPoll: IDisposable
    {
        void AddWatch(OSHandle handle, WatchEventKind events);
        void UpdateEvents(OSHandle handle, WatchEventKind events);
        void RemoveWatch(OSHandle handle);
        
        int WaitAny(int timeout);

        IWatchBuffer Buffer
        {
            get;
        }
    }
}
