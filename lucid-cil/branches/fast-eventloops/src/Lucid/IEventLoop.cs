using System;

namespace Lucid
{
    public delegate void WatchHandler(object source, 
        OSHandle handle, WatchEventKind events);
    public delegate void TimeoutHandler(object source);

    public interface IEventLoop
    {
        object AddWatch(OSHandle handle, WatchEventKind events, WatchHandler handler);   
        void UpdateWatch(object source, WatchEventKind events);
        object AddTimeout(uint timeout, TimeoutHandler handler);
        void RemoveSource(object source);         

        void Run();
        bool RunIteration(bool block);

        void Quit();

        bool Pending
        {
            get;
        }
    }

    /*
	public static class MainLoop
	{		
		[ThreadStatic] 
		static GLib.MainLoop m_loop;		

        //FIXME: This should create a thread-local loop..but how will that play nice w/ gtk?

		private delegate int GSourceFunc(IntPtr data);
		private delegate void GDestroyNotify(IntPtr data);

		[DllImport(Global.GLibDLL)]
		private static extern IntPtr g_main_loop_new(IntPtr main_context, int is_running);

		[DllImport(Global.GLibDLL)]
		private static extern void g_main_loop_run(IntPtr loop);

		[DllImport(Global.GLibDLL)]
		private static extern void g_main_loop_quit(IntPtr loop);
		
		[DllImport(Global.GLibDLL)]
		private static extern IntPtr g_main_loop_get_context(IntPtr main_context);

		[DllImport(Global.GLibDLL)]
		private static extern IntPtr g_timeout_source_new(uint milliseconds);

		[DllImport(Global.GLibDLL)]
		private static extern IntPtr g_idle_source_new();

		[DllImport(Global.GLibDLL)]
		private static extern uint g_source_attach(IntPtr source, IntPtr main_context);

		[DllImport(Global.GLibDLL)]
		private static extern void g_source_set_callback(IntPtr source, GSourceFunc source_func, 
			IntPtr data, GDestroyNotify destroy_notify);

		private static void EnsureMainLoop()
		{
			if(m_loop == IntPtr.Zero)
			{
				m_loop = g_main_loop_new(IntPtr.Zero, 0);	
			}
		}

		public static void Run()
		{
			EnsureMainLoop();
			g_main_loop_run(m_loop);
		}

		public static void Quit()
		{	
			EnsureMainLoop();
			g_main_loop_quit(m_loop);
		}				

		internal uint AddSource(IntPtr source)
		{
			IntPtr context = g_main_loop_get_context(m_loop);
			return g_source_attach(source, context);
		}

		private static int SourceCallback(IntPtr data)
		{
			
		}

		public uint AddTimeout(uint milli_interval)
		{
			IntPtr source = g_timeout_new_source(milli_interval);
			g_source_set_callback(source, SourceCallback, 
			return AddSource(source);
		}

		public uint AddIdle()
		{
			IntPtr source = g_idle_new_source();
			
			return AddSource(source);
		}
	
        private static void EnsureLoop()
        {
            if(m_loop == null)
                m_loop = new GLib.MainLoop();
        }

        public static void Run()
        {
            EnsureLoop();
            m_loop.Run();
        }

        public static void Quit()
        {
            EnsureLoop();
            m_loop.Quit();
        }

		public static uint AddTimeout(uint interval, GLib.TimeoutHandler handler)
		{
			return GLib.Timeout.Add(interval, handler);
		}

		public static uint AddIdle(uint interval, GLib.IdleHandler handler)
		{
			return GLib.Idle.Add(handler);
		}

		public static bool RemoveSource(uint source_id)
		{
			return GLib.Source.Remove(source_id);
		}				
	}
    */
}

