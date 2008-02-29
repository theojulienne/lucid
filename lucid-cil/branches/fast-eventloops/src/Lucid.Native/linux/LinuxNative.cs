using System;
using System.IO;
using System.Collections.Generic;

using Mono.Unix;
using Lucid;

[assembly: NativeFactory(typeof(Lucid.Linux.Native))]

namespace Lucid.Linux
{
    internal class EPollSourceID: Lucid.GLib.SourceID
    {   
        public EPollSourceID(uint id): base(id)
        {
        }
    }
    
    //FIXME: HACK
    internal class GMainLoopWithPoll: Lucid.GLib.GMainLoop
    {
        private IPoll poll;
        private object poll_source;
        private Dictionary<IntPtr, object[]> poll_handlers;          
        
        public GMainLoopWithPoll()
        {
            poll = new EPoll();    
            Console.WriteLine("GMainLoopWithPoll() {0}", (IntPtr)poll.Handle);
            poll_source = base.AddWatch(poll.Handle, 
                WatchEventKind.In, delegate { Dispatch(); });
            poll_handlers = new Dictionary<IntPtr, object[]>();
        }
        
        protected override void Dispose(bool disposing)
        {
            Console.WriteLine("GMainLoopWithPoll.Dispose()");
            base.RemoveSource(poll_source);
            
            if(disposing)
                poll.Dispose();
            
            poll = null;
            
            base.Dispose(disposing);
        }
        
        private void Dispatch()
        {
            //FIXME: Why is this needed!?
            if(poll == null)
                return;

            Console.WriteLine("Dispatch({0}) {1}", poll, poll.Buffer.Count);
            
            poll.WaitAny(0); 
                        
            for(int i = 0; i < poll.Buffer.Count; i++)
            {
                try
                {
                    WatchEvent evt = poll.Buffer[i];
                    if((int)evt.Handle.Handle > 100)
                        continue;
                    object[] closure = poll_handlers[(IntPtr)evt.Handle];
                    WatchHandler handler = (WatchHandler)closure[1];
                    handler(closure[0], evt.Handle, evt.Events);
                }
                catch(Exception ex)
                {
                    Console.WriteLine("Dispatch() FAIL {0}", ex);
                }
            }
        }
        
        public override object AddWatch(OSHandle handle, WatchEventKind events, 
            WatchHandler handler)
        {
            object[] closure = new object[2];
            
            closure[0] = new EPollSourceID((uint)(int)(IntPtr)handle);
            closure[1] = handler;
            
            poll.AddWatch(handle, events);
            poll_handlers[(IntPtr)handle] = closure;
            
            Console.WriteLine("{0} = {1}", (IntPtr)handle, closure.GetHashCode());            
            
            return closure[0];
        }
            
        public override void UpdateWatch(object source, WatchEventKind events)
        {
            if(! (source is EPollSourceID))
                throw new ArgumentException("source");
            
            uint id = ((EPollSourceID)source).ID;
            poll.UpdateEvents(new OSHandle((IntPtr)(int)id, HandleKind.FD), events);
        }
        
        public override void RemoveSource(object source)
        {
            if(source is EPollSourceID)
            {
                uint id = ((EPollSourceID)source).ID;
                poll.RemoveWatch(new OSHandle((IntPtr)id, HandleKind.FD));
                poll_handlers[(IntPtr)id] = null;
            }
            else
                base.RemoveSource(source);
        }
    }
    
    public class Native: INativeFactory
    {
        private static INativeFactory singleton = null;
    
        public static INativeFactory Get()
        {
            if(singleton == null)
                singleton = new Native();
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
    }    
}
