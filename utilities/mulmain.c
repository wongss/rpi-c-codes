// mulmain.c

#include "mulproj.h"

// -------------------------------------------------
uint8_t swIn ( void ) {
	return digitalRead(SWPIN);
}
// -------------------------------------------------
void ledOut(uint8_t val) {
	digitalWrite(LEDPIN, val);
}
// -------------------------------------------------
void sysInit ( void ) {
	printf("Embarking on wiringPi ...\nPress 'E' to quit\n");
	if (wiringPiSetup () == -1) {
		fprintf(stderr, "wiringPiSetup() error. Exit\n");
		exit (1) ;
	}
	if (pi2cOpen("/dev/i2c-1") < 0) {
		fprintf(stderr, "pi2cOpen() error. Exit\n");
		exit(1);
	}
	// These must be done after pi2cOpen()
	if (lcdInit() < 0 ) {
		fprintf(stderr, "lcdInit() error. Exit\n");
		exit(1);
	}
	if (keyInitI2c() < 0 ) {
		fprintf(stderr, "keyInitI2c() error. Exit\n");
		exit(1);
	}
	pinMode(LEDPIN, OUTPUT);
	pinMode(SWPIN, INPUT);
	pullUpDnControl (SWPIN, PUD_UP);
	printf("\nCreating threads: accelerometer, blink and ioexpander");
	if (piThreadCreate(thread_accelerometer) != 0) {
		printf("\nCreate thread_accelerometer failed");
	}
	if (piThreadCreate(thread_blink) != 0) {
		printf("\nCreate thread_blink failed");
	}
	if (piThreadCreate(thread_ioexp) != 0) {
		printf("\nCreate thread_ioexp failed");
	}
}

void sysClose( int status ) {
	// clean up ...
	printf("\nQuiting program with status %d\n",status);
	exit(status);
}
// -------------------------------------------------
void mylcdPutCh( int k) {
	int y;
	if (lcdIsIntelligent()) {
		lcdPutCh(k);
	}
	else {
		if (lcdGetX() == lcdGetSizeX()) {
			// had hit last column, should go next line
			if ((y=lcdGetY()) == lcdGetSizeY()-1) {
				lcdSetXY(0,0);
			}
			else {
				lcdSetXY(0,y+1);
			}
		}
		lcdPutCh(k);
	}
}
// -------------------------------------------------
int main ( void ) {
	int k;
	sysInit();
	
	lcdPutStr("Welcome to RPi");
	lcdPrintf("\n%03d",7);
	setConsoleRawMode();
	while(1) {
		if ((k=keyChk()) != NOKEY_CODE) {
			mylcdPutCh(k);
		}
		if ((k = getConsoleKey()) == 'E') {
			setConsoleOrigMode();
			sysClose(0); // normal quit
		}
	}
	sysClose(0);
	return 0;
}
