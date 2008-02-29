using System;
using System.IO;

namespace Lucid
{
    public abstract class HandleStream: Stream
    {
        public abstract OSHandle GetHandle();
    }
}
