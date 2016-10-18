/*  Collection of aSub subroutines...
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dbDefs.h>
#include <alarm.h>
#include <registryFunction.h>
#include "aSubRecord.h"
#include <epicsExport.h>
#include <math.h>
#include <stdlib.h>

#define SIZE(x)               (sizeof(x)/sizeof(x[0]))

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;

int debug=0;  /* Set to 1 to enable debug printf staements */

static int bytes2msec(byte byte1, byte byte2);
static int bytes2int(byte byte1, byte byte2);

static long asStatus(aSubRecord *pr){
/*------------------------------------------------------------------------------
Parses delay and exposure status bytes from device and sends to records.
*----------------------------------------------------------------------------*/
    //int debug=1;

    unsigned char *delayByte1 = (unsigned char*)pr->a;
    unsigned char *delayByte4 = (unsigned char*)pr->b;
    unsigned char *delayByte5 = (unsigned char*)pr->c;
    unsigned char *expByte1 = (unsigned char*)pr->d;
    unsigned char *expByte4 = (unsigned char*)pr->e;
    unsigned char *expByte5 = (unsigned char*)pr->f;
    unsigned char *freerunByte2 = (unsigned char*)pr->g;
    unsigned char *freerunByte3 = (unsigned char*)pr->h;
    unsigned int  *msecDelay = (unsigned int*)pr->vala;
    unsigned int  *msecExp = (unsigned int*)pr->valb;
    unsigned int  *nFreerun = (unsigned int*)pr->valc;

    if(debug) printf("delayByte1=%#04x, delayByte4=%#04x, delayByte5=%#04x\n", *delayByte1,*delayByte4,*delayByte5);
    if(debug) printf("expByte1=%#04x, expByte4=%#04x, expByte5=%#04x\n", *expByte1,*expByte4,*expByte5);
    if(debug) printf("freerunByte2=%#04x, freerunByte3=%#04x\n", *freerunByte2,*freerunByte3);
   
    *msecDelay=bytes2msec(*delayByte4,*delayByte5);        
    *msecExp=bytes2msec(*expByte4,*expByte5);        
    *nFreerun=bytes2int(*freerunByte2,*freerunByte3);

    
 
    return(0);
}

static long asDelayExp(aSubRecord *pr){
/*------------------------------------------------------------------------------
Gets delay or exposure time from records and formats to send to device.
*----------------------------------------------------------------------------*/
    #define NDIGITS 3
    //int debug=1;

    unsigned int  *msec = (unsigned int*)pr->a;
    unsigned char *msecByte1 = (unsigned char*)pr->vala;
    unsigned char *msecByte2 = (unsigned char*)pr->valb;

    int digits[NDIGITS] = { 0 };
    int i=0,tenths,ones,tens,hundreds;

    while(*msec > 0) {
        digits[i] = *msec % 10;
        *msec /= 10;
        ++i;
    }
    
    if(debug) {
        for (i = 0; i < NDIGITS; ++i) {
            printf("digits[%d] = %d\n", i, digits[i]);
        }    
    }

    ones=digits[0];
    tens=digits[1];
    hundreds=digits[2];
    tenths=0;
    if(debug) printf("set delay: ones=%d, tens=%d, hundreds=%d\n",ones,tens,hundreds);

    *msecByte1=(byte)(hundreds << 4 | tens);
    *msecByte2=(byte)(ones << 4 | tenths);
    if(debug) printf("set delay: msecByte1=%#04x, msecByte2=%#04x\n", *msecByte1,*msecByte2);
    
    return(0);
}

static long asFreerun(aSubRecord *pr){
/*------------------------------------------------------------------------------
Gets freerun from record as an int and splits into 2 bytes to send to device.
*----------------------------------------------------------------------------*/
    //int debug=1;

    unsigned int  *freeRunN = (unsigned int*)pr->a;
    unsigned char *freeRunByte1 = (unsigned char*)pr->vala;
    unsigned char *freeRunByte2 = (unsigned char*)pr->valb;

    *freeRunByte1=(byte)(*freeRunN >> 8);
    *freeRunByte2=(byte)*freeRunN;
    if(debug) printf("freeRunN=%d, freeRunByte1=%#04x, freeRunByte2=%#04x\n", *freeRunN,*freeRunByte1,*freeRunByte2);
    
    return(0);
}

static int bytes2msec(byte byte1, byte byte2) {
    byte hundreds,tens,ones;
    int msec;
    //int debug=1;

    hundreds=(byte)((byte1 & 0xF0) >> 4);
    tens=(byte)(byte1 & 0x0F);
    ones=(byte)((byte2 & 0xF0) >> 4);
    msec=100*hundreds + 10*tens + ones;
    if(debug) printf("get msec: hundreds=%d, tens=%d, ones=%d, msec=%d\n", hundreds,tens,ones,msec);
    return(msec);
}

static int bytes2int(byte byte1, byte byte2) {
    int number;
    //int debug=1;

    number=(byte1 << 8) | byte2;
    if(debug) printf("get number: number=%d\n", number);
    return(number);
}

/**************************************************************************/
epicsRegisterFunction(asStatus);
epicsRegisterFunction(asDelayExp);
epicsRegisterFunction(asFreerun);



