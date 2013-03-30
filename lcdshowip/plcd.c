// plcd.c

// Copyright (2013) to
// Electronic & Computer Engineering, School of Engineering
// Ngee Ann Polytechnic, Singapore

// Written/Edited by Wong SS (wss@np.edu.sg)
// 20 Mar 2013

#include "plcd.h"
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

//-------------------------------------------------------------
static uint8_t lcdInitOK;
static uint8_t __lcd_cx, __lcd_cy; // in lcdDefault.c
static uint8_t _lcdCursorSaved;
static uint8_t _lcdSavedX, _lcdSavedY; // saved coordinates

static void _llcdClrScr ( void );
static uint8_t _lcdGetYAddr ( uint8_t y );
//-------------------------------------------------------------
#if (LCD_INTELLIGENT)
static void _llcdPutCh ( uint8_t ch );
static void lcdScrollUp ( void );
static void lcdDoLF ( void );
static void lcdDoCR ( void );
static void lcdDoBS ( void );
#endif
//-------------------------------------------------------------
#if (LCD_INTELLIGENT && (LCD_SIZE_Y > 1))
static uint8_t lcdBuf[(LCD_SIZE_Y-1)*LCD_SIZE_X];
static uint8_t _autoCR;	// auto CR for LF
#endif // LCD_INTELLIGENT

void mysleepms(int ms ) {
	usleep(ms*1000);
}

