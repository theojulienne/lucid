all:
	g++ *.cpp -c -I. `pkg-config glib-2.0 --cflags` -finline -g
	gcc *.o -o test test.c  `pkg-config glib-2.0 --cflags --libs` -I. -lstdc++ -g

clean:
	rm -f *.o
	rm -f test
	rm -f *~
