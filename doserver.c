#include "doserver.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>

void doserver(char *req, size_t lreq, int sock) {
	char *line, *lsvp;
	int n = 1;

	line = strtok(req, "\r\n");
	char method[4], resource[32], proto[16];

	sscanf(line, "%s %s %s", method, resource, proto);

	printf("Method: %s\nResource: %s\nProto: %s\n", method, resource, proto);

	while ((line = strtok(NULL, "\r\n")) != NULL) {
		char key[32], value[32];
		int i;

		sscanf(line, "%32s %32s", &key,  &value);
		
		key[strlen(key)-1] = 0;

		printf("Key: %s Value: %s\n", key, value);
	}

	char buffer[32];
	sprintf(buffer, "./%s", resource);

	char *message = "HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n";

	send(sock, message, strlen(message), 0);

	int fd = open(buffer, O_RDONLY);
	size_t m;

	while ((m = read(fd, buffer, 32)) == 32)
		send(sock, buffer, m, 0);
		
	send(sock, buffer, m, 0);

	send(sock, "\r\n\r\n", 4, 0);
}
