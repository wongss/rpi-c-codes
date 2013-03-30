// mulfile.c

#include "mulproj.h"
#include <stdio.h>

static void localfunc ( void ) {
	printf("Inside localfunc() within mulfile.c\n");
}

void exFunc( void ) {
	printf("Inside exFunc().\nCalling a local function in the file localfunc().\n");
	localfunc();
}


