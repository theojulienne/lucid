using System;
using System.IO;

using Mono.Unix;
using Lucid;

[assembly: NativeFactory(typeof(Lucid.Native.LinuxNative))]

namespace Lucid.Native
{
    public class LinuxNative: INativeFactory
    {
        private static INativeFactory singleton = null;
    
        public static INativeFactory Get()
        {
            if(singleton == null)
                singleton = new LinuxNative();
            return singleton;
        }

        public IEventLoop GetLoop()
        {
            return Lucid.GLib.GMainLoop.Get();
        }

        public ILogger GetLogger()
        {
            return GLogger.Get();
        }

        public Stream CreateStream(OSHandle handle, bool owns_handle)
        {
            if(handle.HandleKind == HandleKind.FD)
                return new UnixStream((int)handle.Handle, owns_handle);
            else
                throw new ArgumentException(String.Format("Handle of type \"{0}\" not supported by {1}", handle.HandleKind, GetType()));
        }
        
        public IWaitEvent CreateWaitEvent()
        {
            return new WaitEventPipe();
        }
    }    
}
