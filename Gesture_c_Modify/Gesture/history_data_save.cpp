
#include "force_process.h"
#include "history_data_save.h"
#include "tp_data.h"


//历史数据存储
extern  TF_PARAMETER g_tTFParaMeter;
extern AcDataGather_T t_AcDataGather;
extern FingerGather_T tTouchFingers;
void history_data_save(void)
{
  int i = 0;
	
  //历史波形
  for(i = 0; i < CHANNEL; i++)
  {
    t_AcDataGather.AcComponentBefore[i] = t_AcDataGather.AcComponentLast[i];

    t_AcDataGather.AcComponentLast[i] = t_AcDataGather.AcComponent[i];

    t_AcDataGather.AcComponentOrigBefore[i] = t_AcDataGather.AcComponentOrigLast[i];

    t_AcDataGather.AcComponentOrigLast[i] = t_AcDataGather.AcComponentOrig[i];
  }
  
  // 手指状态信息缓存

  g_lFlagDragFinger1Last = g_lFlagDragFinger1;

  g_lFlagDragFinger2Last = g_lFlagDragFinger2;

  g_lLastFingersStatus = g_lThisFingersStatus;

  g_sTfFingerX0_last = tTouchFingers.X0;

  g_sTfFingerX1_last = tTouchFingers.X1;

  g_sTfFingerY0_last = tTouchFingers.Y0;

  g_sTfFingerY1_last = tTouchFingers.Y1;
  
  g_ucFingers_last = g_ucFingers; // 手指数量

  g_ulLastforce = g_ulTF_force;  //力度保存
  
  // Palm状态信息缓存
  g_ucPalms_last = g_ucPalms;    // Palm数量
  


}