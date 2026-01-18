#include "routes.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 256

int loadroutes(char *filename, struct route *routelist)
{
	int fd = open(filename, O_RDONLY);
	char c;
	char buffer[BUFSIZE];
	/* tracks whether filename, escape character or route name is being read */
	enum { RNAME, ESCAPE, FNAME } mode = RNAME, pmode;

	if (fd == -1) {
		perror("could not open route file");
		return -1;
	}

	int n = 0, k, i = 0;
	while (read(fd, &c, 1) && n < MAXROUTES)
	{
		switch (mode)
		{
			case RNAME:
				switch (c)
				{
					case '\\':
						pmode = RNAME;
						mode = ESCAPE;
						break;
					case ' ':
						strncpy(routes[n].from, buffer, i);
						mode = FNAME;
						i = 0;
						break;
					case '\n':
						fprintf(stderr, "Error while parsing configuration"
								" format: routes must have a destination\n");
						exit(1);
						break;
					case EOF:
						close(fd);
						return 0;
					default:
						buffer[i++] = c;
				}
				break;

			case ESCAPE:
				if (c == EOF)
				{
					close(fd);
					return 0;
				}
				buffer[i++] = c;
				mode = pmode;
				break;

			case FNAME:
				switch(c)
				{
					case '\\':
						pmode = FNAME;
						mode = ESCAPE;
						break;
					case ' ':
						fprintf(stderr, "Error while parsing configuration:"
								"each line can only have two rows.\n");
						exit(1);
						break;
					case '\n':
						strncpy(routes[n++].to, buffer, i);
						mode = RNAME;
						i = 0;
						break;
					case EOF:
						close(fd);
						return n;
					default:
						buffer[i++] = c;
				}

		}
	}

	close(fd);

	return n;
}

char *r2localpath(char *from, struct route *r, int lroutes)
{
	for (int i = 0; i < lroutes; i++) {
		if (!strcmp(from, r[i].from))
			return r[i].to;
	}

	return NULL;
}
