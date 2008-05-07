using System;
using System.IO;

using Mono.Unix;
using Lucid;

[assembly: NativeFactory(typeof(Lucid.Native.LinuxNative))]

namespace Lucid.Native
{
    public class WaitEventPipe: IWaitEvent
    {
        UnixStream read_pipe, write_pipe;

        public WaitEventPipe()
        {
            UnixPipes pipes = UnixPipes.CreatePipes();
            this.read_pipe = pipes.Reading;
            this.write_pipe = pipes.Writing;
        }

        ~WaitEventPipe()
        {
            this.Dispose();
        }

        public void Dispose()
        {
            if(read_pipe != null && write_pipe != null)
            {
                read_pipe.Dispose();
                write_pipe.Dispose();
                read_pipe = write_pipe = null;
            }
        }

        public void Set()
        {
            this.write_pipe.WriteByte(0x69);
        }

        public void Reset()
        {
            this.read_pipe.ReadByte();
        }

        public OSHandle Handle
        {
            get
            {
                return new OSHandle(this.read_pipe.Handle);
            }
        }
    }

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
