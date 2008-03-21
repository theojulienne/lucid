using System;
using System.Runtime.InteropServices;

namespace Lucid
{
    [StructLayout (LayoutKind.Sequential)]
    public struct WatchEvent
    {
        public WatchEvent(OSHandle handle, WatchEventKind events)
        {
            Handle = handle;
            Events = events;
        }

        public OSHandle Handle;
        public WatchEventKind Events;
    }
}
