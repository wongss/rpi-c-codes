#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "pdelays.h"
#include "pi2c.h"

#define msleep(x) usleep((x)*1000)

#define ioexpAddr 0x3b

// copy 4 switches value (lower 4 bits) to 4 leds (upper 4 bits)

PI_THREAD (thread_ioexp) {
	uint8_t buf[1];
	while(1) {
		if (pi2cRecData(ioexpAddr,1,buf) != 1) {
			printf("Failed to read i2c ioexp\n");
			goto i2cErr;
		}
		buf[0] = (buf[0] << 4) | 0x0f;
		if (pi2cSendData(ioexpAddr,1,buf) != 1) {
			printf("Failed to write i2c ioexp\n");
			goto i2cErr;
		}
i2cErr:
		msleep(200);	// do once every 0.2s
	}
	return (void *)0;
}