//-------------------------------------------------------------
void lcdPrintf ( const char *msg, ...) {
	va_list argp ;
	char buffer [1024] ;

	va_start (argp, msg) ;
	vsnprintf (buffer, 1023, msg, argp) ;
	va_end (argp) ;

	lcdPutStr (buffer) ;	
}
//-------------------------------------------------------------
// low level clear screen (intelligent version need to clr buffer)
static void _llcdClrScr ( void ) {
	lcdWriteCmd8(0x01);
	__lcd_cx = __lcd_cy = 0;
	mysleepms(5);
}
//-------------------------------------------------------------
int lcdInit( void ) {	// lower layer could be 4 or 8 bit interface
	lcdInitOK = 0;
	if (lcdInterfInit() < 0) {
		return -1;
	}
	lcdWriteCmd8(0x06);
	mysleepms(1);
	lcdWriteCmd8(0x0e);
	mysleepms(5);

	// If more than 1 line, allows lower lines to scroll up
#if (LCD_INTELLIGENT && LCD_SIZE_Y > 1)
	_autoCR = 1;	// default auto CR for LF
#endif // LCD_INTELLIGENT && LCD_SIZE_Y > 1

	_lcdCursorSaved = 0;
	lcdInitOK = 1; // must be set before calling other functions
	lcdClrScr();
	return 0;
}
//-------------------------------------------------------------
// internal lcd RAM address for that row
static uint8_t _lcdGetYAddr ( uint8_t y ) {
	switch (y) {
		case 0:
			return 0x00;
		case 1:
			return 0x40;
		case 2:
			return 0x14;
		case 3:
			return 0x54;
	}
	return 0xff;	// Should never come here !
}
//-------------------------------------------------------------
// return X in upper 8-bits, Y in lower 8-bits
uint16_t lcdGetXY ( void ) {
	if (!lcdInitOK) return 0;
	return ( (((uint16_t)__lcd_cx)<< 8) + __lcd_cy);
}
//-------------------------------------------------------------
uint8_t lcdGetX ( void ) {
	if (!lcdInitOK) return 0;
	return __lcd_cx;
}
//-------------------------------------------------------------
uint8_t lcdGetY ( void ) {
	if (!lcdInitOK) return 0;
	return __lcd_cy;
}
//-------------------------------------------------------------
uint8_t lcdGetSizeX ( void ) {
	return LCD_SIZE_X;
}
//-------------------------------------------------------------
uint8_t lcdGetSizeY ( void ) {
	return LCD_SIZE_Y;
}
//-------------------------------------------------------------
void lcdCursorOff( void ) {
	if (!lcdInitOK) return;
	lcdWriteCmd8(0x0c); // display on but no cursor
	mysleepms(1);
}
//-------------------------------------------------------------
void lcdCursorOn( void ) {
	if (!lcdInitOK) return;
	lcdWriteCmd8(0x0e);	// display on, cursor on, no blinking
	mysleepms(1);
}
//-------------------------------------------------------------
void lcdCursorOnBlink( void ) {
	if (!lcdInitOK) return;
	lcdWriteCmd8(0x0f);	// display on, cursor on, blink
	mysleepms(1);
}
//-------------------------------------------------------------
// can only save 1 level (i.e. no stack is implemented !!!)
void lcdCursorSave( void ) {
	if (!lcdInitOK) return;
	_lcdCursorSaved = 1;
	_lcdSavedX = __lcd_cx;
	_lcdSavedY = __lcd_cy;
}
//-------------------------------------------------------------
void lcdCursorRestore( void ) {
	if (!lcdInitOK) return;
	if (_lcdCursorSaved) {
		lcdSetXY(_lcdSavedX,_lcdSavedY);
	}
}
//-------------------------------------------------------------
int lcdIsIntelligent( void ) {
	return LCD_INTELLIGENT;
}
//-------------------------------------------------------------
void lcdClrLine ( uint8_t lineNo ) {	// 0 to LCD_SIZE_Y-1
	uint8_t i;
	if (!lcdInitOK) return;
	if (lineNo >= LCD_SIZE_Y) {	// beyond screen height !!
		return;
	}
	if (LCD_SIZE_Y == 1) {	// single row LCD, just clear screen
		lcdClrScr();
		return;
	}
	lcdSetXY(0,lineNo);	// set cursor to beginning of line

#if (!LCD_INTELLIGENT) // raw version, just output ' ' for the whole line
	for ( i = 0; i < LCD_SIZE_X; i++ ) {	// 1 column less
		lcdPutCh(' ');
	}
#endif

#if (LCD_INTELLIGENT) // intelligent version
	// handling 1st column till one just before last column
	for (i=0;i<(LCD_SIZE_X-1);i++) {	// 1 column less
		lcdPutCh(' ');
	}
	// handling last column
	// not last row, output last ' '
	if (lineNo != (LCD_SIZE_Y-1)) {
		lcdPutCh(' ');
	}
	// if last row, no need to do anything. It is not possible to display last col at that line
#endif
	// cursor set to beginning of the line
	lcdSetXY(0,lineNo);
}
//-------------------------------------------------------------
void lcdSetXY ( uint8_t x, uint8_t y ){
	if (!lcdInitOK) return;
	if ((x < LCD_SIZE_X) && (y < LCD_SIZE_Y)){
		__lcd_cx = x;
		__lcd_cy = y;
		x = _lcdGetYAddr(y) + x;	// x used as lcd address
		lcdWriteCmd8(x | 0x80);	// set the address (msb = 1)must set if not will have other meaning
		mysleepms(2);
	}
}
//-------------------------------------------------------------
void lcdPutSpaces ( uint8_t n ) {
	while(n) {
		lcdPutCh(' ');
	}
}
// ============================================================================
#if (LCD_INTELLIGENT)
void lcdAutoCROn( void ) {
	_autoCR = 1;
}

