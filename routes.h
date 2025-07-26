#ifndef _ROUTES_H
#define _ROUTES_H

#include "webtypes.h"

extern struct route routes[];

char *r2localpath(char *from, struct route *routes, int lroutes);
#endif
