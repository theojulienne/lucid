using System;
using System.Threading;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;

using NDesk.DBus;
using org.freedesktop.DBus;

using Lucid;

public class TestDBus
{
    private static int GetConnFD(Connection c)
    {
        PropertyInfo transport_prop = typeof(Connection).GetProperty("Transport", BindingFlags.NonPublic|BindingFlags.Instance);    
        FieldInfo socket_field = transport_prop.PropertyType.GetField("SocketHandle");
        return (int)((long)socket_field.GetValue(transport_prop.GetValue(c, null)));
    }

    public static void Main()
    {
        IPoll poll = new EPoll();
        poll.AddWatch(new OSHandle((IntPtr)0), WatchEventKind.In);

        Connection c = Bus.Session;
        IBus dbus = c.GetObject<IBus>("org.freedesktop.DBus", new ObjectPath("/org/freedesktop/DBus"));
        int fd = GetConnFD(c);

        Console.WriteLine("DBus FD = {0}", fd);

        dbus.NameAcquired += delegate(string name) { Console.WriteLine("NameAcquired() = {0}", name); };        
        dbus.NameLost += delegate(string name) { Console.WriteLine("NameLost() = {0}", name); };        

        poll.AddWatch(new OSHandle((IntPtr)fd), WatchEventKind.In | WatchEventKind.Hup);

        int n = 1; 
        
        Console.WriteLine("Running on {0}", Thread.CurrentThread.ManagedThreadId);
    
        while(n > 0)
        {
            n = poll.WaitAny(-1);
            Console.WriteLine("WaitAny(-1) = {0}", n);
            for(int i = 0; i < n; i++)
            {
                WatchEvent evt = poll.Buffer[i];
                Console.WriteLine("\t{0} {1}", evt.Handle, evt.Events);                
                if(evt.Handle == (IntPtr)0)
                    goto done;
                else if(evt.Handle == (IntPtr)fd)
                {
                    if((evt.Events & WatchEventKind.In) != 0)
                        c.Iterate();
                    else
                        goto done;
                }
            }    
        }

done:
        ;
    }
}

