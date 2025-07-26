webserver: main.o doserver.o
	gcc -o webserver main.o doserver.o

doserver.o: doserver.c doserver.h config.h
	gcc -c -o doserver.o doserver.c

main.o: main.c doserver.h config.h
	gcc -c -o main.o main.c
