CC=gcc

run: main
	./main

main: main.cpp
	$(CC) $< -o $@ -L./ -lexample_dll

dll: example_dll.dll

example_dll.dll: example_dll.o
	#$(CC) -shared -o $@ $< -s -Wl,--dll
	ld --dll -o $@ $< -luser32 -lgdi32 -lcomdlg32 -lkernel32 -ladvapi32 -lmingw32 -lmoldname -lcrtdll 

example_dll.o: example_dll.cpp
	$(CC) -DDLL -c -o $@ $< 

clean:
	rm *.dll *.o *~ *.a *.exe -f