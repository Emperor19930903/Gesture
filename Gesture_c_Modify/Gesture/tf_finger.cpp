#include "tf_finger.h"
#include "ad_dc_calculate.h"
#include "fingers_sequence_get.h"
#include "palms_sequence_get.h"
#include "palm_finger_press.h"
#include "force_trigger_state.h"
#include "gesture_slide_state.h"
#include "gesture_drag_state.h"
#include "ratio_calculate.h"
#include "gesture_switch_data_reset.h"
#include "integral_calculate.h"
#include "gesture_fastpress_norelease_state.h"
#include "gesture_slide_errortrigger_handle.h"
#include "force_calculate.h"
#include "history_data_save.h"
#include "force_process.h"
#include "tp_data.h"

/*******************函数定义************************/

extern AcDataGather_T t_AcDataGather;
extern FingerGather_T tTouchFingers;
extern TouchGather_T tTouchVariate;
/* 计算力度 */
unsigned int TF_Getforce(void)
{

    // AD计算模块
    ad_dc_calculate();

    // 手指状态模块
    fingers_sequence_get();

    //Palm状态模块
    palm_sequence_get();

    // 力度触发模块
    force_trigger_state();

    // 滑动状态模块
    gesture_slide_state();

    // 拖拽状态模块
    gesture_drag_state();

    // 系数计算模块
    ratio_calculate();

    // 手势切换模块
    gesture_switch_data_reset();

    // 积分计算模块
    integral_calculate();

    // 连击模块
    gesture_fastpress_norelease_state();

    // 滑动误触模块
    gesture_slide_errortrigger_handle();

    
    // 力度计算模块
    force_calculate();
    // 历史数据缓存模块
    history_data_save();
	
	// 手指抬离，Flag归位
	if (g_ucFingers == 0)
	{
	 	TF_VariableReset();		
	 }
    //每帧进行清空Touchdata
    TouchDataZero();
    return g_ulTF_force;

}


// 触摸数据清零
void TouchDataZero(void)
{
    tTouchFingers.X0 = 0;
    tTouchFingers.Y0 = 0;
    tTouchFingers.X1 = 0;
    tTouchFingers.Y1 = 0;
    tTouchFingers.X2 = 0;
    tTouchFingers.Y2 = 0;
    tTouchFingers.X3 = 0;
    tTouchFingers.Y3 = 0;
    tTouchFingers.X4 = 0;
    tTouchFingers.Y4 = 0;
    tTouchFingers.Finger0 = 0;
    tTouchFingers.Finger1 = 0;
    tTouchFingers.Finger2 = 0;
    tTouchFingers.Finger3 = 0;
    tTouchFingers.Finger4 = 0;
    tTouchFingers.Palm0 = 0;
    tTouchFingers.Palm1 = 0;
    tTouchFingers.Palm2 = 0;
    tTouchFingers.Palm3 = 0;
    tTouchFingers.Palm4 = 0;
    tTouchFingers.Size0 = 0;
    tTouchFingers.Size1 = 0;
    tTouchFingers.Size2 = 0;
    tTouchFingers.Size3 = 0;
    tTouchFingers.Size4 = 0;
}

//手指全部离开后的变量复位
void TF_VariableReset(void)
{
   

    /*******************全局变量初始化************************/
    //系数	static float TF_Interpolation()

    //g_TFRatio = 0;
    //第一指滑动	void Side(void)
    g_lFirstCoorFlag = 0;//当接触后第一个坐标被记录后，置true
    g_sXOrg = 0;
    g_sYOrg = 0;
    g_ucTFIsSlide = 0;
    swipeFlag = 0;
    swipeFlagfinger2 = 0;
    g_sXSumSlide = 0;
    g_sYSumSlide = 0;
    //第二指滑动	void Side_finger2(void)
    g_lFirstCoorFlagFinger2 = 0;
    g_sXOrgFinger2 = 0;
    g_sYOrgFinger2 = 0;
    g_lTFIsSlideFinger2 = 0;
    g_sXSumSlideFinger2 = 0;
    g_sYSumSlideFinger2 = 0;
    //清空滑动数组
    for (int j = 0; j < 50; j++)
    {
        g_sTFFingerLastX0[j] = 0;
        g_sTFFingerLastY0[j] = 0;
        g_sTFFingerLastX1[j] = 0;
        g_sTFFingerLastY1[j] = 0;
    }
    //快按方法2	int FindPeak(int n1 ,int n2)
    g_lLastSign = 0;
    g_lMaxPeakFlag = 0;
    g_lMaxPeakValue = 0;
    g_lMinPeakFlag = 0;
    g_lMinPeakValue = 0;
    g_lPeakCount = 0;
    g_lMaxPeakCount = 0;

    //拖拽
    g_lFlagDragFinger1 = 0;
    g_lFlagDragFinger1Last = 0;
    g_lFlagDragFinger2 = 0;
    g_lFlagDragFinger2Last = 0;

    //本次手指状态 上次手指状态
    g_lThisFingersStatus = 0x0000;
    g_lLastFingersStatus = 0x0000;//第四指。。第一指
    g_ucFingers = 0;
    //Palm清零
    g_lThisPalmStatus = 0;
    g_ucPalms = 0;
    //判断是否第一次穿过g_tTFParaMeter.g_lACThreshold，根据赵艳红要求，清除掉前一个值：t_AcDataGather.AcComponentLast[i] = 0	20200529
    g_cFirstCrossACThresholdFlag = 0;

    //注意以下几个初始化为0的变量没有在这里复位！
    /*计算力度TF_Getforce(void)*/
    g_ulTF_force = 0;
    g_ulLastforce = 0;

    //力度	main.c
    for (int i = 0; i < CHANNEL; i++)
    {
        t_AcDataGather.AcComponent[i] = 0;
        t_AcDataGather.AcComponentOrigBefore[i] = 0;
        t_AcDataGather.AcComponentOrigLast[i] = 0;
        t_AcDataGather.AcComponentOrig[i] = 0;
        t_AcDataGather.AcComponentBefore[i] = 0;//20200528
        t_AcDataGather.AcComponentLast[i] = 0;
    }
    t_AcDataGather.AcAllComponent = 0;
    t_AcDataGather.AcAll = 0;
    t_AcDataGather.AcAllTemp = 0;



    //一指按、一指划、抬起一指
    g_cFlagLeaveFinger = 0;
    //解决一指按压，一指抬起
    g_sCountLeaveFinger = 0;

    g_cFlag1FingerPress2FingerSlide = 0;

    //快按识别时的积分，应对touch压1帧处理
    g_fFastPressAcAll = 0;
    g_sFastPresCount = 0;

    //14寸一体式：一指滑，另一指按，按的手指抬离，应该掉落_20221012
    gesture_1fingerslidenoforce = 0;
    gesture_2fingerspress = 0;
    gesture_1fingertouch2fingerleave = 0;

    /* 清除Touch接收完成标志位 */
    tTouchVariate.tpRecFlag = 0;

}