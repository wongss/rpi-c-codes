// pdelays.h

#ifndef __PDELAYS_H
#define __PDELAYS_H
#include <stdint.h>

typedef struct { 
	uint8_t dlyWasInit;
	uint8_t dlyExpired;
	uint32_t dlyStartTime;
	uint32_t dlyDesired;
} TDlyData_t;

#ifdef __cplusplus
extern "C" {
#endif

// Time in ms since Epoch - midnight 1/1/1970
uint32_t timems (void);
// set a delay of n ms
void timeDlySet( TDlyData_t *t, uint32_t n);
// set a dealy of n ms since last delay expired (remove cummulative err)
void timeDlySetSinceOld( TDlyData_t *t, uint32_t n);
// has delay expired?
uint8_t timeDlyExpired( TDlyData_t *t );
// delay ms before return
void timeDly( TDlyData_t *t, uint32_t n);
// delay ms since last delay before return (remove cummlative error)
void timeDlySinceOld( TDlyData_t *t, uint32_t n);
// give up CPU for n ms
// void sleepms( uint32_t n );

#ifdef __cplusplus
}
#endif

// Usage Example
/*
TDlyData_t t;
timeDlySet(&t, 100);
....
if (timeDlyExpired(&t)) {
	// 100ms delay has passed
	timeDlySet(&t,100);	// Set a new delay of 100ms
	// or 
	timeDlySetSinceOld(&t,100);	// Set a new delay of 100ms	since last delay was expired
}
*/
#endif
