all:
	swig -python -c++ vtest.i
	c++ -c vtest_wrap.cxx -I /System/Library/Frameworks/Python.framework/Headers/
	c++ -dynamiclib vtest.cpp vtest_wrap.o -o _vtest.so -framework Python
