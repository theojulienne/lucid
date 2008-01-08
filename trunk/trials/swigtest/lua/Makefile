linux:
	swig -outdir . -o vtest_wrap.cpp -lua -c++ ../vtest.i
	g++ -shared -fPIC -I/usr/include/lua5.1 -I.. ../vtest.cpp vtest_wrap.cpp -o vtest.so
	
darwin:
	swig -outdir . -o vtest_wrap.cpp -lua -c++ ../vtest.i
	g++ -dynamiclib -I/usr/local/include/lua5.1 -I.. ../vtest.cpp vtest_wrap.cpp -o vtest.dylib

clean:
	rm -f *~
	rm -f vtest_wrap.cpp
	rm -f vtest.so
	rm -f vtest.dylib

