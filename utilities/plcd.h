// plcd.h

// updated 20 March 2013
// Wong SS (wss@np.edu.sg)

#ifndef PLCD_H
#define PLCD_H

#include "plcdlowlevel.h"
#include <stdint.h>

// BEWARE in multitasking environment !!!!
// Do not allow > 1 task accessing LCD !

// intelligent version supports CR, LF, BS, auto scrolling
#define LCD_INTELLIGENT 0	// 0 for raw without autoscroll ...
// BEWARE: For intelligent version, due to autowrapping, the last col of last row not usable

// Only one of them can be 1 !!!!!
#define LCD_DIM_1x08	0
#define LCD_DIM_1x16	0
#define LCD_DIM_1x20	0
#define LCD_DIM_2x12	0
#define LCD_DIM_2x16	0
#define LCD_DIM_2x20	0
#define LCD_DIM_2x40	0
#define LCD_DIM_4x16	0
#define LCD_DIM_4x20	1
// ------------------------------
#ifdef __cplusplus
extern "C" {
#endif

// Intelligent version of LCD functions that emulates a dumb terminal
// supports BS ('\b'), CR ('\r'), LF ('\n') and autoscroll and autowrapping
int lcdInit ( void );

#if (LCD_INTELLIGENT)
void lcdAutoCROn ( void ); // when a LF is received, it addes a CR
void lcdAutoCROff ( void );
#endif

void lcdCursorOn ( void );
void lcdCursorOnBlink( void );
void lcdCursorOff ( void );
void lcdCursorSave ( void );
void lcdCursorRestore( void );
int lcdIsIntelligent( void );
void lcdClrLine ( uint8_t i );	// clear a line
void lcdPutStr ( const char *str ); // generic pointer used
void lcdPutSpaces ( uint8_t n );
void lcdClrScr ( void );
void lcdPutCh ( uint8_t ch );
void lcdSetXY ( uint8_t x, uint8_t y );
// X in upper 8-bits, Y in lower 8-bits
uint16_t lcdGetXY ( void );
uint8_t lcdGetX ( void );
uint8_t lcdGetY ( void );
uint8_t lcdGetSizeX ( void );
uint8_t lcdGetSizeY ( void );

void lcdPrintf ( const char *msg, ...);
// ==========================================================================================
// lcd print num
void lcdPutBin8 ( uint8_t num);
void lcdPutBin16 ( uint16_t num);

void lcdPutHex8 ( uint8_t num);
void lcdPutHex16 ( uint16_t num);

void lcdPutDecU8 ( uint8_t num);
void lcdPutDecU8LZ ( uint8_t dat, uint8_t noOfCol );
void lcdPutDecU8LS ( uint8_t dat, uint8_t noOfCol );

void lcdPutDecU16 ( uint16_t num);
void lcdPutDecU16LZ ( uint16_t dat, uint8_t noOfCol ); // leading 0 shown
void lcdPutDecU16LS ( uint16_t dat, uint8_t noOfCol ); // leading 0 changed to space

void lcdPutDecS8 ( int8_t num);
void lcdPutDecS16 ( int8_t num);

#ifdef __cplusplus
}
#endif

#if (LCD_DIM_1x08)
#define LCD_SIZE_X 8
#define LCD_SIZE_Y 1
#endif

#if (LCD_DIM_1x16)
#define LCD_SIZE_X 16
#define LCD_SIZE_Y 1
#endif

#if (LCD_DIM_1x20)
#define LCD_SIZE_X 20
#define LCD_SIZE_Y 1
#endif

#if (LCD_DIM_2x12)
#define LCD_SIZE_X 12
#define LCD_SIZE_Y 2
#endif

#if (LCD_DIM_2x16)
#define LCD_SIZE_X 16
#define LCD_SIZE_Y 2
#endif

#if (LCD_DIM_2x20)
#define LCD_SIZE_X 20
#define LCD_SIZE_Y 2
#endif

#if (LCD_DIM_2x40)
#define LCD_SIZE_X 40
#define LCD_SIZE_Y 2
#endif

#if (LCD_DIM_4x16)
#define LCD_SIZE_X 16
#define LCD_SIZE_Y 4
#endif

#if (LCD_DIM_4x20)
#define LCD_SIZE_X 20
#define LCD_SIZE_Y 4
#endif

#endif
