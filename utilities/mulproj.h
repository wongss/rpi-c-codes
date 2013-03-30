// mulproj.h

#ifndef _MULPROJ_H // prevent problem if included multi times
#define _MULPROJ_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "pserial.h"
#include "plcd.h"
#include "pi2c.h"
#include "pkey.h"
#include "adxl345.h"
#include "getconsolekey.h"

#define LEDPIN 0
#define SWPIN 1

PI_THREAD (thread_accelerometer);
PI_THREAD (thread_blink);
PI_THREAD (thread_ioexp);

// mulmain.c
void sysClose(int status); // system cleanup and quit program with status

uint8_t swIn( void );
void ledOut(uint8_t val);
 
#endif
