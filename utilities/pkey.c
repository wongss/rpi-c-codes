#include <stdio.h>
#include "pi2c.h"
#include "pkey.h"

#define msleep(x) usleep((x)*1000)

static TDlyData_t keyDlyData;
static uint8_t keyInitOK=0;
static int keyOld;
// ---------------------------------------------
// returns -1 if no key pressed
static int keyScan( void ) {
	uint8_t buf[1], k;
	buf[0] = 0x0f;
	if (pi2cSendData(keyI2cAddr,1,buf) != 1) {
		fprintf(stderr,"\nError do i2c write to keypad at %02x",keyI2cAddr);
		return -1;
	}
	if (pi2cRecData(keyI2cAddr,1,buf) != 1) {
		fprintf(stderr,"\nError do i2c read from keypad at %02x",keyI2cAddr);
		return -1;
	}
	k = buf[0] & 0x0f;
	if (k == 0x0f) {
		return NOKEY_SCANCODE;
	}
	buf[0] = 0xf0;
	pi2cSendData(keyI2cAddr,1,buf); // assumes no error ?
	pi2cRecData(keyI2cAddr,1,buf);
	return (k | (buf[0] & 0x0f0));
}
// ---------------------------------------------
// pi2cOpen() must be called before this
// returns 0 for no error
// -1 for error
int keyInitI2c ( void ) {
	keyOld = keyScan();
	if (keyOld == -1) {
		return -1;
	}
	keyInitOK = 1;
	timeDlySet(&keyDlyData,DEBOUNCE_MS);	// debounce time
	return 0;
}
// ---------------------------------------------
uint8_t translateTable[] = {
	0xee,'1',0xde,'2',0xbe,'3',0x7e,'A',
	0xed,'4',0xdd,'5',0xbd,'6',0x7d,'B',
	0xeb,'7',0xdb,'8',0xbb,'9',0x7b,'C',
	0xe7,'*',0xd7,'0',0xb7,'#',0x77,'D',
	0xff,0xff
};
static int keyTranslate( int scode ) {
	int i;
	i=0;
	while(1) {
		if (translateTable[i] == scode) {
			return translateTable[i+1];
		}
		if (translateTable[i] == 0xff) {
			return NOKEY_CODE;
		}
		i+=2;
	}
}
// ---------------------------------------------
// Assumes that this is called at interval there will not have bounce problem
// returns -1 for no key press
// ascii code for key pressed
int keyChk ( void ) {
	int k;
	if (!keyInitOK) return NOKEY_CODE;
	// ensure called at minimal DEBOUNCE_MS interval
	k = 50;
	while(!timeDlyExpired(&keyDlyData)) {
		msleep(10);
		k--;	// just in case !!! timeout in 500ms
		if (k == 0) {
			break;
		}
	}
	timeDlySet(&keyDlyData,DEBOUNCE_MS);
	if ((k = keyScan()) != keyOld) {
		// change
		keyOld = k;
		return keyTranslate(k);
	}
	// no change
	return NOKEY_CODE; // no key pressed
}
// ---------------------------------------------
// Any key pressed right now?
// 1 for yes, 0 for no, -1 for error
int keyRawPressed ( void ) {
	int i;
	if (!keyInitOK) return 0; // error, consider none pressed
	if ((i=keyScan()) == NOKEY_SCANCODE) {
		return 0;	// none pressed
	}
	if (i != -1) {
		return 1;
	}
	return -1;
}
// ---------------------------------------------
