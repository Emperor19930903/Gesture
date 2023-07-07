#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "main.h"
#include "adc_process.h"
#include "tp_data.h"
#include "force_process.h"
#include "tf_finger.h"
#include <inttypes.h>
#include<cstdio>
#include<ios>
#include<iostream>
#include<vector>
#include "plot_matlab.h"
#include "queue.h"
using namespace std;

extern  TF_PARAMETER g_tTFParaMeter;
extern AcDataGather_T t_AcDataGather;
extern TouchGather_T tTouchVariate;
extern FingerGather_T tTouchFingers;
extern ADCCllect_T ADCCllect;
// 主函数体
int main() 
{
    //初始化队列
    SeqQueue queue; InitQueue(queue);
    /************************ 1.读取系数 ******************************/
    FILE *fileCoe;
    char lineCoe[MAX_LINE_LENGTH];

    // 打开系数文件,待读取.txt文件放入资源文件夹下
    fileCoe = fopen("data\\coe_Module.txt", "r");

    // 检查文件是否成功打开
    if (fileCoe == NULL) {
        cout<<"无法打开文件"<<endl;
        return 1;
    }

    // 逐行读取文件内容
    int s=0;
    while (fgets(lineCoe, MAX_LINE_LENGTH, fileCoe) != NULL) {
        float temp_coe = atof(lineCoe)/10000;   //将字符数组转换为float
        g_TF_force_ratio[s] = temp_coe;
        // printf("%f\n", temp_coe);
        s = s + 1;
    }

    // 关闭文件
    fclose(fileCoe);

    /************************************ 2.打开CSV手势数据 **********************/
    // 打开系数文件, 待读取.csv文件放入资源文件夹下

    //FILE *file = fopen("data\\Modual_FW_fastPress.csv", "r");
    FILE* file = fopen("data\\LG客诉问题\\双指滚动误触\\Log_2023-06-27 11-06-32_1.csv", "r");
    if (file == NULL) 
    {
        printf("无法打开CSV文件。\n");
        return 1;
    }
    // 调整点位坐标
    int number=0;
    while (number<(TF_ROW_NUM * TF_COL_NUM * 2))
    {
        if ((number%2)==0)
        {
            g_TF_Coordinate[number/2][0]=temp_g_TF_Coordinate[number];
            g_TF_Coordinate[number/2][1]=temp_g_TF_Coordinate[number+1];    
        }
        number = number + 1;       
    }
    char line[MAX_LINE_LENGTH];   // 创建单字节数组 由于存储一行字符串
    int Row = 0;  // csv行序号
    while (fgets(line, MAX_LINE_LENGTH, file)) 
    {
        Row = Row + 1;
        if (Row==1) {continue;}   // 第一行表头

        char *field = strtok(line, ",");
        cout << atoi(field) << endl;
        if (atoi(field) == 0)
        {
            DCflag = 1;
        }
        int t = 0;
        while (field != NULL) 
        {
            
            int num = atoi(field);   // 字符转整型
            //读取DC
            if (DCflag == 1)
            {
                g_tTFParaMeter.g_lDCComponent[t] = num;
            }
            // 1.TF AD值
            if (t>0 && t<5) { ADCCllect.Value[t-1] = num; }

            // 2. Touch信息获取          
            if (t==5) id0 = num;
            if (t==6) flag0 = num;
            if (t==7) x_0 = num;
            if (t==8) y_0 = num; 
            if (t==9) id1 = num; 
            if (t==10) flag1 = num; 
            if (t==11) x_1 = num; 
            if (t==12) y_1 = num; 
            if (t==13) id2 = num; 
            if (t==14) flag2 = num; 
            if (t==15) x_2 = num; 
            if (t==16) y_2 = num; 
            if (t==17) id3 = num; 
            if (t==18) flag3 = num; 
            if (t==19) x_3 = num; 
            if (t==20) y_3 = num; 
            if (t==21) id4 = num; 
            if (t==22) flag4 = num; 
            if (t==23) x_4 = num; 
            if (t==24) y_4 = num; 
            if (t==25) botton = num; 
            if (t==26) forceSoft[Row-1] = num*10;
            field = strtok(NULL, ",");
            t = t + 1;

            // Touch信息解析
            switch (flag0)
            {
                case 0:
                    tTouchFingers.Finger0 = 0;
                    tTouchFingers.Palm0 = 0;
                    break;
                case 1:
                    tTouchFingers.Finger0 = 0;
                    tTouchFingers.Palm0 = 0;
                    break;
                case 2:
                    tTouchFingers.Palm0 = 1;
                    tTouchFingers.Finger0 = 0;
                    break;
                case 3:
                    tTouchFingers.Palm0 = 0;
                    tTouchFingers.Finger0 = 1;
            }
            tTouchFingers.X0 = x_0;
            tTouchFingers.Y0 = y_0;        
            int fullflag=EnQueue(queue, x_0);
            if (fullflag == 0)
            {
                DeQueue(queue, x_0);
                EnQueue(queue, x_0);
            }
            switch (flag1)
            {
                case 0:
                    tTouchFingers.Finger1 = 0;
                    tTouchFingers.Palm1 = 0;
                    break;
                case 1:
                    tTouchFingers.Finger4 = 0;
                    tTouchFingers.Palm4 = 0;
                    break;
                case 2:
                    tTouchFingers.Palm1 = 1;
                    tTouchFingers.Finger1 = 0;
                case 3:
                    tTouchFingers.Palm1 = 0;
                    tTouchFingers.Finger1 = 1;
            }
            tTouchFingers.X1 = x_1;
            tTouchFingers.Y1 = y_1;

            switch (flag2)
            {
                case 0:
                    tTouchFingers.Finger2 = 0;
                    tTouchFingers.Palm2 = 0;
                    break;
                case 1:
                    tTouchFingers.Finger2 = 0;
                    tTouchFingers.Palm2 = 0;
                    break;
                case 2:
                    tTouchFingers.Palm2 = 1;
                    tTouchFingers.Finger2 = 0;
                case 3:
                    tTouchFingers.Palm2 = 0;
                    tTouchFingers.Finger2 = 1;
            }
            tTouchFingers.X2 = x_2;
            tTouchFingers.Y2 = y_2;

            switch (flag3)
            {
                case 0:
                    tTouchFingers.Finger3 = 0;
                    tTouchFingers.Palm3 = 0;
                    break;
                case 1:
                    tTouchFingers.Finger3 = 0;
                    tTouchFingers.Palm3 = 0;
                    break;                   
                case 2:
                    tTouchFingers.Palm3 = 1;
                    tTouchFingers.Finger3 = 0;
                case 3:
                    tTouchFingers.Palm3 = 0;
                    tTouchFingers.Finger3 = 1;
            }
            tTouchFingers.X3 = x_3;
            tTouchFingers.Y3 = y_3;
            switch (flag4)
            {
                case 0:
                    tTouchFingers.Finger4 = 0;
                    tTouchFingers.Palm4 = 0;
                    break;
                case 1:
                    tTouchFingers.Finger4 = 0;
                    tTouchFingers.Palm4 = 0;
                    break;
                case 2:
                    tTouchFingers.Palm4 = 1;
                    tTouchFingers.Finger4 = 0;
                case 3:
                    tTouchFingers.Palm4 = 0;
                    tTouchFingers.Finger4 = 1;
            }
            tTouchFingers.X4 = x_4;
            tTouchFingers.Y4 = y_4;
            
        }
        if (g_ucFingers==0)
        {
            tTouchFingers.tpKeepFlag = 0;
        }
        // 此处调试
        if (Row  == 171)
        {
            int temp = 0;
        }
        // 取完一行数据，进入函数
        forceSim[Row -1] = TF_Getforce()*10;
        Acall[Row - 1] = t_AcDataGather.AcAll;
        ADCH1[Row - 1] = t_AcDataGather.AcComponentOrig[0];
        ADCH2[Row - 1] = t_AcDataGather.AcComponentOrig[1];
        ADCH3[Row - 1] = t_AcDataGather.AcComponentOrig[2];
        ADCH4[Row - 1] = t_AcDataGather.AcComponentOrig[3];
        Ratio[Row - 1] = g_TFRatio;
        ADSum[Row - 1] = t_AcDataGather.AcComponentOrig[0] + t_AcDataGather.AcComponentOrig[1] + t_AcDataGather.AcComponentOrig[2] + t_AcDataGather.AcComponentOrig[3];
        // TF_Getforce();
        cout <<     "第"<< Row - 1 <<"帧"<<endl ;
        printf("    四通道AD值:\tAD = %d; \tCH1 = %d; \tCH2 = %d; \tCH3 = %d; \tCH4 = %d; \n",t_AcDataGather.AcComponent[0],t_AcDataGather.AcComponentOrig[0],t_AcDataGather.AcComponentOrig[1],t_AcDataGather.AcComponentOrig[2],t_AcDataGather.AcComponentOrig[3]);
        printf("    手指状态： \tg_lThisFingersStatus = %d; \tg_ucFingers = %d;\n",g_lThisFingersStatus,g_ucFingers);
        printf("    滑动状态: \tg_ucTFIsSlide = %d; \tg_lTFIsSlideFinger2 = %d\n",g_ucTFIsSlide,g_lTFIsSlideFinger2);
        printf("    拖拽状态: \tg_lFlagDragFinger1 = %d; \tg_lFlagDragFinger2 = %d;\n",g_lFlagDragFinger1,g_lFlagDragFinger2);
        cout <<"    双击状态:   " <<   g_sFastPresCount << endl;
        printf("    系数: \tg_TFRatio = %f;\n",g_TFRatio);      
        printf("    计算积分: \tt_AcDataGather.AcAll = %.2f;\n", t_AcDataGather.AcAll);
        cout<<"     力度: g_ulTF_force =" << forceSim[Row - 1] <<"   forceSoft =" << forceSoft[Row - 1] <<endl;
        printf("\n");

    }
    // 仿真画图
    plotMat(Acall, ADCH1, ADCH2, ADCH3,ADCH4, ADSum,forceSim, forceSoft, Ratio,Row);
    fclose(file);

    return 0;
}
