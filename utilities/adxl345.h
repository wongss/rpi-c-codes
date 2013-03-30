// adxl345.h

#ifndef ADXL345_H
#define ADXL345_H

#include <stdint.h>

typedef struct  {
	float x,y,z;
} accel_result_t;

#ifdef __cplusplus
extern "C" {
#endif

int adxl345Init(uint8_t addr, uint8_t res, uint8_t range);
int adxl345Read( accel_result_t *r );

#ifdef __cplusplus
}
#endif

#endif
