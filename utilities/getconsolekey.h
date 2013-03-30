// getconsolekey.h

#ifndef _GETCONSOLKEY_H
#define _GETCONSOLKEY_H

#ifdef __cplusplus
extern "C" {
#endif

void setConsoleRawMode( void );
void setConsoleOrigMode ( void );
int getConsoleKey( void );

#ifdef __cplusplus
}
#endif

#endif
