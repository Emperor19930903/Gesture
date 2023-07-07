#include "force_process.h"
#include "gesture_slide_state.h"
#include "tp_data.h"

extern  TF_PARAMETER g_tTFParaMeter;
extern FingerGather_T tTouchFingers;
/**********  判断滑动  **********/
void gesture_slide_state(void)
{

  //判断滑动以及静止

  JudgeFinger1Slide();

  JudgeFinger1Static();

  JudgeFinger2Slide();

  JudgeFinger2Static();

}




// 判断1指是否滑动
void JudgeFinger1Slide(void)
{
  uint8_t i = 0;
  /********** 1指滑动 **********/
  if(tTouchFingers.Finger0 != 0)
  {
    //记录刚接触后的位置
    if(g_lFirstCoorFlag == 0)
    {
      g_lFirstCoorFlag = 1;     //初始位置标记1
      g_sXOrg = tTouchFingers.X0; //
      g_sYOrg = tTouchFingers.Y0;
      for(i = 0; i <= g_tTFParaMeter.g_lSlideTimeThreshold; i++) //初始位置(为什么在这个地方存储坐标？目的是为了判断滑动后静止做准备)
      {
        g_sTFFingerLastX0[i] = g_sXOrg;
        g_sTFFingerLastY0[i] = g_sYOrg;
      }
    }

    //判断是否滑动
    if(g_ucTFIsSlide == 0)
    {
      // 判断当前坐标与初始坐标(刚接触坐标)的距离是否大于滑动阈值
      if( abs(tTouchFingers.X0 - g_sXOrg) > g_tTFParaMeter.g_lSliderangeThreshold || abs(tTouchFingers.Y0 - g_sYOrg) > g_tTFParaMeter.g_lSliderangeThreshold )   
      {
        g_ucTFIsSlide = 1;
      }
    }
  }
  else
  {
    g_ucTFIsSlide = 0;
    swipeFlag = 0;
    g_lFirstCoorFlag = 0;
  }
}

/* 计算数组最大值 */
short FindMax(short array[], int n)
{
  int i;
  short max = array[0];
  for(i = 0; i < n; i++)
  {
    if(array[i] > max)
      max = array[i];
  }
  return max;
}
/* ?计算数组最小值? */
short FindMin(short array[], int n)
{
  int i;
  short min = array[0];
  for(i = 0; i < n; i++)
  {
    if(array[i] < min)
      min = array[i];
  }
  return min;
}

//判断1指静止		
void JudgeFinger1Static(void)
{
  int i;
  // 判断1指是否静止
  if(g_ucTFIsSlide == 1)
  {
    g_sXSumSlide = 0;   //滑动距离
    g_sYSumSlide = 0;
    for(i = 0; i < g_tTFParaMeter.g_lSlideTimeThreshold; i++)
    {
      g_sTFFingerLastX0[i] = g_sTFFingerLastX0[i + 1];
      g_sTFFingerLastY0[i] = g_sTFFingerLastY0[i + 1];
    }

    g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.X0;  //保存当前的坐标值
    g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.Y0;

    // 静止滑动阈值，当手指静止时，
    g_sXSumSlide = FindMax(g_sTFFingerLastX0, g_tTFParaMeter.g_lSlideTimeThreshold + 1) - FindMin(g_sTFFingerLastX0, g_tTFParaMeter.g_lSlideTimeThreshold + 1);
   g_sYSumSlide = FindMax(g_sTFFingerLastY0, g_tTFParaMeter.g_lSlideTimeThreshold + 1) - FindMin(g_sTFFingerLastY0, g_tTFParaMeter.g_lSlideTimeThreshold + 1);

    //若滑动距离小于静止偏移阈值8 则判定为静止
    if(g_sXSumSlide < g_tTFParaMeter.g_lSumSlideThreshold && g_sYSumSlide < g_tTFParaMeter.g_lSumSlideThreshold)
    {
      g_ucTFIsSlide = 0;
      swipeFlag = 0;
      g_sXOrg = tTouchFingers.X0;  //这里保存当前坐标为起点坐标的目的是 保证连续滑动手势进行（滑动->静止->滑动->静止 此过程手指不抬离，若手指抬离，则按压初始变量全部归位）
      g_sYOrg = tTouchFingers.Y0;
    }
    else // 滑动距离大于静止偏移阈值8情况也有可能静止 此时判断当前点与最近5个点坐标是否相等
    {
      for(i = 0; i < 6; i++)
      {
        //X方向最近5个坐标是否不等 Y方向最近5个坐标是否不等
        if(g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold - i] != g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold] 
            || g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold - i] != g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold]
          )
        {
          break;
        }
      }
      if(i == 6)
      {
        g_ucTFIsSlide = 0;
        swipeFlag = 0;
        g_sXOrg = tTouchFingers.X0;
        g_sYOrg = tTouchFingers.Y0;
      }
    }
  }
}

