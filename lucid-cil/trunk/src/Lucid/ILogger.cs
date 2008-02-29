using System;

namespace Lucid
{
    public interface ILogger
    {
        void WriteDebug(string message, object[] args);
        void WriteWarning(string message, object[] args);
    }
}
