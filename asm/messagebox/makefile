exec: main.exe
	./main.exe
main.exe: main.obj
	masm-link -subsystem:windows main.obj
main.obj: main.asm
	ml -c -coff main.asm

clean:
	rm *.obj *~ *.exe