//判断第二指滑动
void JudgeFinger2Slide(void)
{
  uint8_t i = 0;
  //第二指滑动
  if(tTouchFingers.Finger1 != 0)
  {
    //记录刚接触后的位置
    if(g_lFirstCoorFlagFinger2 == 0)
    {
      g_lFirstCoorFlagFinger2 = 1;
      g_sXOrgFinger2 = tTouchFingers.X1;
      g_sYOrgFinger2 = tTouchFingers.Y1;
      for(i = 0; i <= g_tTFParaMeter.g_lSlideTimeThreshold; i++)
      {
        g_sTFFingerLastX1[i] = g_sXOrgFinger2;
        g_sTFFingerLastY1[i] = g_sYOrgFinger2;
      }
    }
    //判断是否滑动
    if(g_lTFIsSlideFinger2 == 0)
    {
      if( abs(tTouchFingers.X1 - g_sXOrgFinger2) > g_tTFParaMeter.g_lSliderangeThreshold || abs(tTouchFingers.Y1 - g_sYOrgFinger2) > g_tTFParaMeter.g_lSliderangeThreshold )
      {
        g_lTFIsSlideFinger2 = 1;
      }
    }
  }
  else
  {
	swipeFlagfinger2 = 0;
    g_lTFIsSlideFinger2 = 0;
    g_lFirstCoorFlagFinger2 = 0;
  }
}

void JudgeFinger2Static(void)
{
  int i;
  if(g_lTFIsSlideFinger2 == 1)
  {
    g_sXSumSlideFinger2 = 0;
    g_sYSumSlideFinger2 = 0;
    for(i = 0; i < g_tTFParaMeter.g_lSlideTimeThreshold; i++)
    {
      g_sTFFingerLastX1[i] = g_sTFFingerLastX1[i + 1];
      g_sTFFingerLastY1[i] = g_sTFFingerLastY1[i + 1];
    }

    g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.X1;
    g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.Y1;

   g_sXSumSlideFinger2 = FindMax(g_sTFFingerLastX1, g_tTFParaMeter.g_lSlideTimeThreshold + 1) - FindMin(g_sTFFingerLastX1, g_tTFParaMeter.g_lSlideTimeThreshold + 1);
    g_sYSumSlideFinger2 = FindMax(g_sTFFingerLastY1, g_tTFParaMeter.g_lSlideTimeThreshold + 1) - FindMin(g_sTFFingerLastY1, g_tTFParaMeter.g_lSlideTimeThreshold + 1);

    if(g_sXSumSlideFinger2 < g_tTFParaMeter.g_lSumSlideThreshold && g_sYSumSlideFinger2 < g_tTFParaMeter.g_lSumSlideThreshold)
    {
	  swipeFlagfinger2 = 0;
      g_lTFIsSlideFinger2 = 0;
      g_sXOrgFinger2 = tTouchFingers.X1;
      g_sYOrgFinger2 = tTouchFingers.Y1;
    }
  }
}

