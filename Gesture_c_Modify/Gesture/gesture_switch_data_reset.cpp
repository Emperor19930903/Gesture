
#include "force_process.h"
#include "gesture_switch_data_reset.h"
#include "tp_data.h"

//手势切换,需要重置数据，涉及手指顺序变化以及动静变化
extern  TF_PARAMETER g_tTFParaMeter;
extern AcDataGather_T t_AcDataGather;
extern FingerGather_T tTouchFingers;
void gesture_switch_data_reset(void)
{
  /********** 1.手指数量切换 **********/
  //双指切换为单指 有力   将积分值转成适配当前系数，保持力度不变
  if(g_lLastFingersStatus == (FINGER_1 | FINGER_2) && (g_lThisFingersStatus == FINGER_1 || g_lThisFingersStatus == FINGER_2 ) && g_ulTF_force >= g_tTFParaMeter.g_lMotorTrigger)
  {
    t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio;
    //ClearFastPress();//1.35.111防止两指时另一指的触碰干扰导致识别成快按
    g_cFlagLeaveFinger = 1;   // 双指切换单指 标记
    g_sCountLeaveFinger = 0;  // 
  }
  //单指尖切换为双指 有力  将积分值转成适配当前系数，保持力度不变
  if((g_lLastFingersStatus == FINGER_1 || g_lLastFingersStatus == FINGER_2 ) && g_lThisFingersStatus == (FINGER_1 | FINGER_2) && g_ulTF_force >= g_tTFParaMeter.g_lMotorTrigger)
  {
    t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio;
  }

  /********** 2.拖拽状态切换 **********/
  //双指时由拖拽状态切换非拖拽，有力   需将积分值转成适配当前系数，保持力度不变
  if((g_lLastFingersStatus == (FINGER_1 | FINGER_2) && g_lThisFingersStatus == (FINGER_1 | FINGER_2)) && (g_lFlagDragFinger1Last == 1 || g_lFlagDragFinger2Last == 1 ) && (g_lFlagDragFinger1 == 0 && g_lFlagDragFinger2 == 0))
  {
    t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio;
  }
  //1指时手指拖拽状态切换，有力
  if(g_lThisFingersStatus == FINGER_1 && (g_lFlagDragFinger1Last == 1 && g_lFlagDragFinger1 == 0) && g_ulTF_force >= g_tTFParaMeter.g_lMotorRelease)
  {
    t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio;
  }
  //2指时手指拖拽状态切换，有力，允许重新选通道，解决一手palm另一指拖拽停止掉落
  if(g_lThisFingersStatus == FINGER_2 && (g_lFlagDragFinger2Last == 1 && g_lFlagDragFinger2 == 0) && g_ulTF_force >= g_tTFParaMeter.g_lMotorRelease)
  {
    t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio;
  }

  /********** 3.单指->双指->单指 **********/
  //单手1指按2指划
  if(g_lThisFingersStatus == (FINGER_1 | FINGER_2) && g_lTFIsSlideFinger2 == 1 && tTouchFingers.X0 - tTouchFingers.X1 > 0 && tTouchFingers.X0 - tTouchFingers.X1 < 500 && abs(tTouchFingers.Y0 - tTouchFingers.Y1) < 200 && g_tForceTriggerStatus == FORCE_TRIGGERED)
  {
    g_cFlag1FingerPress2FingerSlide = 1;
  }
  //(1)、1指按切换双指 【然后1指抬起 有力，第一指抬起，应该释放】。LG 20220904	LLS
  if((g_lLastFingersStatus == FINGER_1) && g_lThisFingersStatus == (FINGER_1 | FINGER_2) && g_tForceTriggerStatus == FORCE_TRIGGERED)
  {
    gesture_1fingerpress_2fingertouch = 1;
  }
  //(2)、双指切换为2指
  if(g_lLastFingersStatus == (FINGER_1 | FINGER_2) && (g_lThisFingersStatus == FINGER_2) && g_tForceTriggerStatus == FORCE_TRIGGERED && gesture_1fingerpress_2fingertouch == 1)
  {
    gesture_1fingerpress_2fingertouch = 0;
    gesture_1fingerleave_2fingertouch = 1;
    t_AcDataGather.AcAll = 0;
    t_AcDataGather.AcAllTemp = 0;
    g_ulTF_force = 0;
    g_ulLastforce = 0;
    for(int i = 0; i < CHANNEL; i++)
    {
      t_AcDataGather.AcComponent[i] = 0;
      t_AcDataGather.AcComponentBefore[i] = 0;
      t_AcDataGather.AcComponentLast[i] = 0;
    }
  }
  //1指轻滑(无力) 
  if(g_lThisFingersStatus == FINGER_1 && g_ucTFIsSlide == 1 && g_tForceTriggerStatus == FORCE_NOTRIGGER)
  {
    gesture_1fingerslidenoforce = 1;
  }
 //20221203 Conner add，为了解决一指滑动后，又静止，之后触发的问题。 第一指触发不算数。
  if(g_lThisFingersStatus == FINGER_1 && g_ucTFIsSlide == 0 && g_tForceTriggerStatus == FORCE_TRIGGERED)
  {
    gesture_1fingerslidenoforce = 0;
  }	
  // 1指轻滑 切换双指 有力
  if(g_lThisFingersStatus == (FINGER_1 | FINGER_2) && gesture_1fingerslidenoforce == 1 && g_tForceTriggerStatus == FORCE_TRIGGERED)
  {
    gesture_1fingerslidenoforce = 0;
    gesture_2fingerspress = 1;
  }
  if(g_lLastFingersStatus == (FINGER_1 | FINGER_2) && (g_lThisFingersStatus == FINGER_1) && gesture_2fingerspress == 1)
  {
    gesture_1fingertouch2fingerleave = 1;
    t_AcDataGather.AcAll = 0;
    t_AcDataGather.AcAllTemp = 0;
    g_ulTF_force = 0;
    g_ulLastforce = 0;
    for(int i = 0; i < CHANNEL; i++)
    {
      t_AcDataGather.AcComponent[i] = 0;
      t_AcDataGather.AcComponentBefore[i] = 0;
      t_AcDataGather.AcComponentLast[i] = 0;
    }
  }
  //对上面手势标记进行清除
  if(g_lLastFingersStatus == (FINGER_1 | FINGER_2) && g_lThisFingersStatus == FINGER_2 && gesture_1fingerslidenoforce == 1)
  {
    gesture_1fingerslidenoforce = 0;
  }
  if(g_lLastFingersStatus == (FINGER_1 | FINGER_2) && g_lThisFingersStatus == FINGER_2 && gesture_2fingerspress == 1)
  {
    gesture_2fingerspress = 0;
  }
  
}
