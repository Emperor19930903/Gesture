#ifndef _INTEGRAL_CALCULATE_H
#define _INTEGRAL_CALCULATE_H

//积分值计算

void integral_calculate(void);

//判断是否第一次穿过g_tTFParaMeter.g_lACThreshold，根据赵艳红要求，清除掉前一个值：t_AcDataGather.AcComponentLast[i] = 0,为了保证与MATLAB一致
int JudgeFirstCrossAC_threshold(int ac, int ch);

#endif