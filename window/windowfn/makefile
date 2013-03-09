run: main.exe
	./main.exe

main.exe: main.c
	gcc main.c -o main -lgdi32

main2.exe: main2.c
	gcc main2.c -o main2.exe -lgdi32

run3: main3
	./main3

main3: DisplayWithScrollBar.c
	gcc DisplayWithScrollBar.c -o main3 -lgdi32