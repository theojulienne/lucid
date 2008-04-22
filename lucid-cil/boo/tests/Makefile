all: copy-dlls
	booc -debug EvtLoopTest.boo -r:Lucid.dll 
	booc -debug DebugTest.boo -r:Lucid.dll
	gmcs -debug TestDbus.cs -r:Lucid.dll -r:NDesk.DBus
copy-dlls:
	cp -u ../src/Lucid/Lucid.dll Lucid.dll
	cp -u ../src/Lucid.Native/Lucid.Native.dll Lucid.Native.dll
