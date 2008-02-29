using System;
using System.Threading;
using System.Runtime.InteropServices;

using System.Collections.Generic;

using Mono.Unix;
using Mono.Unix.Native;

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
        [DllImport("__Internal")]
        private static extern IntPtr g_thread_self();   
    
        /*
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

        public static UnixPipes MakePipes()
        {
            UnixPipes pipes = UnixPipes.CreatePipes();
            
            int p_fd = pipes.Writing.Handle;
            int cur_fl = Syscall.fcntl(p_fd, FcntlCommand.F_GETFL);
            int r = Syscall.fcntl(p_fd, FcntlCommand.F_SETFL, 
                (int)OpenFlags.O_NONBLOCK | cur_fl);
            UnixMarshal.ThrowExceptionForLastErrorIf(r);   

            return pipes;         
        }

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

            UnixPipes pipes = MakePipes();

            object[] args = new object[2];
            args[0] = pipes;
            args[1] = new Queue<int>();
  
            Thread t = new Thread(
            delegate(object o)
            {
                object[] t_args = o as object[];
                int i = 0;
                Application.AddTimeout(500,
                    delegate
                    {
                        (t_args[1] as Queue<int>).Enqueue(i);                
                        ((UnixPipes)t_args[0]).Writing.WriteByte(0x69); 
                        i += 1;
                        if(i == 5)
                            Application.Quit();   
                    });
                Application.Run();
            });
            
            Application.AddWatch(new OSHandle((IntPtr)pipes.Reading.Handle),
                WatchEventKind.In, 
                delegate 
                { 
                    Console.WriteLine((args[1] as Queue<int>).Dequeue());
                    pipes.Reading.ReadByte();      
                });
            
            Application.AddWatch(new OSHandle((IntPtr)0), 
                WatchEventKind.In,
                delegate
                {
                    Application.Quit();
                });

            t.Start(args);
            Application.Run();
            t.Abort();     

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
           
