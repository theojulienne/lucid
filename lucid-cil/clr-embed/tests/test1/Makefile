# Assumes recent and sane Linux distro

all:
	gcc -g test.c -o test `pkg-config --libs --cflags mono` 
	gmcs Embed.cs -t:library		

run: all
	./test Embed.dll
