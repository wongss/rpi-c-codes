// pi2c.c for rpi

#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "pi2c.h"

#if (SEMAPHORE_USED)
#include <semaphore.h>
static sem_t i2cmutex;
#endif

static int first = 0;
static int i2cfd = 0;
static uint8_t li2caddr = 0;

// ----------------------------------------------
// to be called by main only once before multithreading starts
// -1 if error, fd if OK
int pi2cOpen(const char *name) {
	if (first == 0) {
		if (sem_init(&i2cmutex,0,1) != 0) {
			// failure
			fprintf(stderr, "Error sem_init() in pi2cOpen()");
			exit(1); 
		}
	}
	// need to check open before?
	if (i2cfd != 0) {
		close(i2cfd);
	}
	if ((i2cfd = open(name, O_RDWR)) < 0) {	// Open port for reading and writing
		// printf("Failed to open i2c port\n");
		i2cfd = 0;
		return -1; // exit(1);
	}
	return i2cfd;
}
// ----------------------------------------------
// 0 if not open, non-zero if open
int pi2cIsOpen( void ) {
	if (i2cfd > 0) {
		return i2cfd;
	}
	return 0;
}
// ----------------------------------------------
// -1 if problem. 0 if OK
static int pi2cSetAddr(uint8_t addr7) {
	if (i2cfd == 0) {
		return -1;
	}
	if (li2caddr == addr7) {
		return 0;	// same as last successful address, no action
	}
	if (ioctl(i2cfd, I2C_SLAVE, addr7) < 0) {	// Set port options & addr of device to speak to
		// printf("Unable to get bus access to talk to slave\n");
		li2caddr = 0; // not successful, clear the last successfull address
		return -1; // exit(1);
	}
	li2caddr = addr7;
	return 0;
}
// ----------------------------------------------
// <0 if error, else number of bytes written
static int i2cWriteData(uint8_t *arr, uint16_t n) {
	if (i2cfd == 0) {
		return -1;
	}
	return write(i2cfd, arr, n);
}
// ----------------------------------------------
// <0 if error, else number of bytes read
static int i2cReadData(uint8_t *arr, uint16_t n) {
	if (i2cfd == 0) {
		return -1;
	}
	return read(i2cfd, arr, n);
}
// ----------------------------------------------
int pi2cClose( void) {
	if (i2cfd == 0) {
		return -1;
	}
	if (close(i2cfd) != 0) {
		i2cfd = 0;
		return -1;
	}
	i2cfd = 0;
	return 0;
}
// ----------------------------------------------
int pi2cSendData( uint8_t addr7, uint16_t n,uint8_t *arr ) {
	int status;
	sem_wait(&i2cmutex);
	if (pi2cSetAddr(addr7) < 0) {
		status = -1;
		goto quit;
	}
	status = i2cWriteData(arr, n);
quit:
	sem_post(&i2cmutex);
	return status;
}
// ----------------------------------------------
int pi2cRecData( uint8_t addr7, uint16_t n, uint8_t *arr ) {
	int status;
	sem_wait(&i2cmutex);
	if (pi2cSetAddr(addr7) < 0) {
		status = -1;
		goto quit;
	}
	status = i2cReadData(arr, n);
quit:
	sem_post(&i2cmutex);
	return status;
}
// ----------------------------------------------
