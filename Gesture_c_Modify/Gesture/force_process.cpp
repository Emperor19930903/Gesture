#include <stdio.h>
//#include "gd32e10x.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tp_data.h"
//#include "systick.h"
#include "force_process.h"
#include <stdint.h>
#include"adc_process.h"

/***************** 0.固定参数 **************************/

// 2. ForcePad尺寸信息
// 点位图坐标

int16_t temp_g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM * 2] =  //标准坐标
// int16_t g_TF_Coordinate[][2] =  //标准坐标
{
  70, 58, 142, 58, 214, 58, 286, 58, 358, 58, 430, 58, 502, 58, 574, 58, 646, 58, 718, 58, 790, 58, 862, 58, 934, 58, 1006, 58, 1078, 58, 1150, 58, 1222, 58,
  70, 142, 142, 142, 214, 142, 286, 142, 358, 142, 430, 142, 502, 142, 574, 142, 646, 142, 718, 142, 790, 142, 862, 142, 934, 142, 1006, 142, 1078, 142, 1150, 142, 1222, 142,
  70, 226, 142, 226, 214, 226, 286, 226, 358, 226, 430, 226, 502, 226, 574, 226, 646, 226, 718, 226, 790, 226, 862, 226, 934, 226, 1006, 226, 1078, 226, 1150, 226, 1222, 226,
  70, 310, 142, 310, 214, 310, 286, 310, 358, 310, 430, 310, 502, 310, 574, 310, 646, 310, 718, 310, 790, 310, 862, 310, 934, 310, 1006, 310, 1078, 310, 1150, 310, 1222, 310,
  70, 394, 142, 394, 214, 394, 286, 394, 358, 394, 430, 394, 502, 394, 574, 394, 646, 394, 718, 394, 790, 394, 862, 394, 934, 394, 1006, 394, 1078, 394, 1150, 394, 1222, 394,
  70, 478, 142, 478, 214, 478, 286, 478, 358, 478, 430, 478, 502, 478, 574, 478, 646, 478, 718, 478, 790, 478, 862, 478, 934, 478, 1006, 478, 1078, 478, 1150, 478, 1222, 478,
  70, 562, 142, 562, 214, 562, 286, 562, 358, 562, 430, 562, 502, 562, 574, 562, 646, 562, 718, 562, 790, 562, 862, 562, 934, 562, 1006, 562, 1078, 562, 1150, 562, 1222, 562,
  70, 646, 142, 646, 214, 646, 286, 646, 358, 646, 430, 646, 502, 646, 574, 646, 646, 646, 718, 646, 790, 646, 862, 646, 934, 646, 1006, 646, 1078, 646, 1150, 646, 1222, 646,
  70, 730, 142, 730, 214, 730, 286, 730, 358, 730, 430, 730, 502, 730, 574, 730, 646, 730, 718, 730, 790, 730, 862, 730, 934, 730, 1006, 730, 1078, 730, 1150, 730, 1222, 730,
};
int16_t g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM][2] = {0}; //标准坐标

// 力度系数（下位机）
float g_TF_force_ratio[TF_ROW_NUM * TF_COL_NUM] =
{
	1.0126,0.584,0.4283,0.373,0.3814,0.418,0.4626,0.5016,0.5145,0.4989,0.4556,0.4082,0.3724,0.3715,0.4417,0.6222,1.0734,
	0.5011,0.3043,0.2008,0.1685,0.1837,0.2267,0.2701,0.2999,0.3104,0.2991,0.2692,0.2239,0.177,0.164,0.2031,0.3185,0.5715,
	0.4245,0.2558,0.1694,0.1444,0.1519,0.1863,0.2216,0.2423,0.2493,0.2426,0.2226,0.1882,0.1527,0.145,0.1759,0.2728,0.4778,
	0.4372,0.2979,0.2213,0.1902,0.1855,0.2012,0.2194,0.2268,0.2289,0.2269,0.2196,0.2099,0.2038,0.2105,0.2461,0.3269,0.4938,
	0.4534,0.3316,0.2716,0.2413,0.2255,0.2221,0.2225,0.2221,0.2216,0.2218,0.2218,0.225,0.2379,0.2625,0.2998,0.3684,0.5257,
	0.4208,0.299,0.2394,0.2122,0.1998,0.2025,0.2114,0.2179,0.22,0.2189,0.2146,0.2101,0.214,0.2303,0.2626,0.3353,0.4959,
	0.3804,0.2378,0.1617,0.1396,0.1512,0.1758,0.2011,0.2206,0.2282,0.2225,0.2063,0.1807,0.1552,0.1509,0.1809,0.2682,0.4512,
	0.4107,0.2389,0.149,0.1245,0.1409,0.1815,0.2188,0.2452,0.2562,0.2488,0.2245,0.1873,0.1484,0.1383,0.1684,0.2665,0.477,
	0.6295,0.3949,0.279,0.2345,0.2467,0.2781,0.3131,0.3408,0.3554,0.3475,0.324,0.29,0.2582,0.25,0.3025,0.4313,0.7241
};


