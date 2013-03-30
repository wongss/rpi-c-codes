// threadblink.c

#include "mulproj.h"

#define msleep(x) usleep((x)*1000)

// This thread blinks the LED as long as the PB not pressed
// on 200ms, off 1800ms

static TDlyData_t t;
static TDlyData_t *tp = &t;

static uint8_t ledVal;
// -------------------------------------------------
static uint8_t ledIsOn( void ) {
	return !ledVal;
}
// -------------------------------------------------
static void doledOn( void ) {
	digitalWrite(LEDPIN, 0);
	ledVal = 0;
	timeDlySet(tp,200);
}
// -------------------------------------------------
static void doledOff( void ) {
	digitalWrite(LEDPIN, 1);
	ledVal = 1;
	timeDlySet(tp,1800);
}
// -------------------------------------------------
static void doledToggle( void ) {
	if (ledIsOn()) {
		doledOff();
	}
	else {
		doledOn();
	}
}
void doblink( void ) {
	while(1) {
		digitalWrite(LEDPIN, 0);
		sleep(1);
		digitalWrite(LEDPIN, 1);
		sleep(1);
	}
}
// -------------------------------------------------
PI_THREAD (thread_blink) {
	doledOn();
	while(1) {
		// If PB pressed, off LED and wait till released
		if (swIn() == 0) {
			doledOff();
			while(swIn() == 0) {	// still pressed, wait
				msleep(100);
			}
			doledOn();	// released, on immediately
		}
		// PB not pressed
		if (timeDlyExpired(tp) == 1) {
			doledToggle();
		}
		msleep(100);
	}
	return (void *)0;
}
