using System;

using Lucid.Base;

public class Test
{
	public static void Main(string[] args)
	{
		//LObject obj = new LObject();
		LModule a = LModule.Load("/usr/lib/libmagic.so");
		Console.WriteLine("0x{0:x2} [Handle: 0x{1:x2}] {1}", a.GetHashCode(), a.Handle, a.GetSymbol("magic_open"));
		LModule b = LModule.Load("/usr/lib/libmagic.so");
		Console.WriteLine("0x{0:x2} [Handle: 0x{1:x2}] {1}", a.GetHashCode(), a.Handle, a.GetSymbol("magic_open"));
	}
}
