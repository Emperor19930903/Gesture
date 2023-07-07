#ifndef _MAIN_H
#define _MAIN_H
#include "plot_matlab.h"
//声明结构体别名

//画图需要的变量
vector<uint32_t >forceSim(MAX_LINE_LENGTH);
vector<uint32_t >forceSoft(MAX_LINE_LENGTH);
vector<uint32_t >Acall(MAX_LINE_LENGTH);
vector<int32_t >ADCH1(MAX_LINE_LENGTH);
vector<int32_t >ADCH2(MAX_LINE_LENGTH);
vector<int32_t >ADCH3(MAX_LINE_LENGTH);
vector<int32_t >ADCH4(MAX_LINE_LENGTH);
vector<int32_t >ADSum(MAX_LINE_LENGTH);
vector<float>Ratio(MAX_LINE_LENGTH);
int DCflag;//读取DC 标志位
    // 定义变量
int AD_CH4[4];
int DC_CH4[4] = {2050,2050,2051,2051};
int id0 = 0;
int flag0 = 0;
int x_0 = 0;
int y_0 = 0;
int id1 = 0;
int flag1 = 0;
int x_1 = 0;
int y_1 = 0;
int id2 = 0;
int flag2 = 0;
int x_2 = 0;
int y_2 = 0;
int id3 = 0;
int flag3 = 0;
int x_3 = 0;
int y_3 = 0;
int id4 = 0;
int flag4 = 0;
int x_4 = 0;
int y_4 = 0;
int botton = 0;



#endif