all: assembler

clean:
	rm -rf ./bin/*

assembler:
	gcc -Wall -ansi -pedantic -Iinclude/ -o bin/assembler src/*.c src/assembler/*.c
