
#include "force_process.h"
#include "gesture_slide_errortrigger_handle.h"
#include "tp_data.h"
extern AcDataGather_T t_AcDataGather;
extern TF_PARAMETER g_tTFParaMeter;
extern FingerGather_T tTouchFingers;


//滑动误触的处理
void gesture_slide_errortrigger_handle(void)//1.34.30 根据赵艳红思想实现抑制滑动误触。主要是在传感器上方的积分
{
    short i = 0;
    if (g_ucTFIsSlide == 1)
    {
        ReduceSlideTrigger_finger1();//保持之前的方法
    }
    else//janna增加的算法，1指非滑动的特殊处理     
    {
        for (i = 0; i < g_tTFParaMeter.g_lSlideTimeThreshold; i++)
        {
            g_sTFFingerLastX0[i] = g_sTFFingerLastX0[i + 1];
            g_sTFFingerLastY0[i] = g_sTFFingerLastY0[i + 1];
        }
        g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.X0;
        g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.Y0;
    }

    //conner   在janna基础上，conner考虑解决2个手指的滑动误触问题；swipeFlagfinger2是conner新添加的变量
    if (g_lTFIsSlideFinger2 == 0)
    {
        for (i = 0; i < g_tTFParaMeter.g_lSlideTimeThreshold; i++)
        {
            g_sTFFingerLastX1[i] = g_sTFFingerLastX1[i + 1];
            g_sTFFingerLastY1[i] = g_sTFFingerLastY1[i + 1];
        }
        g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.X1;
        g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold] = tTouchFingers.Y1;
    }
    // 20230605 Kai 解决数组中0也作为滑动判断条件
    for (i = g_tTFParaMeter.g_lSlideTimeThreshold-1; i >=0 ; i--)
    {
        if (g_sTFFingerLastX0[i] == 0)
            g_sTFFingerLastX0[i] = g_sTFFingerLastX0[i + 1];
        if (g_sTFFingerLastY0[i] == 0)
            g_sTFFingerLastY0[i] = g_sTFFingerLastY0[i + 1];
        if (g_sTFFingerLastX1[i] == 0)
            g_sTFFingerLastX1[i] = g_sTFFingerLastX1[i + 1];
        if (g_sTFFingerLastY1[i] == 0)
            g_sTFFingerLastY1[i] = g_sTFFingerLastY1[i + 1];
    }

    if (swipeFlag == 0) // 1指微滑动，滑动状态满足的条件
    {
        if (g_tForceTriggerStatus == FORCE_NOTRIGGER) //只第一指存在且未触发
        {
            int xCoorDiffSum = abs(g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold] - g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold - 1]); //最新帧x轴差分值
            int yCoorDiffSum = abs(g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold] - g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold - 1]); //最新帧y轴差分值
            for (i = 1; i < 6; i++)
            {
                xCoorDiffSum = xCoorDiffSum + abs(g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold - i] - g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold - i - 1]);
                yCoorDiffSum = yCoorDiffSum + abs(g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold - i] - g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold - i - 1]);
                if (xCoorDiffSum > g_tTFParaMeter.SwipeThresholdX || yCoorDiffSum > g_tTFParaMeter.SwipeThresholdY || (xCoorDiffSum + yCoorDiffSum) > (g_tTFParaMeter.SwipeThresholdX + g_tTFParaMeter.SwipeThresholdY - 2)) // 横纵轴范围的两百分之一为距离阈值 一般
                //if (xCoorDiffSum > 15 || yCoorDiffSum > 15 || (xCoorDiffSum + yCoorDiffSum) > (g_tTFParaMeter.SwipeThresholdX + g_tTFParaMeter.SwipeThresholdY - 2))
                {
                    swipeFlag = 1;
                    break;
                }
            }
        }
    }


    if (swipeFlagfinger2 == 0) // 2指 微滑动状态满足的条件 conner添加
    {
        if (g_tForceTriggerStatus == FORCE_NOTRIGGER) //只第一指存在且未触发
        {
            int xCoorDiffSum2 = abs(g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold] - g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold - 1]); //最新帧x轴差分值
            int yCoorDiffSum2 = abs(g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold] - g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold - 1]); //最新帧y轴差分值
            for (i = 1; i < 6; i++)
            {
                xCoorDiffSum2 = xCoorDiffSum2 + abs(g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold - i] - g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold - i - 1]);
                yCoorDiffSum2 = yCoorDiffSum2 + abs(g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold - i] - g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold - i - 1]);
                if (xCoorDiffSum2 > g_tTFParaMeter.SwipeThresholdX || yCoorDiffSum2 > g_tTFParaMeter.SwipeThresholdY || (xCoorDiffSum2 + yCoorDiffSum2) > (g_tTFParaMeter.SwipeThresholdX + g_tTFParaMeter.SwipeThresholdY - 2)) // 横纵轴范围的两百分之一为距离阈值 一般
                {
                    swipeFlagfinger2 = 1;
                    break;
                }
            }
        }
    }

    //conner的处理，没触发之前，任何一个手指是微滑动状态都会触发滑动保持状态
    if ((g_tForceTriggerStatus == FORCE_NOTRIGGER) && (swipeFlag == 1 || swipeFlagfinger2 == 1))
    {
        //if (g_ulTF_force > g_tTFParaMeter.g_lMotorTrigger / 2) //将力度控制在马达触发力度的一半
        //{
        //    g_ulTF_force = g_tTFParaMeter.g_lMotorTrigger / 2;
        //}
        //t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio; //积分保持不变			 
        t_AcDataGather.AcAll = 0;
        g_ulTF_force = 0;
    }


    if (swipeFlag == 1)  // 滑动已满足后，janna添加的模块：手指1的状态清零
    {
        for (i = 0; i < 4; i++)
        {
            if (g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold - i] != g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold] //X方向最近5个坐标是否不等
                || g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold - i] != g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold] //Y方向最近5个坐标是否不等
                )
            {
                break;
            }
        }

        if ((g_tForceTriggerStatus == FORCE_TRIGGERED) || i == 4) // 滑动状态清除条件  （上一次g_ucTFIsSlide = 1且当前g_ucTFIsSlide = 0） 或者 马达已触发g_tForceTriggerStatus == FORCE_TRIGGERED 或者 touch有变无
        {
            swipeFlag = 0;
        }
        else
        {
            if (g_ulTF_force > g_tTFParaMeter.g_lMotorTrigger / 2) //将力度控制在马达触发力度的一半
            {
                g_ulTF_force = g_tTFParaMeter.g_lMotorTrigger / 2;
            }
            t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio; //积分保持不变
        }
    }




    if (swipeFlagfinger2 == 1)  // 滑动已满足后，conner补充添加的模块：手指2的状态清零
    {
        for (i = 0; i < 4; i++)
        {
            if (g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold - i] != g_sTFFingerLastX1[g_tTFParaMeter.g_lSlideTimeThreshold] //X方向最近5个坐标是否不等
                || g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold - i] != g_sTFFingerLastY1[g_tTFParaMeter.g_lSlideTimeThreshold] //Y方向最近5个坐标是否不等
                )
            {
                break;
            }
        }

        if ((g_tForceTriggerStatus == FORCE_TRIGGERED) || i == 4) // 滑动状态清除条件  （上一次g_ucTFIsSlide = 1且当前g_ucTFIsSlide = 0） 或者 马达已触发g_tForceTriggerStatus == FORCE_TRIGGERED 或者 touch有变无
        {
            swipeFlagfinger2 = 0;
        }
        else
        {
            if (g_ulTF_force > g_tTFParaMeter.g_lMotorTrigger / 2) //将力度控制在马达触发力度的一半
            {
                g_ulTF_force = g_tTFParaMeter.g_lMotorTrigger / 2;
            }
            t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio; //积分保持不变
        }
    }

    //两指同时轻滑防误触

    //if (g_ucTFIsSlide == 1 || g_lTFIsSlideFinger2 == 1 && g_tForceTriggerStatus == FORCE_NOTRIGGER)
    if (g_ucTFIsSlide == 1 && g_lTFIsSlideFinger2 == 1 && g_tForceTriggerStatus == FORCE_NOTRIGGER)
    {
        t_AcDataGather.AcAll = 0;
        t_AcDataGather.AcAllTemp = 0;
        g_ulTF_force = 0;
        g_ulLastforce = 0;
        for (i = 0; i < CHANNEL; i++)
        {
            t_AcDataGather.AcComponent[i] = 0;
            t_AcDataGather.AcComponentBefore[i] = 0;
            t_AcDataGather.AcComponentLast[i] = 0;
        }
    }
    //两指同时轻滑防误触：解决缩放触发和手掌按压触发，根源是马达没有触发之前，不能通过滑动触发  左荣虎
}



