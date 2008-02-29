using System;
using System.IO;

namespace Lucid
{
    public interface INativeFactory
    {
        IEventLoop GetLoop();
        ILogger GetLogger();
        Stream CreateStream(OSHandle handle, bool owns_handle);
    }
}
