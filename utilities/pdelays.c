// pdelays.c
#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include "pdelays.h"

#define msleep(x) usleep((x)*1000)
// -------------------------------------------------
uint32_t timems (void) {
	struct timeval tv ;
//	uint32_t t1 ;

	gettimeofday (&tv, NULL) ; // time of day in sec and us since Epoch
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
// -------------------------------------------------
void timeDlySet( TDlyData_t *t, uint32_t n) {
	if (n == 0) {
		t->dlyWasInit = 0;
		return;
	}
	t->dlyStartTime = timems();
	t->dlyDesired = n;
	t->dlyWasInit=1;
	t->dlyExpired = 0;
}
// -------------------------------------------------
void timeDlySetSinceOld( TDlyData_t *t, uint32_t n) {
	if (n == 0) {
		t->dlyWasInit = 0;
		return;
	}
	if (t->dlyWasInit) {
		t->dlyStartTime = t->dlyStartTime + t->dlyDesired; // old start & desired
	}
	else {
		t->dlyStartTime = timems();
		t->dlyWasInit=1;
	}
	t->dlyDesired = n;
}
// -------------------------------------------------
uint8_t timeDlyExpired( TDlyData_t *t ) {
	if (((timems() - t->dlyStartTime)) >= t->dlyDesired) {
		return 1;
	}
	return 0;
}
// -------------------------------------------------
void timeDlySinceOld( TDlyData_t *t, uint32_t n) {
	timeDlySetSinceOld(t, n);
	while(timeDlyExpired(t) == 0) {
		msleep(10);
	}
}
// -------------------------------------------------
void timeDly( TDlyData_t *t, uint32_t n) {
	timeDlySet(t,n);
	while(timeDlyExpired(t) == 0) {
		msleep(10);
	}
}
// -------------------------------------------------
/*void sleepms( uint32_t n ) {
	usleep(n*1000);
}
*/
// -------------------------------------------------
