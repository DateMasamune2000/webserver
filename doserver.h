#ifndef _DOSERVER_H
#define _DOSERVER_H

#include "routes.h"
#include <stddef.h>

void doserver(char *req, size_t lreq, int sock, int nroutes, struct route *routes);

#endif
