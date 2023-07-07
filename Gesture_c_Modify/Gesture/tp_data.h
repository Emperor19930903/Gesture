

#ifndef _TP_DATA_H
#define _TP_DATA_H
#include <stdint.h>


struct FingerGather_T
{
    int X0;
    int Y0;
    int Size0;
    int Finger0;
    int X1;
    int Y1;
    int Size1;
    int Finger1;
    int X2;
    int Y2;
    int Size2;
    int Finger2;
    int X3;
    int Y3;
    int Size3;
    int Finger3;
    int X4;
    int Y4;
    int Size4;
    int Finger4;
    int Palm0;
    int Palm1;
    int Palm2;
    int Palm3;
    int Palm4;
    uint8_t tpKeepFlag;
};

//struct FingerGather_T tTouchFingers;


struct TouchGather_T
{
	uint8_t tpIntFlag:1;
    uint8_t pressFlag:1;
	uint8_t releaseFlag:1;
	uint8_t tpSendFlag:1;
	uint8_t tpOpenFlag:1;
	uint8_t tpRecFlag:1;
	uint8_t passTouchFlag:1;
	uint8_t disenableTouchFlag:1;
	uint8_t	resetAttnFlag:1;
	uint8_t resetAttnCount:1;
	uint8_t touchRawdataFlag:1;
	uint8_t tpGetData[64];
	uint8_t tpBuffer[64]; 
	uint16_t readPassLength;

};
//struct TouchGather_T tTouchVariate;

#endif
