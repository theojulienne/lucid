using System;
using System.Collections;
using System.Threading;

namespace Lucid
{
    /*
        FIXME: This kind of functionality needs to be tied into BaseObject
               Look at the Dispatcher crap in WPF for ideas...
    */
    public sealed class ThreadQueue: IDisposable
    {
        private object olock = new object();
        private Queue msgs = new Queue();
        private bool m_disposed = false;
        
        private IWaitEvent evt;
        private IEventLoop loop;        
        private object msg_src;
       
        public Action<object> MessageReceived;
        
        public ThreadQueue(): this(Native.Factory.GetLoop())
        {
        }
                
        public ThreadQueue(IEventLoop the_loop)
        {
            loop = the_loop;
            evt = Native.Factory.CreateWaitEvent();
            msg_src = loop.AddWatch(evt.Handle, WatchEventKind.In, OnPingReceived);
        }
        
        ~ThreadQueue()
        {
            Dispose(false);
        }
        
        public void Dispose()
        {
            Dispose(true);
        }
        
        private void Dispose(bool disposing)
        {
            //if(! disposing)
            //    Monitor.Enter(olock);
            
            try
            {
                if(m_disposed)
                    return;
                
                if(disposing)
                    GC.SuppressFinalize(this);

                loop.RemoveSource(msg_src);                

                while(msgs.Count > 0)
                    Dispatch();

                evt.Dispose();
                evt = null;
                loop = null;
                
                m_disposed = true;
            }
            catch(Exception ex)
            {
                throw ex;
            }
            finally
            {
                Application.WriteDebug("ThreadQueue.Dispose({0}) [Thread {1}]",
                                       disposing, Thread.CurrentThread.GetHashCode());
                //if(! disposing)
                //    Monitor.Exit(olock);
            }
        }
        
        private void OnPingReceived(object source, OSHandle handle, 
                                    WatchEventKind events)  
        {
            lock(olock)
            {
                if(m_disposed)
                    throw new ObjectDisposedException("this");
                
                Dispatch();
            }
        }
        
        private void Dispatch()
        {
            if(MessageReceived != null)
                MessageReceived(msgs.Dequeue());    
            
            evt.Reset();            
        }
        
        public void PostMessage(object data)
        {
            lock(olock)
            {
                if(m_disposed)
                    throw new ObjectDisposedException("this");
                
                msgs.Enqueue(data);
                evt.Set();
            }
        }
    }
}   
