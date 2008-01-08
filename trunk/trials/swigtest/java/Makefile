linux:
	swig -outdir . -o vtest_wrap.cpp -java -c++ ../vtest.i
	g++ -shared -fPIC -I.. ../vtest.cpp vtest_wrap.cpp -o libvtest.so
	javac Main.java

darwin:
	swig -outdir . -o vtest_wrap.cpp -java -c++ ../vtest.i
	g++ -dynamiclib -I.. -I/System/Library/Frameworks/JavaVM.framework/Headers/ ../vtest.cpp vtest_wrap.cpp -o libvtest.jnilib
	javac Main.java 

clean:
	rm -f *~
	rm -f vtest.java
	rm -f Foo.java
	rm -f vtestJNI.java
	rm -f vtest_wrap.cpp
	rm -f vtest_wrap.h
	rm -f *.class
	rm -f libvtest.so
	rm -f libvtest.jnilib

