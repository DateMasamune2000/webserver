#include "routes.h"

#include <string.h>

char *r2localpath(char *from, struct route *r, int lroutes)
{
	for (int i = 0; i < lroutes; i++) {
		if (!strcmp(from, r[i].from))
			return r[i].to;
	}

	return NULL;
}
