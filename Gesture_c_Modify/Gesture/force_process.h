#ifndef _FORCE_PROCESS_H
#define _FORCE_PROCESS_H
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/*参数个数*/
#define PARA_AMOUNT     33	//20230331(LCL)

/********** 宏定义 **********/
#define FINGER_1 0x0001
#define FINGER_2 (0x0001<<1)
#define FINGER_3 (0x0001<<2)
#define FINGER_4 (0x0001<<3)
#define FINGER_5 (0x0001<<4)
#define CHANNEL  4
#define SINGLEBYTE_FORCE_UPDATE 1   //单字节力度上传
#define FORCE_NOTRIGGER 0           // 力度未触发标志
#define FORCE_TRIGGERED 1           // 力度触发标志
// 2. ForcePad尺寸信息
#define TF_X_MAX 1292    // 下位机物理像素坐标范围
#define TF_Y_MAX 788	
#define TF_X_LENGTH 107  // AA区尺寸
#define TF_Y_LENGTH 65
#define TF_ROW_NUM 9     // 点位图尺寸
#define TF_COL_NUM 17  

#define xCoorLeftMax 70      //左边界横坐标
#define yCoorUpMin 58        //上边界纵坐标
#define xCoorRightMin 1222   //右边界横坐标
#define yCoorDownMax 730     //下边界纵坐标

#define LG_14RS 1

// #define WAVE_DIR_REVERSE  //若电路反向 则取消该注释



// 解决palm手势用到的系数以及坐标信息
extern float g_TF_force_ratio[TF_ROW_NUM * TF_COL_NUM];       //下位机给出力度系数
extern int16_t g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM][2];
extern int16_t temp_g_TF_Coordinate[TF_ROW_NUM * TF_COL_NUM * 2];
extern int32_t LG14_ratio[32][4];   // Palm模板系数，需要手动添加
extern int32_t LG14_XY[32][2];      // Palm模板坐标，需要手动添加

/***********************************************************/
/******************* 1.结构体参数库 ************************/
/***********************************************************/
struct TF_PARAMETER {
	
	/********************* 1.力度触发状态参数 *****************/
	//force_trigger_state.c 	
	//马达触发力度值
	int	g_lMotorTrigger; 
	//马达释放力度值  
	int	g_lMotorRelease; 
	
	/********************* 2.滑动状态判断参数 *****************/
	//gesture_slide_state.c	
	// 滑动半径阈值
	int g_lSliderangeThreshold; 
	// 静止停留阈值  
	int g_lSlideTimeThreshold;   
	// 静止偏移阈值  
	int	g_lSumSlideThreshold;  
	
	/********************* 3.积分计算参数 *****************/
	//	integral_calculate.c
	// AD阈值
	int	g_lACThreshold;
	// AD释放阈值
	int	g_lACThresholdRelease;
	// 两指时AD触发阈值
	int	g_lAC2FingersThreshold;
	// 两指释放阈值	
	int	g_lAC2FingersThresholdRelease;
	//积分参数0.95
	int	g_lExponent;
	//释放时积分参数0.6
	int	g_lExponentRelease;
	// 1指积分释放速度 默认1.08    void integral_calculate()
	float Finger1ReleaseRate;   
	// 双指积分释放速率            void integral_calculate()            
	float g_lDoubleFingerExponentRelease;         
	// 2指释放速率 范围[0.8,1]     void integral_calculate()
	float g_lFinger2ExponentRelease; 
	
	/********************* 4.快按手势参数 *****************/
	// gesture_fastpress_norelease_state.c
	//快按速度阈值	22
	int	g_lCountthrFastpress;
	// 快按幅值阈值	 
	int	g_lFastpressAmplitudeThr;
	// 快按峰值阈值	
	int	g_lFastpressMaxPeakThr;
	// touch压帧后显示的第2帧，让马达容易触发（触发力阈值*0.8） void gesture_fastpress_norelease_state()
	float FastPressTriggerForceDecay;         
	// (波峰 - 波谷)/波峰 > 0.4 则判断到达了波谷   void FindPeak()
	float ValleyWaveJudgeThreshold;
	// 负向波谷权重0.85，取值0~1    void JudgeFastPress()
	float NegativeWaveValleyWeight; 

	
	/********************* 5.滑动误触参数 *****************/	
	// gesture_slide_errortrigger_handle.c
	// 滑动误触关键阈值  38760/TF_X_MAX=30; 23640/TF_Y_MAX=30; void gesture_slide_errortrigger_handle()
	float SwipeThresholdX;
	float SwipeThresholdY;
	
	/********************* 6.PALM防误触参数 *****************/	
	//palm_finger_press.c
	//PALM识别中时钟延迟20 void palm_finger_press()
	int DelayCount;
	//余弦相似度阈值0.35 参数指标比较小，说明相关度比较低，阈值0.5，双击会有失败的； void palm_finger_press()
	float CosineThreshold;

	/********************* 7.力度计算参数 *****************/	
	//  force_calculate.c
	//力度除法器
	int	g_lForceDivider;
	//拖拽力度100 拖拽状态下，力度小于100时，让力度保持在100~180之间 void force_calculate()
	int DragForce;
	// 拖拽力度锁定
	int DragForceLock;
	// 拖拽力度为非锁定状态，拖拽力泄放速度
	float DragForceReleaseRate;
	// 力度*1.9 保证和原测试模式一致
	int g_lTestModeRatio; 
	//读出基准值
	int	g_lDCComponent[4]; 
};
//struct TF_PARAMETER g_tTFParaMeter;


/***********************************************************/
/******************* 2.手势参数Flag ************************/
/***********************************************************/

