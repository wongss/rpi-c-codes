// threadacc.c

#include "mulproj.h"
#include "pi2c.h"
#include "adxl345.h"

#define msleep(x) usleep((x)*1000)

// ADXL345 connections:
// CS = 1
// ALT_ADDRESS = 1 for i2c address 0x1d. Otherwise 0 for 0x53

#define ADXL345_ADDR 0x53
#define FULL_RES	 1	// 0 or 1; if 1, 0.004g per bit, else depends on range
#define RANGE		 16	// 2, 4, 8, 16
// -------------------------------------------------
PI_THREAD (thread_accelerometer) {
	// accInit();
	accel_result_t r;
	int devID;
	printf("\nThread accel: Set adxl345 addr=0x%02X fullres=%d range=+-%dg",ADXL345_ADDR,FULL_RES,RANGE);
 	if ((devID = adxl345Init(ADXL345_ADDR, FULL_RES, RANGE)) <0) {
		printf("\nThread accel: init adxl345 failed, sleep forever\n");
		while(1) sleep(100);
	}
	printf("\nAccel dev id=%02X\n",devID);
	while(1) {
		if (adxl345Read(&r) >= 0) {
			printf("\rX=%+1.2fg Y=%+1.2fg Z=%+1.2fg ",r.x,r.y,r.z);
			fflush(stdout);
			msleep(500);
		}
		else {
			printf("\nErr reading accel, wait 10s");
			sleep(10);
		}
	}
	return (void *)0;
}
