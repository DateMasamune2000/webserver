HEADERS=doserver.h config.h routes.h
OBJFILES=obj/main.o obj/doserver.o obj/routes.o
CFLAGS=-I.

all: bin/webserver test/test

bin/webserver: $(OBJFILES)
	gcc -o bin/webserver obj/main.o obj/doserver.o obj/routes.o

test/test: obj/routes.o test/test.c
	gcc $(CFLAGS) -c -o obj/test.o test/test.c
	gcc $(CFLAGS) -o test/test obj/test.o obj/routes.o 

obj/routes.o: routes.c $(HEADERS)
	gcc $(CFLAGS) -c -o obj/routes.o routes.c

obj/doserver.o: doserver.c $(HEADERS)
	gcc $(CFLAGS) -c -o obj/doserver.o doserver.c

obj/main.o: main.c $(HEADERS)
	gcc $(CFLAGS) -c -o obj/main.o main.c

clean:
	rm -f obj/*.o webserver
