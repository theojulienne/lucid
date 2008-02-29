using System;
using System.Reflection;

using GLib;

namespace Lucid
{
    public class GLogger: ILogger
    {
        private static readonly string log_domain = Assembly.GetEntryAssembly().GetName().Name;
        private static readonly Log log = new Log();
        private static readonly ILogger Instance = new GLogger();

        public static ILogger Get()
        {
            return Instance;
        }

        public void WriteDebug(string message, object[] args)
        {
            string raw = String.Format(message, args);
            Console.ForegroundColor = ConsoleColor.Black;
            log.WriteLog(log_domain, LogLevelFlags.Debug, raw);
            Console.ResetColor();
        }

        public void WriteWarning(string message, object[] args)
        {
            string raw = String.Format(message, args);
            Console.ForegroundColor = ConsoleColor.Red;
            log.WriteLog(log_domain, LogLevelFlags.Warning, raw);
            Console.ResetColor();
        }          
    }
}

