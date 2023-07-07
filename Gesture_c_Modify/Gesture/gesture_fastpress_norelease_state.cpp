#include "force_process.h"
#include "gesture_fastpress_norelease_state.h"

extern AcDataGather_T t_AcDataGather;
extern  TF_PARAMETER g_tTFParaMeter;

//快按不释放的处理
void gesture_fastpress_norelease_state(void)
{
    //  float ac_temp_last = t_AcDataGather.AcComponentLast[0];
    //  float ac_temp = t_AcDataGather.AcComponent[0];

    // 20221126 conner add smooth by 3 points
    float ac_temp_last = ac_temp_last_mean3;
    float ac_temp = (t_AcDataGather.AcComponent[g_lTFCh] + t_AcDataGather.AcComponentLast[g_lTFCh] + t_AcDataGather.AcComponentBefore[g_lTFCh]) / 3.0f;
    //20221126 conner 
    motorTriggeredcount++;

    if (motorTriggeredcount >= 3000000) //20221126 conner 
    {
        motorTriggeredcount = 0;
    }

    if (motorTriggeredcount < 20 && g_tForceTriggerStatus == FORCE_NOTRIGGER && ac_temp > g_tTFParaMeter.g_lACThreshold) //20221126 conner 
    {
        t_AcDataGather.AcAll = t_AcDataGather.AcAll * 1.1f;
    }

    /********** 无拖拽且无滑动状态 **********/
    if ((g_lFlagDragFinger1 == 0 && g_lFlagDragFinger2 == 0) && g_ucTFIsSlide == 0)
    {
        if (g_cFlag1FingerPress2FingerSlide == 0)//单手一指按一指划
        {
            if (g_cFlagLeaveFinger == 0)
            {
                if (JudgeFastPress(ac_temp_last, ac_temp) == 1) //判断快按
                {
                    g_fFastPressAcAll = t_AcDataGather.AcAll;
                    g_sFastPresCount += 1;
                    t_AcDataGather.AcAll = 0;	    //这1帧被touch压住，不显示
                }
                else
                {
                    if (g_sFastPresCount == 2)//这一帧是touch压帧后显示的第2帧，应该小于50，让马达容易按动
                    {
                        {
                            t_AcDataGather.AcAll = (g_tTFParaMeter.g_lMotorTrigger * g_tTFParaMeter.FastPressTriggerForceDecay) * (g_tTFParaMeter.g_lForceDivider * 0.0001f) / g_TFRatio; //力度值=40
                            g_sFastPresCount = 0;
                        }
                    }
                    if (g_sFastPresCount == 1)//这一帧是touch压帧后显示的第1帧，小于20，马达震动
                    {
                        g_sFastPresCount += 1;
                    }
                }
            }

            else
            {
                ClearFastPress();
            }
        }
        else
        {
            ClearFastPress();
        }

    }
    else
    {
        ClearFastPress();
    }

    /********** 连续快按处理 **********/
    int n1 = t_AcDataGather.AcComponentBefore[0];
    int n2 = t_AcDataGather.AcComponentLast[0];
    int n3 = t_AcDataGather.AcComponent[0];
    if (n1 < g_tTFParaMeter.g_lACThresholdRelease && n2 < g_tTFParaMeter.g_lACThresholdRelease && n3 < g_tTFParaMeter.g_lACThresholdRelease && n1 - n2 > 0 && n3 - n2 > 0 && g_sFastPresCount == 0)
    {
        t_AcDataGather.AcAll = t_AcDataGather.AcAll + 30 * (n3 - n2);//5改为20		LLS 0717 LG 20改为30
    }
    ac_temp_last_mean3 = ac_temp;  //当前帧，赋给上一帧
}






//清除快按数据
void ClearFastPress(void)
{
    g_lLastSign = 0;
    g_lPeakCount = 0;
    g_lMinPeakFlag = 0;
    g_lMaxPeakFlag = 0;
    g_lMaxPeakValue = 0;
    g_lMaxPeakCount = 0;
    g_lMinPeakValue = 0;
}

//判断快按

