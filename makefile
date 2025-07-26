HEADERS=doserver.h config.h routes.h

webserver: main.o doserver.o routes.o
	gcc -o webserver main.o doserver.o routes.o

routes.o: routes.c $(HEADERS)
	gcc -c -o routes.o routes.c

doserver.o: doserver.c $(HEADERS)
	gcc -c -o doserver.o doserver.c

main.o: main.c $(HEADERS)
	gcc -c -o main.o main.c

clean:
	rm -f *.o webserver