/******************* 1.结构体参数库 ************************/
//underglass_v1.05	parameter struct init	20191122
struct TF_PARAMETER g_tTFParaMeter =
{
	
	/********************* 1.力度触发状态参数 *****************/
	//force_trigger_state.c 
	//马达
	g_tTFParaMeter.g_lMotorTrigger	= 60,		// 马达触发力度值
	g_tTFParaMeter.g_lMotorRelease	= 30,		// 马达释放力度值
	
	/********************* 2.滑动状态判断参数 *****************/
	//gesture_slide_state.c	
	g_tTFParaMeter.g_lSliderangeThreshold 	= 30,		// 滑动半径阈值
	g_tTFParaMeter.g_lSlideTimeThreshold		= 19,		// 静止停留阈值
	g_tTFParaMeter.g_lSumSlideThreshold		= 8,		// 静止偏移阈值
	
	/********************* 3.积分计算参数 *****************/
	//	integral_calculate.c
	// AD阈值
	g_tTFParaMeter.g_lACThreshold				= 60,
	// AD释放阈值	
	g_tTFParaMeter.g_lACThresholdRelease		= -45,
	// 两指时AD触发阈值
	g_tTFParaMeter.g_lAC2FingersThreshold		= 80,
	// 两指释放阈值	
	g_tTFParaMeter.g_lAC2FingersThresholdRelease = -120,
	//积分参数 0.95
	g_tTFParaMeter.g_lExponent			= 0.950f * 10000,
	//释放时积分参数 0.6		
	g_tTFParaMeter.g_lExponentRelease		= 0.6000f * 10000,
	// 1指积分释放速度 默认1.08    void integral_calculate()
	g_tTFParaMeter.Finger1ReleaseRate = 1.08f,
	// 双指积分释放速率            void integral_calculate()
	g_tTFParaMeter.g_lDoubleFingerExponentRelease = 0.75f * 10000,
	// 2指释放速率 范围[0.8,1]     void integral_calculate()
	g_tTFParaMeter.g_lFinger2ExponentRelease = 0.9f * 10000,


	/********************* 4.快按手势参数 *****************/
	// gesture_fastpress_norelease_state.c
	//快按速度阈值	22	
	g_tTFParaMeter.g_lCountthrFastpress	= 22,
	// 快按幅值阈值	     
	g_tTFParaMeter.g_lFastpressAmplitudeThr	= 150,
	// 快按峰值阈值	
	g_tTFParaMeter.g_lFastpressMaxPeakThr = 150,
	// touch压帧后显示的第2帧，让马达容易触发（触发力阈值*0.8） void gesture_fastpress_norelease_state()
	g_tTFParaMeter.FastPressTriggerForceDecay=0.8f,
	// (波峰 - 波谷)/波峰 > 0.4 则判断到达了波谷   void FindPeak()
	g_tTFParaMeter.ValleyWaveJudgeThreshold = 0.4f,
	// 负向波谷权重，取值0~1    void JudgeFastPress()
	g_tTFParaMeter.NegativeWaveValleyWeight = 0.85f,
	
