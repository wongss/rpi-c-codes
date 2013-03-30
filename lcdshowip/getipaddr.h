// getipaddr.h

#ifndef _GETIPADDR_H // prevent problem if included multi times
#define _GETIPADDR_H

// getipaddr.c
// -------------------------------------------------
// return: 0 not found, 1 found, -1 error
// host must be declared as char host[NI_MAXHOST];
// NI_MAXHOST is declared in netdb.h
int getIPAddr(const char *iname, char *host);

#endif
