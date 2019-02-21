all: assembler

assembler:
	gcc -g -Wall -ansi -pedantic -Iinclude/ -o bin/assembler src/*.c

checkstuff:
	gcc -g -Wall -ansi -pedantic -Iinclude/ -o bin/check check.c