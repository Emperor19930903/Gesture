
#include "force_process.h"
#include "ratio_calculate.h"
#include "tp_data.h"

extern  TF_PARAMETER g_tTFParaMeter;
extern FingerGather_T tTouchFingers;
/********** 系数选择 **********/
void ratio_calculate()
{
    if(g_lThisPalmStatus == 0 ) //不存在Palm
    {
        ratio_select();
    }
    else
    {
        ratio_select_fingers();
    }
}



//计算系数
void ratio_select(void)
{
  if(g_lThisFingersStatus == FINGER_1) // 1指存在
  {
    if(g_ucTFIsSlide == 0)  // 1指不滑动
    {
      g_TFRatio = TF_Interpolation_short(tTouchFingers.X0, tTouchFingers.Y0);
    }
  }

  else if(g_lThisFingersStatus == (FINGER_1 | FINGER_2)) //双指存在
  {
    if(g_ucTFIsSlide == 1 && g_lTFIsSlideFinger2 == 0)   //1指滑动2指静止   2指不滑选之前,放止滑动的系数导致当前力度突变，也不可选静止手指的系数，会在切换时力度突变0831
    {
      //g_TFRatio = TF_Interpolation_finger2();
    }
    else if(g_ucTFIsSlide == 0 && g_lTFIsSlideFinger2 == 1)  //1指不滑2指滑动
    {
      //g_TFRatio = TF_Interpolation();
    }
    else if(g_ucTFIsSlide == 1 && g_lTFIsSlideFinger2 == 1)//双指都滑选最小
    {
      if(TF_Interpolation_short(tTouchFingers.X0, tTouchFingers.Y0) - TF_Interpolation_short(tTouchFingers.X1, tTouchFingers.Y1) > 0.0001f)
      {
        g_TFRatio = TF_Interpolation_short(tTouchFingers.X1, tTouchFingers.Y1);
      }
      else
      {
        g_TFRatio = TF_Interpolation_short(tTouchFingers.X0, tTouchFingers.Y0);
      }
    }
    else//双指都不滑 选2指
    {
      g_TFRatio = TF_Interpolation_short(tTouchFingers.X1, tTouchFingers.Y1);	
    }
  }

  else if(g_lThisFingersStatus == FINGER_2) //2指存在
  {
    if(g_lTFIsSlideFinger2 == 0)
    {
      g_TFRatio = TF_Interpolation_short(tTouchFingers.X1, tTouchFingers.Y1);
    }
  }
  else //双指以上 选1指
  {
    g_TFRatio = TF_Interpolation_short(tTouchFingers.X0, tTouchFingers.Y0);
  }
}


//多指计算系数
void ratio_select_fingers(void)
{
  if(tTouchFingers.Finger4 != 0)
  {
    g_TFRatio = TF_Interpolation_short(tTouchFingers.X4, tTouchFingers.Y4);
  }
  else if(tTouchFingers.Finger3 != 0)
  {
    g_TFRatio = TF_Interpolation_short(tTouchFingers.X3, tTouchFingers.Y3);
  }
  else if(tTouchFingers.Finger2 != 0)
  {
    g_TFRatio = TF_Interpolation_short(tTouchFingers.X2, tTouchFingers.Y2);
  }

  else if(tTouchFingers.Finger1 != 0)
  {
    g_TFRatio = TF_Interpolation_short(tTouchFingers.X1, tTouchFingers.Y1);
  }

  else if(tTouchFingers.Finger0 != 0)
  {
    g_TFRatio = TF_Interpolation_short(tTouchFingers.X0, tTouchFingers.Y0);
  }
  else
  {
    g_TFRatio = TF_Interpolation_short(tTouchFingers.X0, tTouchFingers.Y0);
  }
}

