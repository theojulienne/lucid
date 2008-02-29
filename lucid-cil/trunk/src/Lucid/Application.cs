using System;
using System.Reflection;
using System.Threading;

namespace Lucid
{
    public static class Application
    {
        public static void Init(string[] args)
        {
            Global.MainThread = Thread.CurrentThread;
            Native.Load();
        }
        
        public static void Init()
        {
            Init(null);
        }

        public static bool Debug = false;

        public static void WriteDebug(string message, params object[] args)
        {
            if(! Debug)
                return;

            Native.Factory.GetLogger().WriteDebug(message, args);
        }

        public static void WriteWarning(string message, params object[] args)
        {
            if(! Debug)
                return;
            
            Native.Factory.GetLogger().WriteWarning(message, args);
        }

        public static void Run()
        {
            Native.Factory.GetLoop().Run();
        }

        public static void Quit(bool done)
        {
            IEventLoop loop = Native.Factory.GetLoop();
            loop.Quit();

            if(done)
                (loop as IDisposable).Dispose();
        }

        public static void Quit()
        {
            Quit(true);
        }

        public static object AddWatch(OSHandle handle, WatchEventKind events, 
            WatchHandler handler)
        {
            return Native.Factory.GetLoop().AddWatch(handle, events, handler);
        }

        public static object AddTimeout(uint timeout, TimeoutHandler handler)
        {
            return Native.Factory.GetLoop().AddTimeout(timeout, handler);          
        }   
    }
}
