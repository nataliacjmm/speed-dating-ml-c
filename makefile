# Makefile

Programa: main
	./main

main: main.c matriz.o normalizar.o
	gcc -g main.c matriz.o normalizar.o -o main -Wall

matriz.o: matriz.c matriz.h
	gcc -c -g matriz.c -o matriz.o -Wall

normalizar.o: normalizar.c normalizar.h
	gcc -c -g normalizar.c -o normalizar.o -Wall

clean:
	rm -f matriz.o normalizar.o main
