// threadblink.c

#include "mulproj.h"

// -------------------------------------------------
PI_THREAD (thread_blink) {
	while(1) {
		digitalWrite(LEDPIN, 0);
		sleep(1);
		digitalWrite(LEDPIN, 1);
		sleep(1);
	}
	return (void *)0;
}
