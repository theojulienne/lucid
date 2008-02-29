using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using GLib;

namespace Lucid.GLib
{
    internal static class GIOChannel
	{   
        /*
		
        [DllImport (Global.GLibDLL)]
		static extern IntPtr g_io_channel_new_file (IntPtr filename, IntPtr mode, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_error_quark ();

		[DllImport(Global.GLibDLL)]
		static extern int g_io_channel_error_from_errno (int errno);

		[DllImport (Global.GLibDLL)]
		static extern void g_io_channel_init (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_read_chars (IntPtr raw, byte[] buf, UIntPtr count, out UIntPtr bytes_read, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_read_line (IntPtr raw, out IntPtr str_return, IntPtr length, out UIntPtr terminator_pos, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_read_to_end (IntPtr raw, out IntPtr str_return, out UIntPtr length, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_read_unichar (IntPtr raw, out uint thechar, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_seek_position (IntPtr raw, long offset, int type, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_write_chars (IntPtr raw, byte[] buf, UIntPtr count, out UIntPtr bytes_written, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_write_unichar (IntPtr raw, uint thechar, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_get_buffer_condition (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern bool g_io_channel_get_buffered (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern void g_io_channel_set_buffered (IntPtr raw, bool buffered);

		[DllImport (Global.GLibDLL)]
		static extern UIntPtr g_io_channel_get_buffer_size (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern void g_io_channel_set_buffer_size (IntPtr raw, UIntPtr size);

		[DllImport (Global.GLibDLL)]
		static extern bool g_io_channel_get_close_on_unref (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern void g_io_channel_set_close_on_unref (IntPtr raw, bool do_close);

		[DllImport (Global.GLibDLL)]
		static extern IntPtr g_io_channel_get_encoding (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_set_encoding (IntPtr raw, IntPtr encoding, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_set_flags (IntPtr raw, int flags, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern IntPtr g_io_channel_get_line_term (IntPtr raw, out int length);

		[DllImport (Global.GLibDLL)]
		static extern void g_io_channel_set_line_term (IntPtr raw, byte[] term, int length);

		[DllImport (Global.GLibDLL)]
		static extern IntPtr g_io_channel_ref (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_flush (IntPtr raw, out IntPtr error);

        [DllImport (Global.GLibDLL)]
		static extern int g_io_channel_shutdown (IntPtr raw, bool flush, out IntPtr err);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_read_chars (IntPtr raw, byte[] buf, UIntPtr count, out UIntPtr bytes_read, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_write_chars (IntPtr raw, byte[] buf, UIntPtr count, out UIntPtr bytes_written, out IntPtr error);

		[DllImport (Global.GLibDLL)]
		static extern void g_io_channel_set_close_on_unref (IntPtr raw, bool do_close);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_get_flags (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern uint g_io_add_watch_full (IntPtr raw, int priority, int condition, IOFuncNative func, IntPtr user_data, GLib.DestroyNotify notify);

       */

        [DllImport (Global.GLibDLL)]
		static extern IntPtr g_io_channel_unix_new (int fd);

		[DllImport (Global.GLibDLL)]
		static extern int g_io_channel_unix_get_fd (IntPtr raw);

		[DllImport (Global.GLibDLL)]
		static extern void g_io_channel_unref (IntPtr raw);        

        [DllImport (Global.GLibDLL)]
		static extern IntPtr g_io_create_watch (IntPtr raw, int condition);

        [DllImport (Global.GLibDLL)]
		static extern void g_source_set_callback(IntPtr source, IOFuncNative func, 
            IntPtr user_data, DestroyNotify notify);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	    private delegate bool IOFuncNative(IntPtr source, int condition, IntPtr data);
        
        private static bool IOHandler(IntPtr channel, int condition, IntPtr data)
        {
            object[] args = (GCHandle.FromIntPtr(data).Target as object[]);
            WatchHandler handler = args[0] as WatchHandler;
            object source = args[1];
            try
            {
                OSHandle h = new OSHandle((IntPtr)g_io_channel_unix_get_fd(channel));
                handler(source, h, MapEventsL((IOCondition)condition));
            }
            catch(Exception e)
            {
                    ExceptionManager.RaiseUnhandledException(e, true);
                    return false;
            }
            return true;
        }

        public static IntPtr CreateWatch(int fd, WatchEventKind events, 
            WatchHandler handler, object source_id)
        {
            IntPtr channel = g_io_channel_unix_new(fd);
            IntPtr source = g_io_create_watch(channel, (int)MapEventsN(events));
            g_io_channel_unref(channel);

            object[] args = new object[2];
            args[0] = handler;
            args[1] = source_id;

            IntPtr data = (IntPtr)GCHandle.Alloc(args);
            DestroyNotify notify = DestroyHelper.NotifyHandler;
            g_source_set_callback(source, IOHandler, data, notify); 

            return source;           
        }

        private static IOCondition MapEventsN(WatchEventKind events)
        {
            IOCondition val = (IOCondition)0;
            if((events & WatchEventKind.In) != 0)
                val |= IOCondition.In;
            else if((events & WatchEventKind.Out) != 0)
                val |= IOCondition.Out;
            else if((events & WatchEventKind.Err) != 0)
                val |= IOCondition.Err;
            else if((events & WatchEventKind.Hup) != 0)
                val |= IOCondition.Hup;
            return val;
        }

        private static WatchEventKind MapEventsL(IOCondition events)
        {
            WatchEventKind val = WatchEventKind.None;
            if((events & IOCondition.In) != 0)
                val |= WatchEventKind.In;
            else if((events & IOCondition.Out) != 0)
                val |= WatchEventKind.Out;
            else if((events & IOCondition.Err) != 0)
                val |= WatchEventKind.Err;
            else if((events & IOCondition.Hup) != 0)
                val |= WatchEventKind.Hup;
            return val;
        }

        [Flags]
	    private enum IOCondition 
        {
		    In = 1 << 0,
		    Out = 1 << 2,
		    Pri = 1 << 1,
		    Err = 1 << 3,
		    Hup = 1 << 4,
		    Nval = 1 << 5,
	    }
    }
}

