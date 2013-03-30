// lcdshowip.c

#include <netdb.h>
#include "getipaddr.h"
#include "lcdshowip.h"

#define ATTEMPTS 15

// -------------------------------------------------
void sysInit ( void ) {
	if (pi2cOpen("/dev/i2c-1") < 0) {
		fprintf(stderr, "pi2cOpen() error. Exit\n");
		exit(1);
	}
	// These must be done after pi2cOpen()
	if (lcdInit() < 0 ) {
		fprintf(stderr, "lcdInit() error. Exit\n");
		exit(1);
	}
}
// -------------------------------------------------
void sysClose( int status ) {
	// clean up ...
	printf("\nCleaning up & exit with status %d\n",status);
	pi2cClose();
	exit(status);
}
// -------------------------------------------------
int getIPAddrAttempts(const char *iname, char *host, int attempts) {
	int i, status;
	for (i=0;i<attempts;i++) {
		if ((status=getIPAddr(iname,host)) == 1) {
			return 1; // found
			break;
		}
		else if (status == 0) {
			lcdPutCh('.');
		}
		else if (status == -1) {
			lcdPutStr("get ip addr err");
			sysClose(1);
		}
		sleep(1);
	}
	return 0; // not found
}
// -------------------------------------------------
int main ( void ) {
	int status;
    char host[NI_MAXHOST];

	sysInit();

	lcdSetXY(0,0);
	if ((status=getIPAddrAttempts("eth0",host,ATTEMPTS)) == 1) {
		lcdSetXY(0,0);
		lcdPutStr(host);
	}
	else if (status == 0) {
		lcdSetXY(0,0);
		lcdPutStr("eth0  no ip addr");
	}

	lcdSetXY(0,1);
	if ((status=getIPAddrAttempts("wlan0",host,ATTEMPTS)) == 1) {
		lcdSetXY(0,1);
		lcdPutStr(host);
	}
	else if (status == 0) {
		lcdSetXY(0,1);
		lcdPutStr("wlan0 no ip addr");
	}
	sysClose(0);
	return 0;
}
