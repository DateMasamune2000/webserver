#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "config.h"

#include "doserver.h"
#include "routes.h"

struct server
{
	int fd;
	int port;
	struct sockaddr_in address;
	int addrlen;
};

void servinit(struct server *serv);

int main(void)
{
	struct server serv;

	servinit(&serv);

	if (listen(serv.fd, 16) < 0) {
		perror("listen failure");
		exit(EXIT_FAILURE);
	}

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

		doserver(buffer, valread, new_socket);

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
