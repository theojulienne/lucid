using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using Mono.Unix;
using Mono.Unix.Native;

namespace Lucid
{ 
    //epoll() 
    internal class EPoll: IPoll
    {
        internal static readonly int DefaultSize = 1024;
        internal static readonly int DefaultMultiplier = 4;
        private const int EPOLL_CTL_ADD = 1;
        private const int EPOLL_CTL_DEL = 2;	
        private const int EPOLL_CTL_MOD = 3;	
        private const int EPOLLIN = 0x001;
        private const int EPOLLOUT = 0x004;
        private const int EPOLLERR = 0x008;
        private const int EPOLLHUP = 0x010;

        [DllImport("libc.so.6")]
        private static extern int epoll_create(int size);

        [DllImport("libc.so.6")]
        private static extern int epoll_ctl(int epfd, int op, int fd, ref EPollEvent evt);

        [DllImport("libc.so.6")]
        private static extern int epoll_wait(int epfd, IntPtr evt, int maxevents, int timeout); 

        internal static readonly int ElemSize = Marshal.SizeOf(typeof(EPollEvent));

        [StructLayout (LayoutKind.Sequential)]
        internal struct EPollEvent
        {
            public int EventKind;
            //Really a union - 64bit (?)
            public int FD;
        }

        private int m_fd;
        private EPollBuffer m_buffer;
        private bool m_disposed = false;

        public EPoll()
        {
            m_fd = epoll_create(DefaultSize);
            UnixMarshal.ThrowExceptionForLastErrorIf(m_fd);
            m_buffer = new EPollBuffer();
        }

        ~EPoll()
        {
            Dispose(false);            
        }             

        public void Dispose()
        {
            Dispose(true);
        }

        private void Dispose(bool disposing)
        {
            if(m_disposed)
                return;

            if(disposing)
            {
                m_buffer.Dispose();
                m_buffer = null;

                GC.SuppressFinalize(this);
            }

            int r;
			do 
            {
		        r = Syscall.close(m_fd);
			} while(UnixMarshal.ShouldRetrySyscall(r));
			UnixMarshal.ThrowExceptionForLastErrorIf(r);
            
            m_fd = -1;
            m_disposed = true;
        }

        public IWatchBuffer Buffer
        {
            get
            {
                return m_buffer;
            }
        }

        internal static int MapEventsN(WatchEventKind events)
        {
            int val = 0;
            if((events & WatchEventKind.In) != 0)
                val |= EPOLLIN;
            else if((events & WatchEventKind.Out) != 0)
                val |= EPOLLOUT;
            else if((events & WatchEventKind.Err) != 0)
                val |= EPOLLERR;
            else if((events & WatchEventKind.Hup) != 0)
                val |= EPOLLHUP;
            return val;
        }

        internal static WatchEventKind MapEventsL(int events)
        {
            WatchEventKind val = WatchEventKind.None;
            if((events & EPOLLIN) != 0)
                val |= WatchEventKind.In;
            else if((events & EPOLLOUT) != 0)
                val |= WatchEventKind.Out;
            else if((events & EPOLLERR) != 0)
                val |= WatchEventKind.Err;
            else if((events & EPOLLHUP) != 0)
                val |= WatchEventKind.Hup;
            return val;
        }

        public void AddWatch(OSHandle handle, WatchEventKind events)
        {
            EPollEvent evt;
        
            if(handle.HandleKind != HandleKind.FD)
                throw new ArgumentException(String.Format("Handle of type \"{0}\" not supported by {1}", handle.HandleKind, GetType()));

            evt.FD = (int)handle.Handle;
            evt.EventKind = MapEventsN(events);

            Console.WriteLine("AddWatch({0})", evt.FD);

            int r = epoll_ctl(m_fd, EPOLL_CTL_ADD, evt.FD, ref evt);
            UnixMarshal.ThrowExceptionForLastErrorIf(r);  

            m_buffer.Count += 1;      
        }        
    
        public void UpdateEvents(OSHandle handle, WatchEventKind events)
        {
            EPollEvent evt;

            if(handle.HandleKind != HandleKind.FD)
                throw new ArgumentException(String.Format("Handle of type \"{0}\" not supported by {1}", handle.HandleKind, GetType()));

            evt.FD = (int)handle.Handle;
            evt.EventKind = MapEventsN(events);

            int r = epoll_ctl(m_fd, EPOLL_CTL_MOD, evt.FD, ref evt);
            UnixMarshal.ThrowExceptionForLastErrorIf(r);    
        }

        public void RemoveWatch(OSHandle handle)
        {
            EPollEvent evt;

            if(handle.HandleKind != HandleKind.FD)
                throw new ArgumentException(String.Format("Handle of type \"{0}\" not supported by {1}", handle.HandleKind, GetType()));

            evt.FD = (int)handle.Handle;
            evt.EventKind = 0;

            int r = epoll_ctl(m_fd, EPOLL_CTL_DEL, evt.FD, ref evt);
            UnixMarshal.ThrowExceptionForLastErrorIf(r);    
            
            m_buffer.Count -= 1;
        }

        // Wait Forever = -1, Wait Minimum Time = 0
        public int WaitAny(int timeout)
        {
            int r = epoll_wait(m_fd, m_buffer.Handle, m_buffer.Count, timeout);
            UnixMarshal.ThrowExceptionForLastErrorIf(r);    
            return r;
        } 
    }

    public class EPollBuffer: IWatchBuffer
    {
        private bool m_disposed = false;
        private int m_used = 0, m_count = EPoll.DefaultSize;

        public IntPtr Handle;

        public EPollBuffer()
        {
            Handle = UnixMarshal.AllocHeap(EPoll.DefaultSize * EPoll.ElemSize);
        }

        ~EPollBuffer()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
        }

        private void Dispose(bool disposing)
        {
            if(m_disposed)
                return;
            
            if(disposing)
            {
                GC.SuppressFinalize(this);
            }
        
            UnixMarshal.FreeHeap(Handle);
            Handle = IntPtr.Zero;

            m_disposed = true;
        }

        //FIXME: Stdlib.realloc isn't all that safe (see support/stdlib.c)
        private void Grow()
        {
            if(m_disposed)
                throw new ObjectDisposedException("EPollBuffer");

            if(m_used > m_count)
            {
                int new_count = m_count * EPoll.DefaultMultiplier;
                IntPtr tmp = Stdlib.realloc(Handle, 
                    (ulong)(new_count * EPoll.ElemSize));                
                if(tmp == IntPtr.Zero)
                    UnixMarshal.ThrowExceptionForLastError();    
                Handle = tmp;
                //Console.WriteLine("EPollBuffer.Grow(): {0} -> {1}", m_count, new_count);
                m_count = new_count;
            }    
        }

        //FIXME: This makes me shiver..
        unsafe public WatchEvent this [int index]
        {
            get
            {
                EPoll.EPollEvent evt = 
                    * (((EPoll.EPollEvent *)Handle.ToPointer()) + index);
                return new WatchEvent(new OSHandle((IntPtr)evt.FD, 
                    HandleKind.FD), EPoll.MapEventsL(evt.EventKind));
            }
        }

        public int Count
        {
            get
            {
                return m_used;
            }
            //FIXME: This is being abused for '+=' and '-=' ops, only +/-1 makes sense
            set
            {
                m_used = value;
                Grow();            
            }
        }   
    }
}