	/********************* 5.滑动误触参数 *****************/	
	// gesture_slide_errortrigger_handle.c
	// 滑动误触关键阈值  38760/TF_X_MAX=30; 23640/TF_Y_MAX=30; void gesture_slide_errortrigger_handle()
	g_tTFParaMeter.SwipeThresholdX = 30,
	g_tTFParaMeter.SwipeThresholdY = 30,
	
	/********************* 6.PALM防误触参数 *****************/	
	//palm_finger_press.c
	//PALM识别中时钟延迟20 void palm_finger_press()
	g_tTFParaMeter.DelayCount = 20,
	//余弦相似度阈值0.35 参数指标比较小，说明相关度比较低，阈值0.5，双击会有失败的； void palm_finger_press()
	g_tTFParaMeter.CosineThreshold = 0.35f,
	
	/********************* 7.力度计算参数 *****************/	
	//  force_calculate.c
	//力度除法器
	g_tTFParaMeter.g_lForceDivider			= 100 * (1.0f - 0.950f) * 10000 / 140 * 140,
	// 拖拽状态下 力度保持在100~180之间的随机值，可以将此状态解除，支持拖拽泄力释放  void force_calculate()
	g_tTFParaMeter.DragForce = 100,
	// 拖拽力度锁定
	g_tTFParaMeter.DragForceLock = 1,
	// 拖拽力度为非锁定状态，拖拽力泄放速度
	g_tTFParaMeter.DragForceReleaseRate = 1.2,
	
	g_tTFParaMeter.g_lTestModeRatio	= 1  * 10000,
	//读出基准值
	g_tTFParaMeter.g_lDCComponent[0] = 2048,
	g_tTFParaMeter.g_lDCComponent[1] = 2048,
	g_tTFParaMeter.g_lDCComponent[2] = 2048,
	g_tTFParaMeter.g_lDCComponent[3] = 2048, 
};

/***********************************************************/
/******************* 2.手势参数Flag ************************/
/***********************************************************/

/********************* 1.力度触发状态 *****************/
//超过马达触发值
uint8_t g_tForceTriggerStatus = FORCE_NOTRIGGER;
int32_t motorTriggeredcount = 0;
/********************* 2.滑动状态判断 *****************/
/******1指滑动 ******/
//1指坐标缓存
short g_sTFFingerLastX0[50] = {0};
short g_sTFFingerLastY0[50] = {0};
short g_sTfFingerX0_last = 1;
short g_sTfFingerY0_last = 1;
//当接触后第一个坐标被记录后，置true
int32_t g_lFirstCoorFlag = 0; 
//1指的刚接触坐标
short g_sXOrg = 0;  
short g_sYOrg = 0;
// 1指是否滑动标志
uint8_t g_ucTFIsSlide = 0;
// 1指是否微滑标志
uint8_t swipeFlag = 0; 
// 1指滑动距离
short g_sXSumSlide = 0, g_sYSumSlide = 0;

/******2指滑动 ******/
//2指坐标缓存
short g_sTFFingerLastX1[50]={0}, g_sTFFingerLastY1[50] = {0};
short g_sTfFingerX1_last = 1, g_sTfFingerY1_last = 1;
//当接触后第一个坐标被记录后，置true
int32_t	g_lFirstCoorFlagFinger2 = 0;
//2指的刚接触坐标
short	g_sXOrgFinger2 = 0;   // 2指按压起始位置
short	g_sYOrgFinger2 = 0;
// 2指是否滑动标志
uint8_t	g_lTFIsSlideFinger2 = 0;
// 2指是否微滑标志
uint8_t	swipeFlagfinger2 = 0;
// 2指滑动距离
short g_sXSumSlideFinger2 = 0, g_sYSumSlideFinger2 = 0;

