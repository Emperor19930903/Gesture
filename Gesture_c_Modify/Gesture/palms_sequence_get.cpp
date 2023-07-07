
#include "force_process.h"
#include "palms_sequence_get.h"
#include "tp_data.h"

extern FingerGather_T tTouchFingers;

void palm_sequence_get(void)
{
    //获取palm状态
    if(tTouchFingers.Palm0 == 1)
        g_lThisPalmStatus |=  0x0001;
    else
        g_lThisPalmStatus &= ~0x0001;
    if(tTouchFingers.Palm1 == 1)
        g_lThisPalmStatus |=  (0x0001 << 1);
    else
        g_lThisPalmStatus &= ~(0x0001 << 1);
    if(tTouchFingers.Palm2 == 1)
        g_lThisPalmStatus |=  (0x0001 << 2);
    else
        g_lThisPalmStatus &= ~(0x0001 << 2);
    if(tTouchFingers.Palm3 == 1)
        g_lThisPalmStatus |=  (0x0001 << 3);
    else
        g_lThisPalmStatus &= ~(0x0001 << 3);
    if(tTouchFingers.Palm4 == 1)
        g_lThisPalmStatus |=  (0x0001 << 4);
    else
        g_lThisPalmStatus &= ~(0x0001 << 4);
	
	//计算Palm的数量 
	g_ucPalms = tTouchFingers.Palm0 + tTouchFingers.Palm1 + tTouchFingers.Palm2 + tTouchFingers.Palm3 + tTouchFingers.Palm4;
}