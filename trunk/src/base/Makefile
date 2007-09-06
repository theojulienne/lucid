all:
	g++ *.cpp -c -I. `pkg-config glib-2.0 --cflags` -finline -g
	ar rcs liblt-base.a *.o
	gcc -o test test.c liblt-base.a  `pkg-config glib-2.0 --cflags --libs` -I. -lstdc++ -g

clean:
	rm -f *.o
	rm -f *.a
	rm -f test
	rm -f *~
