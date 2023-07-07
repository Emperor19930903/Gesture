
#include "force_process.h"
#include "gesture_drag_state.h"
#include "tp_data.h"
extern FingerGather_T tTouchFingers;
/**********  判断拖拽  **********/
void gesture_drag_state(void)
{
  // 1指拖拽判断
  if(g_lThisFingersStatus == FINGER_1)
  {
    JudgeSingleDrag();
  }
  // 双指拖拽判断
  else if(g_lThisFingersStatus == (FINGER_1 | FINGER_2)) 
  {
    JudgeDoubleDrag();
  }
  // 2指拖拽判断
  else if(g_lThisFingersStatus == FINGER_2)
  {
    JudgeSingleDrag();
  }
  else//两指以上
  {}
}



//判断单指拖拽
void JudgeSingleDrag(void)
{
  //第一指拖拽
  if(tTouchFingers.Finger0 == 1 && tTouchFingers.Finger1 == 0)
  {
    if(g_ucTFIsSlide == 1 && g_tForceTriggerStatus == FORCE_TRIGGERED)  // 1指滑动且力度处于触发状态
    {
      g_lFlagDragFinger1 = 1;
    }
    else
    {
      g_lFlagDragFinger1 = 0;
    }
  }
  else
  {
    g_lFlagDragFinger1 = 0;
  }

  //第二指拖拽
  if(tTouchFingers.Finger0 == 0 && tTouchFingers.Finger1 == 1)
  {
    if(g_lTFIsSlideFinger2 == 1 && g_tForceTriggerStatus == FORCE_TRIGGERED)
    {
      g_lFlagDragFinger2 = 1;
    }
    else
    {
      g_lFlagDragFinger2 = 0;
    }
  }
  else
  {
    g_lFlagDragFinger2 = 0;
  }
}


//判断双指拖拽

void JudgeDoubleDrag(void)
{
  //第一指拖拽
  if(tTouchFingers.Finger0 == 1 && tTouchFingers.Finger1 == 1)
  {
    if(g_tForceTriggerStatus == FORCE_TRIGGERED)//双指马达触发阈值
    {
      // 判断1指是否滑动
      if(g_ucTFIsSlide == 1)  
      {
        g_lFlagDragFinger1 = 1;
      }
      else
      {
        g_lFlagDragFinger1 = 0;
      }
      //判断2指是否滑动
      if(g_lTFIsSlideFinger2 == 1)  
      {
        g_lFlagDragFinger2 = 1;
      }
      else
      {
        g_lFlagDragFinger2 = 0;
      }
    }
    else  // 若双指没触发马达
    {
      g_lFlagDragFinger1 = 0;
      g_lFlagDragFinger2 = 0;
    }
  }
}