//反距离加权插值算法计算系数:传参
float TF_Interpolation_short(short x, short y)
{
  // i j:索引初值   coor：符合条件的系数索引
  int i, j, coor = 0, tmp = 0;

  //当前点的周围的点，左上，左下，右上，右下，系数，加权和，拓展系数1，拓展系数2
  float disleftUp = 0, disleftDown = 0, disrightUp = 0, disrightDown = 0, ratio = 0, weightSum = 0, ratioEx1 = 0, ratioEx2 = 0;
 
 
  /********** 插值开始 **********/
  //左上角
  if (x <= xCoorLeftMax && y <= yCoorUpMin)
  {
    ratio = g_TF_force_ratio[0] - (abs(x - g_TF_Coordinate[0][0]) + abs(y - g_TF_Coordinate[0][1])) * (g_TF_force_ratio[TF_COL_NUM + 1] - g_TF_force_ratio[0]) / (abs(g_TF_Coordinate[TF_COL_NUM + 1][0] - g_TF_Coordinate[0][0]) + abs(g_TF_Coordinate[TF_COL_NUM + 1][1] - g_TF_Coordinate[0][1]));
  }
  //右上角
  else if (x >= xCoorRightMin && y <= yCoorUpMin)
  {
    ratio = g_TF_force_ratio[TF_COL_NUM - 1] - (abs(x - g_TF_Coordinate[TF_COL_NUM - 1][0]) + abs(y - g_TF_Coordinate[TF_COL_NUM - 1][1])) * (g_TF_force_ratio[2 * TF_COL_NUM - 2] - g_TF_force_ratio[TF_COL_NUM - 1]) / (abs(g_TF_Coordinate[2 * TF_COL_NUM - 2][0] - g_TF_Coordinate[TF_COL_NUM - 1][0]) + abs(g_TF_Coordinate[2 * TF_COL_NUM - 2][1] - g_TF_Coordinate[TF_COL_NUM - 1][1]));
  }
  //左下角
  else if (x <= xCoorLeftMax && y >= yCoorDownMax)
  {
    ratio = g_TF_force_ratio[(TF_ROW_NUM - 1) * TF_COL_NUM] - (abs(x - g_TF_Coordinate[(TF_ROW_NUM - 1) * TF_COL_NUM][0]) + abs(y - g_TF_Coordinate[(TF_ROW_NUM - 1) * TF_COL_NUM][1])) * (g_TF_force_ratio[(TF_ROW_NUM - 2) * TF_COL_NUM + 1] - g_TF_force_ratio[(TF_ROW_NUM - 1) * TF_COL_NUM]) / (abs(g_TF_Coordinate[(TF_ROW_NUM - 2) * TF_COL_NUM + 1][0] - g_TF_Coordinate[(TF_ROW_NUM - 1) * TF_COL_NUM][0]) + abs(g_TF_Coordinate[(TF_ROW_NUM - 2) * TF_COL_NUM + 1][1] - g_TF_Coordinate[(TF_ROW_NUM - 1) * TF_COL_NUM][1]));
  }
  //右下角
  else if (x >= xCoorRightMin && y >= yCoorDownMax)
  {
    ratio = g_TF_force_ratio[TF_ROW_NUM * TF_COL_NUM - 1] - (abs(x - g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM - 1][0]) + abs(y - g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM - 1][1])) * (g_TF_force_ratio[(TF_ROW_NUM - 1) * TF_COL_NUM - 2] - g_TF_force_ratio[TF_ROW_NUM * TF_COL_NUM - 1]) / (abs(g_TF_Coordinate[(TF_ROW_NUM - 1) * TF_COL_NUM - 2][0] - g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM - 1][0]) + abs(g_TF_Coordinate[(TF_ROW_NUM - 1) * TF_COL_NUM - 2][1] - g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM - 1][1]));
  }
  // 左边缘超出
  else if (x <= xCoorLeftMax)
  {
    for (j = 0; j <= (TF_ROW_NUM - 1) * TF_COL_NUM; j = j + TF_COL_NUM)
    {
      if (y < g_TF_Coordinate[j][1])
      {
        coor = j;
        break;
      }
      //万一没进上层 if判断条件，所以需要不断更新coor
      coor = j;
    }
    //系数拓展
    ratioEx2 = g_TF_force_ratio[coor] - g_TF_Coordinate[coor][0] * (g_TF_force_ratio[coor + 1] - g_TF_force_ratio[coor]) / (g_TF_Coordinate[coor + 1][0] - g_TF_Coordinate[coor][0]);
    ratioEx1 = g_TF_force_ratio[coor - TF_COL_NUM] - g_TF_Coordinate[coor - TF_COL_NUM][0] * (g_TF_force_ratio[coor - TF_COL_NUM + 1] - g_TF_force_ratio[coor - TF_COL_NUM]) / (g_TF_Coordinate[coor - TF_COL_NUM + 1][0] - g_TF_Coordinate[coor - TF_COL_NUM][0]);
    // 当前点距左上的距离
    disleftUp = x + abs(y - g_TF_Coordinate[coor - TF_COL_NUM ][1]) + 1e-6;
    // 当前点距左下的距离
    disleftDown = x  + abs(y - g_TF_Coordinate[coor][1]) + 1e-6;
    // 当前点距右上的距离
    disrightUp = abs(x - g_TF_Coordinate[coor - TF_COL_NUM][0]) + abs(y - g_TF_Coordinate[coor - TF_COL_NUM][1]) + 1e-6;
    // 当前点距右下的距离
    disrightDown = abs(x - g_TF_Coordinate[coor][0]) + abs(y - g_TF_Coordinate[coor][1]) + 1e-6;

    disleftUp = pow(disleftUp, 2);
    disleftDown = pow(disleftDown, 2);
    disrightUp = pow(disrightUp, 2);
    disrightDown = pow(disrightDown, 2);
    // 总加权和
    weightSum = 1 / disleftUp + 1 / disleftDown + 1 / disrightUp + 1 / disrightDown;
    // 计算系数
    ratio = 1 / disleftUp / weightSum * ratioEx1 +
            1 / disleftDown / weightSum * ratioEx2 +
            1 / disrightUp / weightSum * g_TF_force_ratio[coor - TF_COL_NUM] +
            1 / disrightDown / weightSum * g_TF_force_ratio[coor];
  }
  // 右边缘超出
  else if (x >= xCoorRightMin)
  {
    for (j = TF_COL_NUM - 1; j < TF_ROW_NUM * TF_COL_NUM; j = j + TF_COL_NUM)
    {
      if (y < g_TF_Coordinate[j][1])
      {
        coor = j;
        break;
      }
      //万一没进上层 if判断条件，所以需要不断更新coor
      coor = j;
    }
    //系数拓展
    ratioEx2 = g_TF_force_ratio[coor] - ((xCoorRightMin + xCoorLeftMax) - g_TF_Coordinate[coor][0]) * (g_TF_force_ratio[coor - 1] - g_TF_force_ratio[coor]) / (g_TF_Coordinate[coor][0] - g_TF_Coordinate[coor - 1][0]);
    ratioEx1 = g_TF_force_ratio[coor - TF_COL_NUM] - ((xCoorRightMin + xCoorLeftMax) - g_TF_Coordinate[coor - TF_COL_NUM][0]) * (g_TF_force_ratio[coor - TF_COL_NUM - 1] - g_TF_force_ratio[coor - TF_COL_NUM]) / (g_TF_Coordinate[coor - TF_COL_NUM][0] - g_TF_Coordinate[coor - TF_COL_NUM - 1][0]);

    // 当前点距左上的距离
    disleftUp = abs(x - g_TF_Coordinate[coor - TF_COL_NUM][0]) + abs(y - g_TF_Coordinate[coor - TF_COL_NUM][1]) + 1e-6;
    // 当前点距左下的距离
    disleftDown = abs(x - g_TF_Coordinate[coor][0]) + abs(y - g_TF_Coordinate[coor][1]) + 1e-6;
    // 当前点距右上的距离
    disrightUp = abs(x - (xCoorRightMin + xCoorLeftMax)) + abs(y - g_TF_Coordinate[coor - TF_COL_NUM][1]) + 1e-6;
    // 当前点距右下的距离
    disrightDown = abs(x - (xCoorRightMin + xCoorLeftMax)) + abs(y - g_TF_Coordinate[coor][1]) + 1e-6;
    disleftUp = pow(disleftUp, 2);
    disleftDown = pow(disleftDown, 2);
    disrightUp = pow(disrightUp, 2);
    disrightDown = pow(disrightDown, 2);
    // 总加权和
    weightSum = 1 / disleftUp + 1 / disleftDown + 1 / disrightUp + 1 / disrightDown;
    // 计算系数
    ratio = 1 / disleftUp / weightSum * g_TF_force_ratio[coor - TF_COL_NUM] +
            1 / disleftDown / weightSum * g_TF_force_ratio[coor] +
            1 / disrightUp / weightSum * ratioEx1 +
            1 / disrightDown / weightSum * ratioEx2;
  }
  // 上边缘超出
  else if (y <= yCoorUpMin)
  {
    for (j = 0; j < TF_COL_NUM; ++j)
    {
      if (x < g_TF_Coordinate[j][0])
      {
        coor = j;
        break;
      }
      //万一没进上层 if判断条件，所以需要不断更新coor
      coor = j;
    }
    //系数拓展
    ratioEx2 = g_TF_force_ratio[coor] - g_TF_Coordinate[coor][1] * (g_TF_force_ratio[coor + TF_COL_NUM] - g_TF_force_ratio[coor]) / (g_TF_Coordinate[coor + TF_COL_NUM][1] - g_TF_Coordinate[coor][1]);
    ratioEx1 = g_TF_force_ratio[coor - 1] - g_TF_Coordinate[coor - 1][1] * (g_TF_force_ratio[coor + TF_COL_NUM - 1] - g_TF_force_ratio[coor - 1]) / (g_TF_Coordinate[coor + TF_COL_NUM - 1][1] - g_TF_Coordinate[coor - 1][1]);
    // 当前点距左下的距离
    disleftDown = abs(x - g_TF_Coordinate[coor - 1][0]) + abs(y - g_TF_Coordinate[coor - 1][1]) + 1e-6;
    // 当前点距右下的距离
    disrightDown = abs(x - g_TF_Coordinate[coor][0]) + abs(y - g_TF_Coordinate[coor][1]) + 1e-6;
    // 当前点距左上的距离
    disleftUp = abs(x - g_TF_Coordinate[coor - 1][0]) + y + 1e-6;
    // 当前点距右上的距离
    disrightUp = abs(x - g_TF_Coordinate[coor][0]) + y  + 1e-6;
    disleftUp = pow(disleftUp, 2);
    disleftDown = pow(disleftDown, 2);
    disrightUp = pow(disrightUp, 2);
    disrightDown = pow(disrightDown, 2);
    // 总加权和
    weightSum = 1 / disleftUp + 1 / disleftDown + 1 / disrightUp + 1 / disrightDown;
    // 计算系数
    ratio = 1 / disleftDown / weightSum * g_TF_force_ratio[coor - 1] +
            1 / disrightDown / weightSum * g_TF_force_ratio[coor] +
            1 / disleftUp / weightSum * ratioEx1 +
            1 / disrightUp / weightSum * ratioEx2;
  }

  // 下边缘超出
  else if (y >= yCoorDownMax)
  {
    for (j = (TF_ROW_NUM - 1) * TF_COL_NUM; j < TF_ROW_NUM * TF_COL_NUM; ++j)
    {
      if (x < g_TF_Coordinate[j][0])
      {
        coor = j;
        break;
      }
      //万一没进上层 if判断条件，所以需要不断更新coor
      coor = j;
    }
    //系数拓展
    ratioEx2 = g_TF_force_ratio[coor] - ((yCoorUpMin + yCoorDownMax) - g_TF_Coordinate[coor][1]) * (g_TF_force_ratio[coor - TF_COL_NUM] - g_TF_force_ratio[coor]) / (g_TF_Coordinate[coor ][1] - g_TF_Coordinate[coor - TF_COL_NUM][1]);
    ratioEx1 = g_TF_force_ratio[coor - 1] - ((yCoorUpMin + yCoorDownMax) - g_TF_Coordinate[coor - 1][1]) * (g_TF_force_ratio[coor - TF_COL_NUM - 1] - g_TF_force_ratio[coor - 1]) / (g_TF_Coordinate[coor - 1][1] - g_TF_Coordinate[coor - TF_COL_NUM - 1][1]);
    // 当前点距左下的距离
    disleftDown = abs(x - g_TF_Coordinate[coor - 1][0]) + abs(y - (yCoorUpMin + yCoorDownMax)) + 1e-6;
    // 当前点距右下的距离
    disrightDown = abs(x - g_TF_Coordinate[coor][0]) + abs(y - (yCoorUpMin + yCoorDownMax)) + 1e-6;
    // 当前点距左上的距离
    disleftUp = abs(x - g_TF_Coordinate[coor - 1][0]) + abs(y - g_TF_Coordinate[coor - 1][1]) + 1e-6;
    // 当前点距右上的距离
    disrightUp = abs(x - g_TF_Coordinate[coor][0]) + abs(y - g_TF_Coordinate[coor][1]) + 1e-6;
    disleftUp = pow(disleftUp, 2);
    disleftDown = pow(disleftDown, 2);
    disrightUp = pow(disrightUp, 2);
    disrightDown = pow(disrightDown, 2);
    // 总加权和
    weightSum = 1 / disleftUp + 1 / disleftDown + 1 / disrightUp + 1 / disrightDown;
    // 计算系数
    ratio = 1 / disleftUp / weightSum * g_TF_force_ratio[coor - 1] +
            1 / disrightUp / weightSum * g_TF_force_ratio[coor] +
            1 / disleftDown / weightSum * ratioEx1 +
            1 / disrightDown / weightSum * ratioEx2;
  }
  //未超出边缘的点,用四点加权
  else
  {
    // 寻找当前点右下角的点,用两个循环
    for (i = 1; i <= (TF_COL_NUM - 1); i++)
    {
      if (x < g_TF_Coordinate[i][0])
      {
        tmp = i;
        break;
      }
      tmp = i;
    }
    for (j = tmp + TF_COL_NUM; j <= tmp + (TF_ROW_NUM - 1) * TF_COL_NUM; j = j + TF_COL_NUM)
    {
      if (y < g_TF_Coordinate[j][1])  //2022 0608修改
      {
        coor = j;
        break;
      }
      //万一没进上层 if判断条件，所以需要不断更新coor
      coor = j;
    }
    // 当前点距左上的距离
    disleftUp = abs(x - g_TF_Coordinate[coor - TF_COL_NUM - 1][0]) + abs(y - g_TF_Coordinate[coor - TF_COL_NUM - 1][1]) + 1e-6;
    // 当前点距左下的距离
    disleftDown = abs(x - g_TF_Coordinate[coor - 1][0]) + abs(y - g_TF_Coordinate[coor - 1][1]) + 1e-6;
    // 当前点距右上的距离
    disrightUp = abs(x - g_TF_Coordinate[coor - TF_COL_NUM][0]) + abs(y - g_TF_Coordinate[coor - TF_COL_NUM][1]) + 1e-6;
    // 当前点距右下的距离
    disrightDown = abs(x - g_TF_Coordinate[coor][0]) + abs(y - g_TF_Coordinate[coor][1]) + 1e-6;
    // 总加权和
    disleftUp = pow(disleftUp, 2);
    disleftDown = pow(disleftDown, 2);
    disrightUp = pow(disrightUp, 2);
    disrightDown = pow(disrightDown, 2);
    weightSum = 1 / disleftUp + 1 / disleftDown + 1 / disrightUp + 1 / disrightDown;

    ratio = 1 / disleftUp / weightSum * g_TF_force_ratio[coor - TF_COL_NUM - 1] +
            1 / disleftDown / weightSum * g_TF_force_ratio[coor - 1] +
            1 / disrightUp / weightSum * g_TF_force_ratio[coor - TF_COL_NUM] +
            1 / disrightDown / weightSum * g_TF_force_ratio[coor];

  }
  return ratio;
}





