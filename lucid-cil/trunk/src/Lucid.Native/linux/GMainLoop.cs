using System;
using System.Threading;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Runtime.ConstrainedExecution;

using GLib;

namespace Lucid.GLib
{
    internal class SourceID
    {
        private uint m_id;
        
        public SourceID(uint id)
        {
            m_id = id;
        }
        
        public virtual uint ID
        {
            get
            {
                return m_id;
            }
        }
    }
	
    //FIXME: This is pretty ugly..actual error handling would be nice too.
    internal class GMainLoop: CriticalFinalizerObject, IEventLoop, IDisposable
    {
        [DllImport (Global.GLibDLL)]
		static extern uint g_source_attach(IntPtr source, IntPtr context);

        [DllImport (Global.GLibDLL)]
		static extern void g_source_unref(IntPtr source);

	[DllImport (Global.GLibDLL)]
        static extern void g_source_destroy(IntPtr source);

        [DllImport (Global.GLibDLL)]
		static extern IntPtr g_main_context_find_source_by_id(IntPtr context, uint id);

        [DllImport (Global.GLibDLL)]
		static extern IntPtr g_timeout_source_new(uint interval);

        [DllImport (Global.GLibDLL)]
		static extern void g_source_set_callback(IntPtr source, TimeoutFuncNative func, 
            IntPtr user_data, DestroyNotify notify);

        [DllImport(Global.GLibDLL)]
		private static extern IntPtr g_main_loop_new(IntPtr main_context, int is_running);

		[DllImport(Global.GLibDLL)]
		private static extern void g_main_loop_run(IntPtr loop);

		[DllImport(Global.GLibDLL)]
		private static extern void g_main_loop_quit(IntPtr loop);
		
		[DllImport(Global.GLibDLL)]
		private static extern IntPtr g_main_loop_get_context(IntPtr loop);

        [DllImport(Global.GLibDLL)]
		private static extern int g_main_context_pending(IntPtr ctx);

        [DllImport(Global.GLibDLL)]
		private static extern int g_main_context_iteration(IntPtr ctx, int block);

        [DllImport(Global.GLibDLL)]
		private static extern IntPtr g_main_context_new();

        [DllImport(Global.GLibDLL)]
		private static extern void g_main_loop_unref(IntPtr loop);

        private static readonly LocalDataStoreSlot LoopTLS = System.Threading.Thread.AllocateDataSlot();

        // ** The real magic is this function. **
        public static IEventLoop Get()
        {
            object loop = System.Threading.Thread.GetData(LoopTLS);    
            if(loop == null)
            {
                loop = new GMainLoop();
                System.Threading.Thread.SetData(LoopTLS, loop);
            }
            return (IEventLoop)loop;
        }

        private bool m_disposed = false;

        public IntPtr Handle;
        public IntPtr Context;

        public GMainLoop()
        {
            //FIXME: HACK TO GET THE 'DEFAULT' CONTEXT
            if(Lucid.Global.MainThread == System.Threading.Thread.CurrentThread)
                Handle = g_main_loop_new(IntPtr.Zero, 0);
            else
                Handle = g_main_loop_new(g_main_context_new(), 0);    
            Context = g_main_loop_get_context(Handle);

            //Console.WriteLine("GMainLoop({0} {1})", Handle, Lucid.Global.MainThread == Thread.CurrentThread ? "main" : "");
        }

        ~GMainLoop()
        {
            Dispose(false);
        }

        private void Dispose(bool disposing)
        {
            if(m_disposed)
                return;
            
            if(disposing)
            {
                System.Threading.Thread.SetData(LoopTLS, null);
                GC.SuppressFinalize(this);
            }

            g_main_loop_unref(Handle);
            Application.WriteDebug("GMainLoop.Dispose({0}) [Thread {1}]",
	 	disposing, System.Threading.Thread.CurrentThread.GetHashCode());

            Handle = Context = IntPtr.Zero;
        }

        public void Dispose()
        {
            Dispose(true);
        }
        
        public virtual object AddWatch(OSHandle handle, WatchEventKind events, WatchHandler handler)
        {
            if(handle.HandleKind != HandleKind.FD)
                throw new ArgumentException(String.Format("Handle of type \"{0}\" not supported by {1}", handle.HandleKind, GetType()));
    
            uint[] id_object = new uint[1];
            IntPtr source = GIOChannel.CreateWatch((int)((IntPtr)handle), 
                events, handler, id_object);

            uint id = g_source_attach(source, Context);
            g_source_unref(source);
            id_object[0] = id;

            return (object)id_object;
        }

        //TODO
        public virtual void UpdateWatch(object source, WatchEventKind events)
        {            
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	    private delegate bool TimeoutFuncNative(IntPtr data);
        
        private static bool TimeoutHandler(IntPtr data)
        {
            object[] args = (GCHandle.FromIntPtr(data).Target as object[]);
            TimeoutHandler handler = args[0] as TimeoutHandler;
            object source = args[1];
            try
            {
                handler(source);
            }
            catch(Exception e)
            {
                    ExceptionManager.RaiseUnhandledException(e, true);
                    return false;
            }
            return true;
        }

        public object AddTimeout(uint timeout, TimeoutHandler handler)
        {
            IntPtr source = g_timeout_source_new(timeout);
            
            if(source == IntPtr.Zero)
                throw new ArgumentException("timeout"); 

            uint[] id_object = new uint[1];
            object[] args = new object[2];
            args[0] = handler;
            args[1] = id_object; 

            IntPtr data = (IntPtr)GCHandle.Alloc(args);
            DestroyNotify notify = DestroyHelper.NotifyHandler;
            g_source_set_callback(source, TimeoutHandler, data, notify); 

            uint id = g_source_attach(source, Context);
            g_source_unref(source);
            id_object[0] = id;

            return (object)id_object;
        }

        public virtual void RemoveSource(object source)
        {
            if(! (source is uint[]))
                throw new ArgumentException("source");
            
            uint id = (source as uint[])[0];
            IntPtr real_source = g_main_context_find_source_by_id(Context, id);

            if(real_source == IntPtr.Zero)
                throw new ArgumentException("source");
    
            g_source_destroy(real_source);   
        }

        public void Run()
        {
            g_main_loop_run(Handle);    
        }

        public bool RunIteration(bool block)
        {
            return g_main_context_iteration(Context, block ? 1 : 0) == 0 ? false : true;
        }

        public void Quit()
        {   
            g_main_loop_quit(Handle);
        }

        public bool Pending
        {
            get 
            {
                return g_main_context_pending(Context) == 0 ? false : true;
            }
        }
    }
}

