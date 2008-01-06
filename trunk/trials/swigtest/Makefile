all: python

python:
	swig -python -c++ vtest.i
	c++ -c vtest_wrap.cxx -I /System/Library/Frameworks/Python.framework/Headers/
	c++ -dynamiclib vtest.cpp vtest_wrap.o -o _vtest.so -framework Python

lua:
	swig -lua -c++ vtest.i
	c++ -c vtest_wrap.cxx -I /sw/include
	c++ -bundle vtest.cpp vtest_wrap.o -o vtest.so -llua

java:
	swig -java -c++ vtest.i
	c++ -c vtest_wrap.cxx -I /System/Library/Frameworks/JavaVM.framework/Headers/
	c++ -bundle vtest.cpp vtest_wrap.o -o libvtest.jnilib
