#include "doserver.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "routes.h"

struct route routes[] = {
	(struct route) { "/", "main.c" },
	(struct route) { "/i3", "/home/bondrewd/.config/i3/config" },
	(struct route) { "/doserver", "doserver.c" },
};

const char *successHeader = "HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: ";

const char *notAvailableHeader = "HTTP/1.1 404 Not Found\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"404 Not found\r\n"
		"\r\n";

const char *errorHeader = "HTTP/1.1 500 Internal Server Error\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"500 Internal Server Error\r\n"
		"\r\n";

void doserver(char *req, size_t lreq, int sock) {
	char *line, *lsvp;
	int n = 1;

	line = strtok(req, "\r\n");
	char method[4], resource[32], proto[16];

	sscanf(line, "%s %s %s", method, resource, proto);

#ifdef _DEBUG
	printf("Method: %s\nResource: %s\nProto: %s\n", method, resource, proto);
#endif

	while ((line = strtok(NULL, "\r\n")) != NULL) {
		char key[32], value[32];
		int i;

		sscanf(line, "%32s %32s", &key,  &value);
		
		key[strlen(key)-1] = 0;

#ifdef _DEBUG
		printf("Key: %s Value: %s\n", key, value);
#endif
	}

	char buffer[32];

	char *localpath = r2localpath(resource, routes, 2);

	if (localpath != NULL)
	{
		/* Get content length */
		struct stat st;
		if (stat(localpath, &st) == -1)
			goto err;

		int temp = sprintf(buffer, "%d\r\n\r\n", st.st_size);

		/* Read file and send over socket */
		int fd = open(localpath, O_RDONLY);

		send(sock, successHeader, strlen(successHeader), 0);
		send(sock, buffer, temp, 0); /*Value of Content-Length field + 2xCRLF*/

		size_t m;
		while ((m = read(fd, buffer, 32)) == 32)
			send(sock, buffer, m, 0);

		send(sock, buffer, m, 0);

		send(sock, "\r\n\r\n", 4, 0);

		close(fd);
	}
	else
	{
		send(sock, notAvailableHeader, strlen(notAvailableHeader), 0);
	}

err:
	send(sock, errorHeader, strlen(errorHeader), 0);
}
