#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "doserver.h"

#define PORT 3000

int main(void)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	const int enable = 1;
	if (setsockopt(
				server_fd,
				SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
		perror("bind failure");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 16) < 0) {
		perror("listen failure");
		exit(EXIT_FAILURE);
	}

	while (true) {
		int new_socket = accept(server_fd, (struct sockaddr *) &address,
				(socklen_t*) &addrlen);
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
			} else if (valread >= 4 && strcmp(buffer+valread-4, "\r\n\r\n") == 0) {
				rdone = true;
			}
		} while (!rdone);

		doserver(buffer, valread, new_socket);

		close(new_socket);
	}

	close(server_fd);

	return 0;
}
