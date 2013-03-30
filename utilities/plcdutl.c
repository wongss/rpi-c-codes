// plcdutl.c

// Copyright (2013) to
// Electronic & Computer Engineering, School of Engineering
// Ngee Ann Polytechnic, Singapore

// Written/Edited by Wong SS (wss@np.edu.sg)
// 14 Mar 2013

#include "plcd.h"
#include <string.h>
#include <stdio.h>

//-------------------------------------------------------------
void lcdPutStr(const char *s) {
	while(*s) {
		lcdPutCh(*s);
		s++;
	}
}
//-------------------------------------------------------------
void lcdPutBin8 ( uint8_t num) {
	int i;
	for(i=0; i<8; i++) {
		if ((num & 0x080) != 0) {
			lcdPutCh('1');
		}
		else {
			lcdPutCh('0');
		}
		num = num << 1;
	}
}
//-------------------------------------------------------------
void lcdPutBin16 ( uint16_t num) {
	lcdPutBin8(num>>8);
	lcdPutBin8(num);	
}
//-------------------------------------------------------------
void lcdPutHex8 ( uint8_t num) {
	char buf[3];	// "biggest" FF
	sprintf(buf,"%02X",num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutHex16 ( uint16_t num) {
	char buf[5];	// "biggest" FFFF
	sprintf(buf,"%04X",num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecU8 ( uint8_t num) {
	char buf[4];	// "biggest" 255
	sprintf(buf,"%d",num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecU8LZ ( uint8_t num, uint8_t noOfCol ) {
	char fbuf[5], bufn[2], buf[4];	// "biggest" 255
	if (noOfCol > 3) {
		noOfCol = 3;
	}
	strcpy(fbuf,"\%0");
	sprintf(bufn,"%d",noOfCol);
	strcat(fbuf,bufn);
	strcat(fbuf,"d");
	sprintf(buf,fbuf,num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecU8LS ( uint8_t num, uint8_t noOfCol ) {
	char fbuf[5], bufn[2], buf[4];	// "biggest" 255
	if (noOfCol > 3) {
		noOfCol = 3;
	}
	strcpy(fbuf,"\%");
	sprintf(bufn,"%d",noOfCol);
	strcat(fbuf,bufn);
	strcat(fbuf,"d");
	sprintf(buf,fbuf,num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecU16 ( uint16_t num) {
	char buf[6];	// "biggest" 65535
	sprintf(buf,"%d",num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecU16LZ ( uint16_t num, uint8_t noOfCol ) { // leading 0 shown
	char fbuf[5], bufn[2], buf[6];	// "biggest" 65535
	if (noOfCol > 5) {
		noOfCol = 5;
	}
	strcpy(fbuf,"\%0");
	sprintf(bufn,"%d",noOfCol);
	strcat(fbuf,bufn);
	strcat(fbuf,"d");
	sprintf(buf,fbuf,num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecU16LS ( uint16_t num, uint8_t noOfCol ) { // leading 0 changed to space
	char fbuf[5], bufn[2], buf[6];	// "biggest" 65535
	if (noOfCol > 5) {
		noOfCol = 5;
	}
	strcpy(fbuf,"\%");
	sprintf(bufn,"%d",noOfCol);
	strcat(fbuf,bufn);
	strcat(fbuf,"d");
	sprintf(buf,fbuf,num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecS8 ( int8_t num) {
	char buf[5];	// "biggest" -128
	sprintf(buf,"%d",num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
void lcdPutDecS16 ( int8_t num) {
	char buf[7];	// "biggest" -32xxx
	sprintf(buf,"%d",num);
	lcdPutStr(buf);
}
//-------------------------------------------------------------
