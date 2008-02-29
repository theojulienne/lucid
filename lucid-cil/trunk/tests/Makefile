all: copy-dlls
	gmcs -debug Test1.cs -r:Lucid.dll -r:Mono.Posix
	gmcs -debug TestDbus.cs -r:Lucid.dll -r:NDesk.DBus
copy-dlls:
	cp ../src/Lucid/Lucid.dll .
	cp ../src/Lucid.Native/Lucid.Native.dll .
