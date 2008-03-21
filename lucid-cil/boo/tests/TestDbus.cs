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
        Application.Init();
        Application.Debug = true;
        
        Connection c = Bus.Session;
        IBus dbus = c.GetObject<IBus>("org.freedesktop.DBus", new ObjectPath("/org/freedesktop/DBus"));
        int fd = GetConnFD(c);

        Console.WriteLine("DBus FD = {0}", fd);

        dbus.NameAcquired += delegate(string name) 
        { 
            Console.WriteLine("NameAcquired() = {0}", name); 
        };        
        dbus.NameLost += delegate(string name) 
        { 
            Console.WriteLine("NameLost() = {0}", name); 
        };        
        dbus.NameOwnerChanged += delegate(string name, string old, string nnew)
        {
            Console.WriteLine("NameOwnerChanged() = {0}, {1} -> {2}", name, 
                              old == "" ? "nil" : old, 
                              nnew == "" ? "nil" : nnew); 
        };
            
        Application.AddWatch(new OSHandle((IntPtr)fd), 
            WatchEventKind.In | WatchEventKind.Hup, delegate { c.Iterate(); });

        Application.AddWatch(new OSHandle((IntPtr)0), WatchEventKind.In,
                             delegate { Application.Quit(); });
        
        Application.Run();
    }        
}

