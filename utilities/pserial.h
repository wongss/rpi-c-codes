// pserial.h
// Add in 2 serial functions to wiringSerial.c (in library)

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>
#include <unistd.h>
//#include <errno.h>
//#include "wiringSerial.h"

// Copied from wiringSerial.h (functions in library) and added 2 functions
#ifdef __cplusplus
extern "C" {
#endif

extern int   serialOpen      (char *device, int baud) ; // returns fd or <0 for error
extern void  serialClose     (int fd) ;
extern void  serialFlush     (int fd) ; // remove all data at receiver
extern void  serialPutchar   (int fd, unsigned char c) ;
extern void  serialPuts      (int fd, char *s) ;
extern void  serialPrintf    (int fd, char *message, ...) ;
extern int   serialDataAvail (int fd) ; // how many bytes available at the receiver
extern int   serialGetchar   (int fd) ; // -1 if no data; better to use serialDataAvail() 1st

// Added to wiringPi library
extern int serialPutData (int fd, uint8_t *s , int n); // return how many sent
// Use serialDataAvail() to determine how many bytes available first
extern int serialGetData (int fd, uint8_t *s , int n); // return how many read

#ifdef __cplusplus
}
#endif

#endif
