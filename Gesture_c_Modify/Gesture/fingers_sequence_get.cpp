

#include "force_process.h"
#include "fingers_sequence_get.h"
#include "tp_data.h"
extern FingerGather_T tTouchFingers;
void fingers_sequence_get(void)
{
  //获取第一指状态
  if(tTouchFingers.Finger0 == 1)
  {
    g_lThisFingersStatus |=  0x0001;
  }
  else
  {
    g_lThisFingersStatus &= ~0x0001;
  }

  if(tTouchFingers.Finger1 == 1)
  {
    g_lThisFingersStatus |=  (0x0001 << 1);
  }
  else
  {
    g_lThisFingersStatus &= ~(0x0001 << 1);
  }

  if(tTouchFingers.Finger2 == 1)
  {
    g_lThisFingersStatus |=  (0x0001 << 2);
  }
  else
  {
    g_lThisFingersStatus &= ~(0x0001 << 2);
  }

  if(tTouchFingers.Finger3 == 1)
  {
    g_lThisFingersStatus |=  (0x0001 << 3);
  }
  else
  {
    g_lThisFingersStatus &= ~(0x0001 << 3);
  }

  if(tTouchFingers.Finger4 == 1)
  {
    g_lThisFingersStatus |=  (0x0001 << 4);
  }
  else
  {
    g_lThisFingersStatus &= ~(0x0001 << 4);
  }
  
  // 计算手指的数量
  g_ucFingers = tTouchFingers.Finger0 + tTouchFingers.Finger1 + tTouchFingers.Finger2 + tTouchFingers.Finger3 + tTouchFingers.Finger4;

}
