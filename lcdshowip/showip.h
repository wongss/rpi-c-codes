// mulproj.h

#ifndef _MULPROJ_H // prevent problem if included multi times
#define _MULPROJ_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "plcd.h"
#include "pi2c.h"

// getipaddr.c
// -------------------------------------------------
// return: 0 not found, 1 found, -1 error
int getIPAddr(const char *iname, char *host);

// showip.c
void sysClose(int status); // system cleanup and quit program with status

#endif
