linux:
	swig -outdir . -o vtest_wrap.cpp -csharp -c++ ../vtest.i
	g++ -shared -fPIC -I.. ../vtest.cpp vtest_wrap.cpp -o libvtest.so
	gmcs -out:runme.exe Foo.cs vtest.cs runme.cs vtestPINVOKE.cs Foot.cs

darwin:
	swig -outdir . -o vtest_wrap.cpp -csharp -c++ ../vtest.i
	g++ -dynamiclib -I.. ../vtest.cpp vtest_wrap.cpp -o libvtest.dylib
	gmcs -out:runme.exe Foo.cs vtest.cs runme.cs vtestPINVOKE.cs Foot.cs

clean:
	rm -f *~
	rm -f vtest.cs
	rm -f vtestPINVOKE.cs
	rm -f Foo.cs
	rm -f vtest_wrap.cpp
	rm -f vtest_wrap.h
	rm -f runme.exe
	rm -f libvtest.so
	rm -f libvtest.dylib

