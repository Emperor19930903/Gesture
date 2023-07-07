#include "force_process.h"
#include "integral_calculate.h"
#include "tp_data.h"
#include "palms_sequence_get.h"
#include "tf_finger.h"

extern  TF_PARAMETER g_tTFParaMeter;
extern AcDataGather_T t_AcDataGather;
extern FingerGather_T tTouchFingers;

//积分值计算
void integral_calculate(void)
{

	float ac_temp_last = t_AcDataGather.AcComponentLast[g_lTFCh];
	float ac_temp = t_AcDataGather.AcComponent[g_lTFCh];
	short i = 0;

	/******µ¥¶ÀPALMÊ±Êä³öÁ¦¶ÈÎª0******/
//		if((g_lThisPalmStatus == 0 && g_usPalmFlag  == 1) || (g_usPalmFlag  == 0))
//			g_usPalmTimerCount = 0;

		//		if((g_tTfFinger.Size0 > 38 || g_tTfFinger.Size1 > 38 || g_tTfFinger.Size2 > 38 || g_tTfFinger.Size3 > 38 || g_tTfFinger.Size4 > 38) && g_lThisPalmStatus == 0 && g_lThisFingerPalmStatus > 0)
		//		{
		//			t_AcDataGather.AcAll = 0;
		//			t_ForceGather.Force = 0;
		//			/*******ÊÖÊÆËùÓÐ±äÁ¿¸´Î»	¿ªÊ¼	LLS*******/
		//			TF_VariableReset();
		//			/*******ÊÖÊÆËùÓÐ±äÁ¿¸´Î»	½áÊø	LLS*******/
		//		}
	if (g_ucFingers > 1 && g_ucPalms == 0)
	{
		ac_temp_last = ac_temp_last * 0.5;
		ac_temp = ac_temp * 0.5;
	}
	if (g_lThisPalmStatus > 0 && g_lThisFingersStatus == 0) //|| (g_usPalmFlag == 1 &&g_lThisFingersStatus == 0)) //|| (g_lThisPalmStatus == 0 && g_lThisFingerPalmStatus > 3))//¸ÃÌõ¼þÊ¹ÈýÖ¸ÒÔÉÏÃ»ÓÐÁ¦¶È
	{
		t_AcDataGather.AcAll = 0;
		g_ulTF_force = 0;
		/*******ÊÖÊÆËùÓÐ±äÁ¿¸´Î»	¿ªÊ¼	LLS*******/
		TF_VariableReset();
		TouchDataZero();
	}
	/********** 1指状态 **********/
	else
	{
		if (g_lThisFingersStatus == FINGER_1)
		{
			if (g_lFlagDragFinger1 == 0)//不是拖拽
			{
				JudgeFirstCrossAC_threshold(t_AcDataGather.AcComponent[g_lTFCh], g_lTFCh); //20200529,如果是第一次穿过60，则将last清零，保持与MATLAB一致  这个是否可以去除？？？
				if (g_cFlagLeaveFinger == 0)
				{
					if (ac_temp < g_tTFParaMeter.g_lACThresholdRelease) //此代码应加快释放效果 特别是有两指时，残留大。负波向下
					{
						if (ac_temp * 1.0f / ac_temp_last > g_tTFParaMeter.Finger1ReleaseRate)
						{
							t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponentRelease * 0.0001f;
						}
						else
						{
							t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
						}
					}
					else if (ac_temp > g_tTFParaMeter.g_lACThreshold)
					{
						t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
						//对超过阈值时刻有负波的情况进行翻转积分，适合多次的滑动后停止按压以及抬离按压
						if (g_ucTFIsSlide == 0)//不是滑动
						{
							if (ac_temp_last <= g_tTFParaMeter.g_lACThreshold)
							{
								for (i = 0; i < CHANNEL; i++)
								{
									if (t_AcDataGather.AcComponentOrig[i] < 0)
									{
										t_AcDataGather.AcAll = t_AcDataGather.AcAll - t_AcDataGather.AcComponentOrig[i] - t_AcDataGather.AcComponentOrig[i];   // 负波取反
									}
								}
							}
						}
					}
				}
				else    //一指按另一指滑停抬离
				{
					if (g_sCountLeaveFinger < 10)
					{
						g_sCountLeaveFinger++;
					}
					else
					{
						g_cFlagLeaveFinger = 0;
					}
				}
			}
			else    //是拖拽
			{
				if (ac_temp >= g_tTFParaMeter.g_lACThreshold || ac_temp <= g_tTFParaMeter.g_lACThresholdRelease)
				{
					t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
				}
			}
		}
		/********** 双指状态 *********/
		else if (g_lThisFingersStatus == (FINGER_1 | FINGER_2))
		{
			//两指无滑动
			if (g_ucTFIsSlide == 0 && g_lTFIsSlideFinger2 == 0)
			{
				if (ac_temp >= g_tTFParaMeter.g_lAC2FingersThreshold || ac_temp <= g_tTFParaMeter.g_lAC2FingersThresholdRelease)
				{
					//负波向上时不积分
					if (ac_temp < 0 && ac_temp_last < 0 && ac_temp > ac_temp_last)
					{
					}
					//负向积分波形下降 加快释放
					else if (ac_temp < 0 && ac_temp_last < 0 && ac_temp < ac_temp_last) //此代码应加快释放效果 特别是有两指时，残留大。负波向下
					{
						t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lDoubleFingerExponentRelease * 0.0001f;
					}
					else   //正波向上且未触发马达，即双指滑动，减小积分防误触；
					{
						//降低双指滑动误触 正向积分 上升
						if (ac_temp > 0 && ac_temp_last > 0 && ac_temp > ac_temp_last && g_tForceTriggerStatus == FORCE_NOTRIGGER)
						{
							t_AcDataGather.AcAll += (ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f) * (exp(-0.02f * sqrt(
								(tTouchFingers.X0 - g_sTfFingerX0_last + tTouchFingers.X1 - g_sTfFingerX1_last) * (tTouchFingers.X0 - g_sTfFingerX0_last + tTouchFingers.X1 - g_sTfFingerX1_last) +
								(tTouchFingers.Y0 - g_sTfFingerY0_last + tTouchFingers.Y1 - g_sTfFingerY1_last) * (tTouchFingers.Y0 - g_sTfFingerY0_last + tTouchFingers.Y1 - g_sTfFingerY1_last)
							)
							)
								);
						}
						else   //否则正常计算积分
						{
							t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
						}
					}
				}
			}
			else   //双指有滑动
			{
				if (ac_temp >= g_tTFParaMeter.g_lACThreshold || ac_temp <= g_tTFParaMeter.g_lACThresholdRelease * 0.0001f)
				{
					//负波向上时不积分；
					if (t_AcDataGather.AcComponent[0] < 0 && t_AcDataGather.AcComponentLast[0] < 0 && t_AcDataGather.AcComponent[0] > t_AcDataGather.AcComponentLast[0])
					{
					}

					// 否则正常计算积分
					else
					{
						t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
					}
				}
			}

		}
		/********** 只存在2指 **********/
		else if (g_lThisFingersStatus == FINGER_2)
		{
			if (g_lFlagDragFinger2 == 0)//不是拖拽
			{
				if (ac_temp >= g_tTFParaMeter.g_lACThreshold || ac_temp <= g_tTFParaMeter.g_lACThresholdRelease)
				{
					if (ac_temp < 0) //此代码应加快释放效果 特别是有两指时，残留大。负波向下 <负阈值时加快积分释放；
					{
						t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lFinger2ExponentRelease * 0.0001f;
					}
					//否则正常计算积分
					else t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
				}
			}
			else//是拖拽 正常计算积分
			{
				if (ac_temp >= g_tTFParaMeter.g_lACThreshold || ac_temp <= g_tTFParaMeter.g_lACThresholdRelease)
				{
					t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
				}
			}
		}

		/********** 存在更多手指 **********/
		else
		{
			if (ac_temp >= g_tTFParaMeter.g_lACThreshold || ac_temp <= g_tTFParaMeter.g_lACThresholdRelease)
			{
				if (ac_temp < 0) //此代码应加快释放效果 特别是有两指时，残留大。负波向下
				{
					t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lFinger2ExponentRelease * 0.0001f;
				}
				else t_AcDataGather.AcAll += ac_temp - ac_temp_last * g_tTFParaMeter.g_lExponent * 0.0001f;
			}
		}
		if (t_AcDataGather.AcAll - 0.1f < 0.0001f)
		{
			t_AcDataGather.AcAll = 0;
		}
	}
}


//判断是否第一次穿过g_tTFParaMeter.g_lACThreshold，根据赵艳红要求，清除掉前一个值：t_AcDataGather.AcComponentLast[i] = 0,为了保证与MATLAB一致

int JudgeFirstCrossAC_threshold(int ac, int ch)
{

	if (g_cFirstCrossACThresholdFlag == 0 && ac - g_tTFParaMeter.g_lACThreshold >= 0)

	{

		g_cFirstCrossACThresholdFlag = 1;

		t_AcDataGather.AcComponentLast[ch] = 0;

		return 1;

	}

	else

	{

		return 0;

	}

}


