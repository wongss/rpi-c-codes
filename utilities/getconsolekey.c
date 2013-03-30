// getConsoleKey.c

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include "getconsolekey.h"

// Source: http://stackoverflow.com/questions/2984307/c-key-pressed-in-linux-console

// returns EOF if no key pressed
// Note that key pressed is shown on console

static int wasSet = 0;
static int modeSaved = 0;
static struct termios orig_term_attr;
// -----------------------------------------------------------------
void setConsoleRawMode( void ) {
	struct termios new_term_attr;

	tcgetattr(fileno(stdin), &orig_term_attr);	// save original setting
	memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
	new_term_attr.c_lflag &= ~(ECHO|ICANON);
	new_term_attr.c_cc[VTIME] = 0;
	new_term_attr.c_cc[VMIN] = 0;
	tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);
	
	modeSaved = 1;
	wasSet = 1;
}
// -----------------------------------------------------------------
void setConsoleOrigMode ( void ) {
	if (modeSaved) {
		tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
	}
	wasSet = 0;
}
// -----------------------------------------------------------------
int getConsoleKey( void ) {
	int c;
	if (wasSet) {
		c = fgetc(stdin); // was set before, just read it
	}
	else {
		setConsoleRawMode(); // not set before, set
		c = fgetc(stdin); // was set before, just read it
		setConsoleOrigMode(); // restore
	}
	return c;
}
