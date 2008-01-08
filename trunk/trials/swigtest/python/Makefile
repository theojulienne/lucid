linux:
	swig -outdir . -o vtest_wrap.cpp -python -c++ ../vtest.i
	g++ -shared -fPIC `python-config --cflags` -I.. ../vtest.cpp vtest_wrap.cpp -o _vtest.so
	
darwin:
	swig -outdir . -o vtest_wrap.cpp -python -c++ ../vtest.i
	g++ -dynamiclib -I /System/Library/Frameworks/Python.framework/Headers/ -I.. ../vtest.cpp vtest_wrap.cpp -o _vtest.dylib

clean:
	rm -f *~
	rm -f vtest.py
	rm -f vtest.pyc
	rm -f vtest_wrap.cpp
	rm -f vtest_wrap.h
	rm -f _vtest.so
	rm -f _vtest.dylib

