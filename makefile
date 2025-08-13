HEADERS=doserver.h config.h routes.h
OBJFILES=obj/main.o obj/doserver.o obj/routes.o

all: bin/webserver bin/test

bin/webserver: $(OBJFILES)
	gcc -o bin/webserver obj/main.o obj/doserver.o obj/routes.o

bin/test: obj/routes.o test.c
	gcc -c -o obj/test.o test.c
	gcc -o bin/test obj/test.o obj/routes.o 

obj/routes.o: routes.c $(HEADERS)
	gcc -c -o obj/routes.o routes.c

obj/doserver.o: doserver.c $(HEADERS)
	gcc -c -o obj/doserver.o doserver.c

obj/main.o: main.c $(HEADERS)
	gcc -c -o obj/main.o main.c

clean:
	rm -f obj/*.o webserver
