all: assembler

assembler:
	gcc -g -Wall -ansi -pedantic -Iinclude/ -o bin/assembler src/*.c

check:
	gcc -g -Wall -ansi -pedantic -o bin/check check.c