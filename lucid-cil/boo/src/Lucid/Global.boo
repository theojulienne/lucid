using System;
using System.Threading;

namespace Lucid
{
	public static class Global
	{
        private static Thread main = null;

        public static Thread MainThread
        {
            internal set
            {
                main = value;
            }
            get
            {
                return main;
            }
        }
	}
}
