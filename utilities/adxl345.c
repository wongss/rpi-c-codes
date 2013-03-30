// adcxl345.c

#include "pi2c.h"
#include "adxl345.h"

#define ADXL345_DEBUG 0
// ADXL345 connections:
// CS = 1 for i2c
// ALT_ADDRESS = 1 for i2c address 0x1d. Otherwise 0 for 0x53

static uint8_t adxl345_addr; // 0 means not initialized
static float scale;

// if success returns device ID, else -1
int adxl345Init(uint8_t addr, uint8_t res, uint8_t range) {
	const uint8_t rangeV[4] = {2,4,8,16}; // g range, index is fmt
	uint8_t data_fmt;
	uint8_t buf[2];
	int i;
	int devID;
	for(i=0;i<4;i++) {
		if (rangeV[i] == range) {
			data_fmt = i;	//, 0, 1, 2 or 3
			break;
		}
	}
	if (i>3) {
		return -1;
	}
	if (res) {
		data_fmt |= 0x08;
	}
	if (!(addr == 0x1d || addr == 0x53)) {
		return -1; // invalid address
	}
	// try now ...
//	piLock(I2C_LOCK);

	buf[0] = 0;
	if (pi2cSendData(addr,1,buf) != 1) {
#if (ADXL345_DEBUG)
		printf("Failed to write to ADXL345_ADDR\n");
#endif
		goto err;
	}
	if (pi2cRecData(addr,1,buf) != 1) {
#if (ADXL345_DEBUG)
		printf("Failed to read from ADXL345_ADDR\n");
#endif
		goto err;
	}
	devID = (int)buf[0] & 0x0ff;

#if (ADXL345_DEBUG)
	printf("Accelerometer ID=%02X\n",buf[0]); // expects 0xE5
	if (res) {
		printf("Full resolution of 0.004g/lsb & range +/-%dg\n",range);
	}
	else {
		printf("10-bit resolution & range +/-%dg\n",range);
	}
#endif	
	buf[0] = 0x31;	// data format
	buf[1] = data_fmt;
	
	if (pi2cSendData(addr,2,buf) != 2) {
		goto err;
	}
	buf[0] = 0x2D;	// power control reg
	buf[1] = 0x08;	// measure mode
	if (pi2cSendData(addr,2,buf) != 2) {
		goto err;
	}
	if (res) {
		scale = 0.004;
	}
	else {
		scale = ((float)range) /(1024/2);
	}
	adxl345_addr = addr;
#if (ADXL345_DEBUG)
	printf("adxl345Init() - addr=%02X full_res=%d range=+-%dg scale=%f\n",adxl345_addr,res,range,scale);
#endif
	return devID;
err:
	return -1;
}

int adxl345Read( accel_result_t *r ) {
	uint8_t buf[6];
	if (adxl345_addr == 0) return -1;
	buf[0] = 0x32;	// set address to read
	if (pi2cSendData(adxl345_addr,1,buf) != 1) {
#if (ADXL345_DEBUG)
		printf("Fail to write acc i2c\n");
#endif
		goto i2cerr;
	}
	if (pi2cRecData(adxl345_addr,6,buf) != 6) { // read 6 bytes of results
#if (ADXL345_DEBUG)
		printf("Fail to read acc i2c x y z\n");
#endif
		goto i2cerr;
	}

	r->x = scale * ((int16_t)((buf[1] << 8) + buf[0])); //accel_X;
	r->y = scale * ((int16_t)((buf[3] << 8) + buf[2])); //accel_Y;
	r->z = scale * ((int16_t)((buf[5] << 8) + buf[4])); //accel_Z;
	return 0;
i2cerr:
	return -1;
}
