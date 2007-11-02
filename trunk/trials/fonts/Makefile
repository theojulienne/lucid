all:
	gcc test.c claro-font.c claro-font-ft2.c -I ../../src `pkg-config gtk+-2.0 --cflags --libs` -o test ../../src/claro/base/libclaro-base.a ../../src/claro/graphics/libclaro-graphics.a

mac:
	gcc test.c claro-font.c claro-font-atsui.m -I ../../src -framework Cocoa -Wall `/sw/bin/glib-config --cflags` -I/usr/local/include -I/usr/local/include/cairo -lcairo -o test -L../../src/claro/base -lclaro-base -L../../src/claro/graphics -lclaro-graphics -g -I../../src/claro/base/eglib/src/ -I../../src/claro/base/ac/ -D_MAC

win:
	gcc test.c claro-font.c claro-font-win32.c -I ../../src -Wall -I/usr/local/include -I/MinGW/include/cairo -L/MinGW/lib -lcairo -o test -L../../src/claro/base -lclaro-base -L../../src/claro/graphics -lclaro-graphics -g -I../../src/claro/base/eglib/src/ -I../../src/claro/base/ac/ -DUNICODE -lgdi32


clean:
	rm *~ -rf
	rm test