void lcdAutoCROff ( void ) {
	_autoCR = 0;
}
// clear line buffer to spaces
void clrlcdBuf (void) {
	int i;
	if (LCD_SIZE_Y > 1)	{	// more than 1 row
		i = ((LCD_SIZE_Y - 1) * LCD_SIZE_X) - 1;
		while(1) {
			lcdBuf[i]=' ';	// use of pointer => more codes !
			if (i==0) break;
			i--;
		}
	}
}
//------------------------------------------------------
static void _llcdPutCh ( uint8_t ch ) {
	lcdWriteData8(ch);	// low level write to LCD current cursor location
	mysleepms(2);
}
//------------------------------------------------------
static void lcdScrollUp ( void ) {
	uint8_t col;
	uint8_t row;
	uint8_t i;
	// remember where the cursor is currently located
	_llcdClrScr(); // low level clrScr without clearing line 2 buffer
	// disply buffer content to upper lines
	if (LCD_SIZE_Y<2) return;	// not applicable for 1 line lcd
	i=0;
	for (row=0;row<(LCD_SIZE_Y-1);row++) {
		lcdSetXY(0,row);
		for(col=0;col<LCD_SIZE_X;col++) {
			_llcdPutCh(lcdBuf[i]);
			i++;
		}
	}
	row = LCD_SIZE_X*(LCD_SIZE_Y-2);
	// copy buffer up 1 line
	for(i=0;i<row;i++) {
		lcdBuf[i] = lcdBuf[i+LCD_SIZE_X];
	}
	// clear last line
	for(row=0;row<LCD_SIZE_X;row++,i++) {
		lcdBuf[i] = ' ';
	}
	// set cursor back to where it was
	lcdSetXY(0,LCD_SIZE_Y-1); // this also updates __lcd_cx and __lcd_cy
}

static void lcdDoLF ( void ) {
	uint8_t ox;
	if (__lcd_cy < (LCD_SIZE_Y-1)) {
		// now in 1st line, set cursor to 2nd line
		lcdSetXY(__lcd_cx,__lcd_cy+1);
	}
	else {
		// now in 2nd line, scroll up!
		ox = __lcd_cx;
		lcdScrollUp();	// this changes __lcd_cx, thus save in ox first
		lcdSetXY(ox,LCD_SIZE_Y-1);
	}
}

static void lcdDoCR ( void ) {
	lcdSetXY(0,__lcd_cy);
}

static void lcdDoBS ( void ) {
	if (__lcd_cx != 0) {
		lcdSetXY(__lcd_cx-1, __lcd_cy);
	}
}
#endif
// ============================================================================
// Intelligent version of 2 basic LCD functions
#if (LCD_INTELLIGENT)
//-------------------------------------------------------------
// Intelligent version
// this clear screen includes clearing internal line buffer content
void lcdClrScr ( void ) {
	if (!lcdInitOK) return;
	_llcdClrScr();	// low level clear screen
	clrlcdBuf();	// must also clear line(s) buffer
}
//-------------------------------------------------------------
// Intelligent version
void lcdPutCh ( uint8_t ch ) {
	if (!lcdInitOK) return;
	switch(ch) {
		case '\n':
			lcdDoLF ();
			if (_autoCR) {
				lcdDoCR();
			}
			return;
		case '\r':
			lcdDoCR();
			return;
		case '\b':
			lcdDoBS();
			return;
	}
	_llcdPutCh(ch);
	// update buffer
	if (__lcd_cy != 0) {
		// Save to buffer at that x,y coord! For scroll up function
		lcdBuf[((__lcd_cy-1)*LCD_SIZE_X)+__lcd_cx] = ch;
	}
	// wraping or scrolling
	if ( __lcd_cx == (LCD_SIZE_X-1)) {
		// hit last column of a row
		__lcd_cy++;
		if (__lcd_cy != LCD_SIZE_Y) {
			// now at end of 1st row, move to 2nd row
			lcdSetXY(0,__lcd_cy);	// col 0 for next row
		}
		else {
			// now at end of 2nd row, scroll up!
			lcdScrollUp();
		}
	}
	else {
		__lcd_cx++;	//simply go to next column (x)
	}
}
#endif
// ============================================================================
// Raw version of 2 basic LCD functions
#if (!LCD_INTELLIGENT)
//-------------------------------------------------------------
// Raw version
void lcdClrScr ( void ) {
	if (!lcdInitOK) return;
	_llcdClrScr();
}
//-------------------------------------------------------------
// Raw version
void lcdPutCh ( uint8_t ch ) {
	if (!lcdInitOK) return;
	if (__lcd_cx >= (LCD_SIZE_X)) {
		return; // exceed screen width, do nothing
	}
	lcdWriteData8(ch);	// Does not support CR and LF
	__lcd_cx++;
	mysleepms(2);
}
#endif