/********************* 3.手指状态 *****************/
//本次手指状态
int32_t g_lThisFingersStatus = 0x0000;
//上一帧手指状态
int32_t g_lLastFingersStatus = 0x0000; 
//当前手指个数
uint8_t  g_ucFingers = 0;   
//上一帧手指个数
uint8_t g_ucFingers_last = 0;
/********************* 4.连击手势 *****************/
//快按方法2	int FindPeak(int n1 ,int n2)
int32_t g_lLastSign = 0;
int32_t	g_lMaxPeakFlag = 0, g_lMaxPeakValue = 0;
int32_t g_lMinPeakFlag = 0, g_lMinPeakValue = 0;
int32_t g_lPeakCount = 0, g_lMaxPeakCount = 0;
int32_t motorTriggered = 0; 
float ac_temp_last_mean3 = 0;
//误判为双击
float g_ValueOrgPeak[CHANNEL] = {0}, g_ValueOrgValley[CHANNEL] = {0};  //储存峰值的原始值
//快按识别时的积分
float g_fFastPressAcAll = 0;
//快按次数
short g_sFastPresCount = 0;
/********************* 5.拖拽手势 *****************/
//拖拽
int32_t g_lFlagDragFinger1 = 0;
int32_t g_lFlagDragFinger1Last = 0;
int32_t g_lFlagDragFinger2 = 0;
int32_t g_lFlagDragFinger2Last = 0;


/********************* 6.Palm手势 *****************/
//PALM状态参数
int32_t   g_lThisPalmStatus = 0x0000;      //PALM0--PALM4
uint16_t  g_usPalmTimerStart = 0;
uint8_t   g_usPalmFlag = 0;
uint16_t g_usPalmTimerCount = 0;
uint8_t  g_ucPalms = 0;      //Palm 个数
uint16_t g_ucPalms_last = 0;  // 上一帧Palm个数
uint16_t  Delay_times= 0;     // 延迟计时

/********************* 7.计算力度 *****************/
//计算力度 TF_Getforce(void)
uint32_t g_ulTF_force = 0, g_ulLastforce = 0;
//单点系数系数 static float TF_Interpolation()
float g_TFRatio = 0;

/********************* 8.积分计算标志位 *****************/
//通道选择
int g_lTFCh = 0;
//判断是否第一次穿过g_tTFParaMeter.g_lACThreshold,根据赵艳红要求，清除掉前一个值：t_AcDataGather.AcComponentLast[i] = 0
char g_cFirstCrossACThresholdFlag = 0;

/********************* 9.手势切换标志位*****************/
//一指按、一指划、抬起一指
char g_cFlagLeaveFinger = 0;
//解决一指按压，一指抬起
short g_sCountLeaveFinger = 0;
//单手一指按一指划 标志位
char g_cFlag1FingerPress2FingerSlide = 0;
//1指按切换双指 有力  标志位
char gesture_1fingerpress_2fingertouch = 0;
// 双指切换2指 有力   标志位
char gesture_1fingerleave_2fingertouch = 0;
//14寸一体式：一指滑，另一指按，按的手指抬离，应该掉落_20221012
// 1指滑动且不触发标志位
char gesture_1fingerslidenoforce = 0;
//1指滑切换至双指且力度触发
char gesture_2fingerspress = 0;
//1指按2指离
char gesture_1fingertouch2fingerleave = 0;

/*********** 10.AC数据结构体集合 ***********/
struct AcDataGather_T t_AcDataGather={0}; //结构体初始化
struct FingerGather_T tTouchFingers = { 0 };
struct TouchGather_T tTouchVariate=
{
	tTouchVariate.tpIntFlag=1,
	tTouchVariate.pressFlag=1,
	tTouchVariate.releaseFlag=1,
	tTouchVariate.tpSendFlag=1,
	tTouchVariate.tpOpenFlag=1,
	tTouchVariate.tpRecFlag=1,
	tTouchVariate.passTouchFlag = 1,
	tTouchVariate.disenableTouchFlag = 1,
	tTouchVariate.resetAttnFlag = 1,
	tTouchVariate.resetAttnCount = 1,
	tTouchVariate.touchRawdataFlag = 1,
	tTouchVariate.tpGetData[64]={0},
	tTouchVariate.tpBuffer[64]={0},
	tTouchVariate.readPassLength
};
struct ADCCllect_T ADCCllect = { 0 };