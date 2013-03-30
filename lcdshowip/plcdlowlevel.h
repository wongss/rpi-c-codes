// plcdLowLevel.h

#ifndef PLCDLOWLEVEL_H
#define PLCDLOWLEVEL_H

#include <stdint.h>
// whether 4 or 8-bit interface is used, do take care in plcdLowLevel.c
// Internally, 4-bit and 8-bit intialization sequence is different !!!!

#define LCD_INTERFACE_4 1 // 0 for 8-bit

#define LCD_I2C_ADDR 0x21

// return -1 for error
int lcdInterfInit( void ); // set port pins .., init sqeuence for 4 or 8-bit
int lcdWriteCmd8(uint8_t cmd8);
int lcdWriteData8(uint8_t data8);

#endif
