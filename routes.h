#ifndef _ROUTES_H
#define _ROUTES_H

#include "config.h"

struct route {
	char from[32];
	char to[256];
};

extern struct route routes[];

// Returns number of routes loaded
int loadroutes(char *filename, struct route *routelist);

char *r2localpath(char *from, struct route *routes, int lroutes);

#endif
