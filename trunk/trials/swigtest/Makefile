all: python

python:
	cd python
	swig -python -c++ ../vtest.i
	c++ -c vtest_wrap.cxx -I /System/Library/Frameworks/Python.framework/Headers/
	c++ -dynamiclib vtest.cpp vtest_wrap.o -o _vtest.so -framework Python
	
lua:
	cd lua
	swig -lua -c++ vtest.i
	c++ -c vtest_wrap.cxx -I /sw/include
	c++ -bundle vtest.cpp vtest_wrap.o -o vtest.so -llua

java:
	swig -java -c++ vtest.i
	c++ -c vtest_wrap.cxx -I /System/Library/Frameworks/JavaVM.framework/Headers/
	c++ -bundle vtest.cpp vtest_wrap.o -o libvtest.jnilib

mono:
	swig -csharp -c++ vtest.i
	c++ -c vtest_wrap.cxx -I /System/Library/Frameworks/JavaVM.framework/Headers/
	c++ -dynamiclib vtest.cpp vtest_wrap.o -o libvtest.so
	gmcs -out:runme.exe Foo.cs vtest.cs runme.cs vtestPINVOKE.cs Foot.cs
