using System;
using System.Threading;
using System.Runtime.InteropServices;

using System.Collections.Generic;

using Lucid;

namespace Test
{
    public class Sprocket: Lucid.Object
    {
        [Event]
        public static int FailedEvent;

        public event EventHandler Failed
        {
            add
            {
                AddHandler(FailedEvent, value);
            }
            remove
            {
                RemoveHandler(FailedEvent, value);
            }
        }

        public void Fail()
        {
            Emit(FailedEvent, this, EventArgs.Empty);
        }   
    }

    public class Widget: Lucid.Object
    {
        [Event]
        public static int FailedEvent;

        public event EventHandler Failed
        {
            add
            {
                AddHandler(FailedEvent, value);
            }
            remove
            {
                RemoveHandler(FailedEvent, value);
            }
        }

        public void Fail()
        {
            Emit(FailedEvent, this, EventArgs.Empty);
        }
    }

    public class Button: Widget
    {
        [Event]
        public static int WonEvent;
    }

    public class App
    {
        /*
        [DllImport("__Internal")]
        private static extern IntPtr g_thread_self();   
    
        public static void OnDataRecv(IOChannel channel)
        {
            channel.DataReceived -= OnDataRecv;
            MainLoop.Quit();
        }

        public static void Main()
        {
            IOChannel channel = new IOChannel(0);
            Console.WriteLine(channel.CanRead);
            channel.DataReceived += OnDataRecv;
            channel.DataReceived += OnDataRecv;
            Console.WriteLine("waiting..");
            MainLoop.Run();
        }
        */

        public static void Main()
        {
        /*
            Poll poll = new Poll();
            poll.AddFD(0, EventKind.In);
            int n = poll.Wait(-1);
            Console.WriteLine("{0}", poll.EventBuffer[0].EventKind);
            Thread.Sleep(1000);

        */

            Application.Init();
            Application.Debug = true;

            Application.WriteDebug("Pipe/Thread test");

            ThreadQueue q = new ThreadQueue();
            q.MessageReceived = delegate(object data)
            {
                int tid = Thread.CurrentThread.GetHashCode();
                Application.WriteDebug("Thread {0}: Received {1}", tid, data);
            };
            
            Thread t = new Thread(
            delegate(object o)
            {
                ThreadQueue main_q = (ThreadQueue)o;
                /*
                int i = 0;
                Application.AddTimeout(500,
                    delegate
                    {
                        main_q.PostMessage(i); 
                        i += 1;
                        if(i == 5)
                            Application.Quit();   
                    });
                Application.Run();
                */

                // "long calculation"
                while(true)
                {
                    Thread.Sleep(500);
                    main_q.PostMessage(DateTime.Now.Ticks);
                }
            });
            
            Application.AddWatch(new OSHandle((IntPtr)0), 
                WatchEventKind.In,
                delegate
                {
                    Application.Quit();
                });

            t.Start(q);
            Application.Run();
            
            t.Abort();     

            //GC.WaitForPendingFinalizers();
            
        /*
            Sprocket s = new Sprocket();
            Widget w = new Widget();             new Button();
            Console.WriteLine(Widget.FailedEvent);
            Console.WriteLine(Button.WonEvent);
            s.Failed += delegate { Console.WriteLine("i can has fail"); };
            s.Fail();
        */
        }
    }
}
           