int JudgeFastPress(int n1, int n2)
{
    short i = 0;

    float sum_multiply = 0, sumsqrt_peak = 0, sumsqrt_valley = 0;

    FindPeak(n1, n2);

    //20221024 Janna增加
    if (g_tForceTriggerStatus == FORCE_TRIGGERED && motorTriggered < 50)
    {
        motorTriggered++; //马达触发计数
    }
    else if (g_tForceTriggerStatus == FORCE_NOTRIGGER)
    {
        motorTriggered = 0;
    }


    if (g_lMinPeakFlag >= 3 && g_lMinPeakFlag <= 13)
    {
        if (n2 > ((1 - g_tTFParaMeter.NegativeWaveValleyWeight) * g_lMaxPeakValue + g_tTFParaMeter.NegativeWaveValleyWeight * g_lMinPeakValue) && motorTriggered > 0 && motorTriggered < 50)
        {
            g_lMinPeakFlag = 0;
            return 1;
        }
        else
        {
            g_lMinPeakFlag = g_lMinPeakFlag + 1;
        }
        if (g_lMinPeakFlag == 14)
        {
            g_lMinPeakFlag = 0;
        }
    }

    if (g_lMinPeakFlag == 2 || g_lMinPeakFlag == 1)
    {
        if ((g_lPeakCount - g_lMaxPeakCount) < g_tTFParaMeter.g_lCountthrFastpress && g_lMaxPeakValue > g_tTFParaMeter.g_lFastpressMaxPeakThr && g_tForceTriggerStatus == FORCE_TRIGGERED)
        {
            g_lMinPeakFlag = 3;
            g_lMinPeakValue = n1; //记录波谷值
        }
        if (g_lMinPeakFlag == 2)
        {
            g_lMinPeakFlag = 0;
        }
    }

    //20221024 Janna增加
    if (g_lMinPeakFlag == 1 || g_lMinPeakFlag == 3) //  //20221024 Janna修改
    {
        if (g_lMinPeakFlag == 1)
        {
            g_lMinPeakFlag = 0;
        }
        if (g_lPeakCount - g_lMaxPeakCount < g_tTFParaMeter.g_lCountthrFastpress && g_ulTF_force >= g_tTFParaMeter.g_lMotorRelease && g_lMaxPeakValue > g_tTFParaMeter.g_lFastpressMaxPeakThr) //g_ulTF_force >=g_tTFParaMeter.g_lMotorRelease避免小力度时误判
        {
            if (g_lMinPeakFlag == 3)
            {
                g_lMinPeakFlag = 0;
            }
            //V2.05加入艳红新条件，防止长按误识别为双击
            for (i = 0; i < CHANNEL; i++)
            {
                sum_multiply += g_ValueOrgPeak[i] * g_ValueOrgValley[i];
                sumsqrt_peak += g_ValueOrgPeak[i] * g_ValueOrgPeak[i];
                sumsqrt_valley += g_ValueOrgValley[i] * g_ValueOrgValley[i];
            }
            sumsqrt_peak = sqrt(sumsqrt_peak);
            sumsqrt_valley = sqrt(sumsqrt_valley);
            return 1;
        }

        else
        {
            return 0;
        }
    }

    else
    {
        return 0;
    }
}


// 判断积分值大于0或者小于0
int JudgeSign(int n)
{
    if (n >= 0)return 1;
    else return 0;
}


//找波峰波谷
int FindPeak(int n1, int n2)
{
    int diff;
    float ratio, r;
    short i = 0;

    g_lPeakCount++;

    if (g_lPeakCount >= 3000000) g_lPeakCount = 2;

    if (g_lPeakCount == 1)
    {
        diff = 0;
    }
    else
    {
        diff = JudgeSign(n2 - n1) - g_lLastSign;
    }

    if (diff < 0 && n1 > g_tTFParaMeter.g_lFastpressAmplitudeThr) //20221024 Janna修改
    {
        g_lMinPeakFlag = 0;
        g_lMaxPeakFlag = 1;
        g_lMaxPeakValue = n1; 	//20221024 Janna修改

        //存储峰顶原始值
        for (i = 0; i < CHANNEL; i++)
        {
            g_ValueOrgPeak[i] = t_AcDataGather.AcComponentOrig[i];
        }
        g_lMaxPeakCount = g_lPeakCount;    // 保存当前波峰个数
    }
    if (((g_lLastFingersStatus == FINGER_1 || g_lLastFingersStatus == FINGER_2) && g_lThisFingersStatus == (FINGER_1 | FINGER_2)))  //20230523 Kai 修复一指按一指滑，滑的手指放上去的一瞬，马达释放的Bug
    {
        g_lMaxPeakFlag = 0;
        g_lMinPeakFlag = 0;
    }
    if (g_lMaxPeakFlag == 1 && diff > 0)
    {
        ratio = abs((g_lMaxPeakValue - n2)) * 1.0f / g_lMaxPeakValue; //条件1：波谷足够低于波峰（防止波动导致产生的伪波谷）
        r = 1 - pow(g_tTFParaMeter.g_lExponent * 0.0001f, (g_lPeakCount - g_lMaxPeakCount) * 1.0f) * 0.85f;
        //r = 1 - pow(temp, (g_lPeakCount - g_lMaxgPeakCount)) * 0.85;
        //r = 1 - pow(1, 2);
        if (ratio - r > 0.0001f)
        {
            g_lMinPeakFlag = 1;
            g_lMaxPeakFlag = 0;
            g_lMinPeakValue = n1;
            //存储波谷原始值
            for (i = 0; i < CHANNEL; i++)
            {
                g_ValueOrgValley[i] = t_AcDataGather.AcComponentOrig[i];
            }
        }
        else if (ratio > g_tTFParaMeter.ValleyWaveJudgeThreshold)  //20221024 Janna增加
        {
            g_lMinPeakFlag = 2;
            g_lMaxPeakFlag = 0;
        }
    }
    g_lLastSign = JudgeSign(n2 - n1);

    return diff;

}







