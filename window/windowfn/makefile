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

run4: main4
	./main4

main4: DisplayWithScrollBarV2.c
	gcc DisplaywithScrollBarV2.c -o main4 -lgdi32


run5: main5
	./main5

main5: createDC.c
	gcc createDc.c -o main5 -lgdi32