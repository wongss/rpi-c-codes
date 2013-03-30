// pkey.h

#ifndef __PKEY_H
#define __PKEY__H

#include <unistd.h>
#include "pdelays.h"

#ifdef __cplusplus
extern "C" {
#endif

#define keyI2cAddr 0x3a

#define DEBOUNCE_MS 50

#define NOKEY_SCANCODE 0xff
#define NOKEY_CODE 0xff

// ---------------------------------------------
// returns 0 for no error
// -1 for error
int keyInitI2c ( void );

// ---------------------------------------------
// returns -1 for no key press
// ascii code for key pressed
int keyChk ( void );

// ---------------------------------------------
// Any key pressed right now
// non zero for yes
// 0 for no
int keyRawPressed ( void );

#ifdef __cplusplus
}
#endif

#endif
