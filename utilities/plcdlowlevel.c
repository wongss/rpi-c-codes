// plcdLowLevel.c

// written by wong ss (modify from fromer 8051 library codes)
// 14 March 2013, Ngee Ann Polytechnic, Electronic & Computer Engineering

#include "plcdlowlevel.h"
#include <stdint.h>
#include <unistd.h>
#include "pi2c.h"

// =================================================================
#if (LCD_INTERFACE_4) // 4-bit interface
// For 4-bit interface using parallel HW (rather than using I2C IO Expander),
// just have to overwrite _lcdWriteNibble()
// -----------------------------------------------------------------
// 0 - D4, 1 - D5, 2 - D6, 3 - D7, 4 - RS, 5 - RW, 6 - E, 7 - always write 1
static int _lcdWriteNibble ( uint8_t c, uint8_t rs) { // rs=0 for instr, non-zero for data
	uint8_t tmp[2];
	tmp[0] = (c & 0x0f) | 0xC0; // 1100xxxx, i.e. ensure E is high
	if (rs != 0) { // 110?xxxx // RS high or low
		tmp[0] = tmp[0] | 0x10;	// RS bit set to 1 for data, see connection above (MPI Oct 07)
	}
	tmp[1] = tmp[0] & 0xbf; // 100?xxxx, i.e. E goes low
	
	if (pi2cSendData(LCD_I2C_ADDR,2,tmp) < 0 ) {	// E 90us high then low (min 90us)
		return -1;
	}
	return 0;
}
// -----------------------------------------------------------------
int lcdWriteCmd8 (uint8_t c ) {
	if (_lcdWriteNibble (c >> 4, 0) < 0 ) {	// 0 for instruction reg
		return -1;
	}
	return _lcdWriteNibble (c, 0);
}

int lcdWriteData8 ( uint8_t c ) {
	if (_lcdWriteNibble (c >> 4, 1) < 0) {	// 1 for data reg
		return -1;
	}
	return _lcdWriteNibble (c, 1);
}
#endif
// =================================================================
#if (!LCD_INTERFACE_4) // 8-bit interface
// Not implemented yet
// return -1 for error
static int _lcdWriteByte ( uint8_t c, uint8_t rs); // rs=0 for instr, non-zero for data
// -----------------------------------------------------------------
int lcdWriteCmd8 (uint8_t c ) {
	return _lcdWriteByte (c, 0);	// 0 for instruction reg
}
// -----------------------------------------------------------------
int lcdWriteData8 ( uint8_t c ) {
	return _lcdWriteByte (c, 1) ;	// 1 for data reg
}
#endif
// =================================================================
int lcdInterfInit ( void ) {
#if (LCD_INTERFACE_4) // 4-bit interface
	usleep(100*1000);
	if (_lcdWriteNibble(0x03,0) < 0) {	// set to 8-bit 3 X first (default?)
		return -1;
	}
	usleep(20*1000);
	_lcdWriteNibble(0x03,0); // assume no possible to have error from now
	usleep(20*1000);
	_lcdWriteNibble(0x03,0);
	usleep(20*1000);
	_lcdWriteNibble(0x02,0);	// now set to 4 bit
	usleep(20*1000);
	lcdWriteCmd8(0x28);
	usleep(5*1000);
#endif
#if (!LCD_INTERFACE_4) // 8-bit interface
	usleep(100*1000);
	lcdWriteCmd8(0x38);
	usleep(5*1000);
	lcdWriteCmd8(0x38);
	usleep(1*1000);
	lcdWriteCmd8(0x38);
	usleep(1*1000);
#endif
	return 0;
}
/*static int _lcdWriteNibble ( uint8_t c, uint8_t rs) { // rs=0 for instr, non-zero for data
	// 10r1xxxx write a nibble
	// 10r0xxxx
	uint8_t tmp;
	tmp = (c & 0x0f) | 0xC0; // 1100xxxx, i.e. ensure E is high
	if (rs != 0) { // 110?xxxx // RS high or low
		tmp = tmp | 0x10;	// RS bit set to 1 for data, see connection above (MPI Oct 07)
	}
	if (pi2cSendData(LCD_I2C_ADDR,1,&tmp) <0 ) {
		return -1;
	}
	tmp = tmp & 0xbf; // 100?xxxx, i.e. E goes low
	if (pi2cSendData(LCD_I2C_ADDR,1,&tmp) <0 ) {
		return -1;
	}
	return 0;
}
*/
