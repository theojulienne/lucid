all: copy-dlls
	gmcs -debug Test1.cs -r:Lucid.dll -r:Mono.Posix
	gmcs -debug TestDbus.cs -r:Lucid.dll -r:NDesk.DBus
copy-dlls:
	cp -u ../src/Lucid/Lucid.dll Lucid.dll
	cp -u ../src/Lucid.Native/Lucid.Native.dll Lucid.Native.dll