//之前的误触识别方法
void ReduceSlideTrigger_finger1(void)//1.235 还未判断出滑动时，就有误触，艳红
{
    char i = 0;
    float ac_temp_ch[CHANNEL] = { 0 };
    float ac_temp_ch_value_greaterthan0[CHANNEL];
    char channelnum_greaterthan0 = 0;
    char channel_down = 0;//通道有向下的

    if (g_lThisFingersStatus != FINGER_1)
    {
        return;
    }


    if (t_AcDataGather.AcComponent[0] > t_AcDataGather.AcComponentLast[0] && t_AcDataGather.AcComponentBefore[0] > 0
        && t_AcDataGather.AcComponent[0] > g_tTFParaMeter.g_lACThreshold
        && g_tForceTriggerStatus == FORCE_NOTRIGGER)// 未触发
    {
        for (i = 0; i < CHANNEL; i++)
        {
            if (t_AcDataGather.AcComponentOrig[i] > g_tTFParaMeter.g_lACThreshold * 0.8f)
            {
                ac_temp_ch[i] = t_AcDataGather.AcComponentOrigBefore[i] * 1.0f / t_AcDataGather.AcComponentOrig[i];

                //后面MAX-MIN不用值是0的通道,只用这里的数据
                ac_temp_ch_value_greaterthan0[channelnum_greaterthan0] = ac_temp_ch[i];
                channelnum_greaterthan0++;
            }
        }

        for (i = 0; i < CHANNEL; i++)
        {
            if (t_AcDataGather.AcComponentOrig[i] < t_AcDataGather.AcComponentOrigLast[i])
            {
                channel_down = 1;
            }

        }

        if (FindMax_Float(ac_temp_ch_value_greaterthan0, channelnum_greaterthan0) - FindMin_Float(ac_temp_ch_value_greaterthan0, channelnum_greaterthan0) > 0.1f && channel_down == 1)

        {

            t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio; //保持不变

        }
        else if (g_lThisFingersStatus == FINGER_1)//只第一指存在	0723 LLS TJJ 单指滑动误触从传感器滑动		14寸LG一体式
        {
            int corDiffCount = 0; // 相邻坐标差计数
            for (i = 0; i < 6; i++)
            {
                if (abs(g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold - i] - g_sTFFingerLastX0[g_tTFParaMeter.g_lSlideTimeThreshold - i - 1]) +
                    abs(g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold - i] - g_sTFFingerLastY0[g_tTFParaMeter.g_lSlideTimeThreshold - i - 1]) >= 2
                    )
                {
                    corDiffCount++;
                }
            }
            if (corDiffCount >= 5) //最新6个点坐标在变化，则积分保持不变
            {
                t_AcDataGather.AcAll = g_ulTF_force * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio; //积分保持不变
            }
        }
    }
}

/* ?计算数组最大值?浮点型 */
float FindMax_Float(float array[], int n)
{
    int i;
    float max = array[0];
    for (i = 0; i < n; i++)
    {
        if (array[i] > max)
            max = array[i];
    }
    return max;
}

/* ?计算数组最小值?浮点型 */
float FindMin_Float(float array[], int n)
{
    int i;
    float min = array[0];
    for (i = 0; i < n; i++)
    {
        if (array[i] < min)
            min = array[i];
    }
    return min;
}