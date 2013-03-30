// serial1.c for rpi
// Uses wiringSerial.c
// Add in 2 serial functions to wiringSerial.c

#include "pserial.h"

// -----------------------------------------------
int serialPutData (int fd, uint8_t *s , int n) {
	return write (fd, s, n) ;
}
// -----------------------------------------------
int serialGetData (int fd, uint8_t *s , int n) {
	int avail;
	avail = serialDataAvail(fd);
	if (n > avail) {
		n = avail;
	}
	if (read (fd, s, n) != n) {
		return -1 ;
	}
	return n;
}
