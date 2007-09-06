all: clean
	g++ *.cpp -c -I. `pkg-config gmodule-2.0 glib-2.0 --cflags` -finline -g
	ar rcs liblt-base.a *.o
	gcc -o test test.c liblt-base.a  `pkg-config gmodule-2.0 glib-2.0 --cflags --libs` -I. -lstdc++ -g
	gcc -shared -fPIC -o liblt-base.so *.o  `pkg-config gmodule-2.0 glib-2.0 --cflags --libs` -lstdc++

	
clean:
	rm -f *.o
	rm -f *.a
	rm -f test
	rm -f *~
