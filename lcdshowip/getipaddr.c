// getipaddr.c

#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdio.h>
#include "getipaddr.h"

// -------------------------------------------------
// return: 0 not found, 1 found, -1 error
// host must be declared as char host[NI_MAXHOST];
// NI_MAXHOST is declared in netdb.h
int getIPAddr(const char *iname, char *host) {
	struct ifaddrs *ifaddr, *ifa;
	int s, status;

	status = 0; // assumeing not found
	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return -1;
	}
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL) {
			continue;  
		}
		s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

		if((strcmp(ifa->ifa_name, iname)==0)&&(ifa->ifa_addr->sa_family==AF_INET)) {
			if (s != 0) {
				// printf("getnameinfo() failed: %s\n", gai_strerror(s));
				freeifaddrs(ifaddr);
				return -1;
			}
			printf("\tInterface : <%s>\n",ifa->ifa_name );
			printf("\t  Address : <%s>\n", host);
			status = 1; 
			break;	// found, quit with status 1
		}
	}
	freeifaddrs(ifaddr);
	return status;
}
