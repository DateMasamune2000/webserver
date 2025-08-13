#include <stdio.h>
#include <stdlib.h>

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
	assert(1 == 2);
	return 0;
}