/********************* 1.力度触发状态 *****************/
//超过马达触发值
//typedef enum {FORCE_NOTRIGGER = 0, FORCE_TRIGGERED = 1} ForceTriggerStatus;
extern uint8_t g_tForceTriggerStatus;
extern int32_t motorTriggeredcount;
/********************* 2.滑动状态判断 *****************/
/******1指滑动 ******/
//1指坐标缓存
extern short g_sTFFingerLastX0[50],g_sTFFingerLastY0[50];
extern short g_sTfFingerX0_last,g_sTfFingerY0_last;  
//当接触后第一个坐标被记录后，置true
extern int g_lFirstCoorFlag;
//1指的刚接触坐标
extern short g_sXOrg;       
extern short g_sYOrg;
// 1指是否滑动标志
extern uint8_t g_ucTFIsSlide;
// 1指是否微滑标志
extern uint8_t swipeFlag;
// 1指滑动距离
extern short g_sXSumSlide,g_sYSumSlide;

/******2指滑动 ******/
//2指坐标缓存
extern short g_sTFFingerLastX1[50],g_sTFFingerLastY1[50];
extern short g_sTfFingerX1_last,g_sTfFingerY1_last;
//当接触后第一个坐标被记录后，置true
extern int	g_lFirstCoorFlagFinger2;
//2指的刚接触坐标
extern short	g_sXOrgFinger2;
extern short	g_sYOrgFinger2;	
// 2指是否滑动标志
extern uint8_t	g_lTFIsSlideFinger2;
// 2指是否微滑标志
extern uint8_t swipeFlagfinger2;
// 2指滑动距离
extern short g_sXSumSlideFinger2,g_sYSumSlideFinger2;

/********************* 3.手指状态 *****************/
//本次手指状态
extern int32_t g_lThisFingersStatus;
// 上一帧手指状态
extern int32_t g_lLastFingersStatus;
 //当前手指个数
extern uint8_t g_ucFingers;
//上一帧手指个数
extern uint8_t g_ucFingers_last; 

/********************* 4.连击手势 *****************/
//快按方法2	int FindPeak(int n1 ,int n2)
extern int32_t g_lLastSign;
extern int32_t	g_lMaxPeakFlag,g_lMaxPeakValue;
extern int32_t g_lMinPeakFlag,g_lMinPeakValue;		
extern int32_t g_lPeakCount,g_lMaxPeakCount;
extern int32_t motorTriggered;
//平滑三帧的波形
extern float ac_temp_last_mean3;
//误判为双击
extern float g_ValueOrgPeak[CHANNEL],g_ValueOrgValley[CHANNEL];
//快按识别时的积分
extern float g_fFastPressAcAll;
//快按次数
extern short g_sFastPresCount;

/********************* 5.拖拽手势 *****************/
//拖拽
extern int32_t g_lFlagDragFinger1;
extern int32_t g_lFlagDragFinger1Last;
extern int32_t g_lFlagDragFinger2;
extern int32_t g_lFlagDragFinger2Last;

/********************* 6.Palm手势 *****************/
//PALM状态参数
extern int32_t   g_lThisPalmStatus;//PALM0--PALM4
extern uint16_t  g_usPalmTimerStart;
extern uint8_t   g_usPalmFlag;
extern uint16_t  g_usPalmTimerCount;
extern uint8_t g_ucPalms;  //当前Palm 个数
extern uint16_t g_ucPalms_last;  // 上一帧Palm个数
extern uint16_t  Delay_times;    // 延迟计时

/********************* 7.计算力度 *****************/
//计算力度TF_Getforce(void)	
extern unsigned int g_ulTF_force,g_ulLastforce;
//单点系数 static float TF_Interpolation()
extern float g_TFRatio;

/********************* 8.积分计算标志位 *****************/
//通道选择
extern int g_lTFCh; 
//判断是否第一次穿过g_tTFParaMeter.g_lACThreshold,根据赵艳红要求，清除掉前一个值：t_AcDataGather.AcComponentLast[i] = 0
extern char g_cFirstCrossACThresholdFlag;

/********************* 9.手势切换标志位*****************/
//一指按、一指划、抬起一指
extern char g_cFlagLeaveFinger;
//解决一指按压，一指抬起	
extern short g_sCountLeaveFinger;
//单手一指按一指划 标志位
extern char g_cFlag1FingerPress2FingerSlide;
//1指按切换双指 有力  标志位
extern char gesture_1fingerpress_2fingertouch;
// 双指切换2指 有力   标志位
extern char gesture_1fingerleave_2fingertouch;
// 1指滑动且不触发标志位
extern char gesture_1fingerslidenoforce;
//1指滑切换至双指且力度触发
extern char gesture_2fingerspress;
//1指按2指离
extern char gesture_1fingertouch2fingerleave;



/*********** 10.AC数据结构体集合 ***********/
struct AcDataGather_T
{ 
	int32_t AcComponent[CHANNEL]; 
	int32_t AcComponentOrig[CHANNEL]; 
	int32_t AcComponentOrigLast[CHANNEL];
	int32_t AcComponentOrigBefore[CHANNEL];   //去直流分量
	int32_t AcComponentLast[CHANNEL];
	int32_t AcComponentBefore[CHANNEL];   //去直流分量
	int32_t AcTotalEveryComponent[CHANNEL];
	int32_t AcTotalEveryComponentLast[CHANNEL]; 
	int32_t TimerAcEveryComponent[CHANNEL];   //
	int32_t PcAcAllComponent[CHANNEL];   //
	float AcEvery[CHANNEL];
	float AcAll;
	float AcAllTemp;
	int32_t AcAllComponent;
	int32_t Timer_AcAllComponent; 
	int32_t AcAllSum;
	int32_t AcAllComponentLast; //四通道去直流分量总值
};
//struct AcDataGather_T t_AcDataGather; //结构体声明



//void force_process(void);
#endif