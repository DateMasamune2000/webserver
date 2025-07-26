#ifndef _ROUTES_H
#define _ROUTES_H

struct route {
	char from[32];
	char to[256];
};

extern struct route routes[];

char *r2localpath(char *from, struct route *routes, int lroutes);

#endif
