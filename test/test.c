#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "routes.h"
#include "config.h"

#define dbglinum fprintf(stderr, "line %d\n", __LINE__)

#define test(exp) if (!(exp)) { fprintf(stderr, \
		"[[31m-[0m] Test %s@ [31mFAILED[0m\n", #exp);\
		} else { fprintf(stderr, \
				"[[32mX[0m] Test %s [32mSUCCEEDED[0m\n" \
				, #exp); } 

#define assert(exp) if (!(exp)) { fprintf(stderr, \
		"[[31m-[0m] Assert %s[33m@%d [31mFAILED[0m\n", #exp, \
		__LINE__); exit(1); }

struct route routes[MAXROUTES];

int main(void)
{
	printf("Loading routes...\n");

	assert(loadroutes("./routes.txt", routes) == 2);

	printf("Routes loaded.\nChecking...\n");

	assert(strcmp(routes[0].from, "/") == 0);
	assert(strcmp(routes[0].to, "test.c") == 0);
	assert(strcmp(routes[1].from, "/about") == 0);
	assert(strcmp(routes[1].to, "about.txt") == 0);

	printf("Routes loaded successfully.\n");

	return 0;
}
