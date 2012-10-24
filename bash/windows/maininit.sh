#!/bin/sh

MAKEFILE=`cat<<EOF
run: main
\n\t./main
\nmain: main.c
\n\tcc main.c -o main
\nclean:
\n\trm main *.o
\n
EOF`

echo -e $MAKEFILE > Makefile

MAINFILE=`cat<<EOF
#include <stdio.h>
\n\nint main(void)
\n{
\n\n}\n
EOF`

echo -e $MAINFILE >main.c

emacs main.c
