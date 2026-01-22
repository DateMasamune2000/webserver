#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "config.h"

#include "doserver.h"
#include "routes.h"

char config_path[4096];

struct server
{
	int fd;
	int port;
	struct sockaddr_in address;
	int addrlen;
};

int nroutes = 1;
struct route routes[MAXROUTES] = {
	(struct route) { .from = "/", .to = "main.c" }
};

void servinit(struct server *serv);

int main(int argc, char *argv[])
{
	if (argc > 2) {
		fprintf(stderr, "Incorrect number of arguments.\n");
		fprintf(stderr, "usage: %s [dir]\n");
		fprintf(stderr, "Paths in config expressed relative to dir\n");
		return 2;
	} else if (argc == 2) {
		if (chdir(argv[1]) == -1) {
			perror("could not access directory");
			return 1;
		}
	}

	struct server serv;

	servinit(&serv);

	if (listen(serv.fd, 16) < 0) {
		perror("listen failure");
		exit(EXIT_FAILURE);
	}

	/* read config file */
	sprintf(config_path, "%s/.config/dioserver/routes", getenv("HOME"));
	nroutes = loadroutes(config_path, routes);
	if (nroutes == -1)
		exit(-1);

	/* main loop */
	while (true) {
		int new_socket = accept(serv.fd, (struct sockaddr *) &serv.address,
				(socklen_t*) &serv.addrlen);
		if (new_socket < 0) {
			perror("accept failure");
			exit(EXIT_FAILURE);
		}

		char buffer[1024] = {0};
		size_t valread;
		bool rdone = false;

		do {
			valread = read(new_socket, buffer, 1023);
			buffer[valread] = 0;

			if (valread == 2) {
				rdone = true;
			} else if (valread >= 4
					&& strcmp(buffer+valread-4, "\r\n\r\n") == 0) {
				rdone = true;
			}
		} while (!rdone);

		doserver(buffer, valread, new_socket, nroutes, routes);

		close(new_socket);
	}

	close(serv.fd);

	return 0;
}

void servinit(struct server *serv)
{
	serv->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv->fd == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	const int enable = 1;
	if (setsockopt(
				serv->fd,
				SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	serv->addrlen = sizeof(serv->address);

	serv->address.sin_family = AF_INET;
	serv->address.sin_addr.s_addr = INADDR_ANY;
	serv->address.sin_port = htons(PORT);

	if (bind(serv->fd, (struct sockaddr*) &(serv->address),
				sizeof(serv->address)) < 0) {
		perror("bind failure");
		exit(EXIT_FAILURE);
	}
}
