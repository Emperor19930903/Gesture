#include"plot_matlab.h"
#include <engine.h>  
#include<iostream>
void plotMat(vector<uint32_t>& AcAll, vector<int32_t>& ADCH1, vector<int32_t>& ADCH2, vector<int32_t>& ADCH3,
    vector<int32_t>& ADCH4, vector<int32_t>& ADSum, vector<uint32_t>& forceSim, vector<uint32_t>& forceSoft, vector<float>& Ratio, int Row)
{
    double* AcAll0 = new double[Row]; double* forceSim0 = new double[Row]; double* forceSoft0 = new double[Row]; double* Ratio0 = new double[Row];
    double* ADCH10 = new double[Row]; double* ADCH20 = new double[Row]; double* ADCH30 = new double[Row]; double* ADCH40 = new double[Row];
    double* ADSum0 = new double[Row];
    
    for (int i=0;i< Row; i++)
    {
        AcAll0[i] = AcAll[i];
        forceSim0[i] = forceSim[i];
        //cout << forceSim0[i] << endl;
        forceSoft0[i] = forceSoft[i];
        ADCH10[i] = ADCH1[i]; ADCH20[i] = ADCH2[i]; ADCH30[i] = ADCH3[i]; ADCH40[i] = ADCH4[i];
        ADSum0[i] = ADSum[i];
        Ratio0[i] = Ratio[i];
    }
    
    Engine* epSim;
    
    // open engine
    if (!(epSim = engOpen("\0")))
    {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
    }
    // AD原始信号
    // CH1
    mxArray* TADCH10 = NULL;
    TADCH10 = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TADCH10), (void*)ADCH10, Row * sizeof(double));
    // CH2
    mxArray* TADCH20 = NULL;
    TADCH20 = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TADCH20), (void*)ADCH20, Row * sizeof(double));
    // CH3
    mxArray* TADCH30 = NULL;
    TADCH30 = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TADCH30), (void*)ADCH30, Row * sizeof(double));
    // CH4
    mxArray* TADCH40 = NULL;
    TADCH40 = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TADCH40), (void*)ADCH40, Row * sizeof(double));
    // Sum
    mxArray* TADSum0 = NULL;
    TADSum0 = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TADSum0), (void*)ADSum0, Row * sizeof(double));
    //AcAll
    mxArray* TAcAll = NULL;
    TAcAll = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TAcAll), (void*)AcAll0, Row * sizeof(double));
    //ForceSim
    mxArray* TSim = NULL;
    TSim = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TSim), (void*)forceSim0, Row * sizeof(double));
    //ForceSoft
    mxArray* TSoft = NULL;
    TSoft = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TSoft), (void*)forceSoft0, Row * sizeof(double));
    //Ratio
    mxArray* TRatio = NULL;
    TRatio = mxCreateDoubleMatrix(Row, 1, mxREAL);
    memcpy((void*)mxGetPr(TRatio), (void*)Ratio0, Row * sizeof(double));
    delete []AcAll0;
    delete []forceSim0;
    delete []forceSoft0;
    delete[]ADCH10; delete[]ADCH20; delete[]ADCH30; delete[]ADCH40;
    delete[]Ratio0;
    //画图-4通道波形
    engPutVariable(epSim, "TADCH10", TADCH10);         // put data to engine
    engEvalString(epSim, "close all;");
    engEvalString(epSim, "figure(1);");
    engEvalString(epSim, "set(gcf, 'Position', [50, 300, 1500, 600]);");
    engEvalString(epSim, "Y=TADCH10;");
    engEvalString(epSim, "plot(Y);");
    engEvalString(epSim, "hold on;"); 

    engPutVariable(epSim, "TADCH20", TADCH20); engEvalString(epSim, "Y=TADCH20;");engEvalString(epSim, "plot(Y);");
    engPutVariable(epSim, "TADCH30", TADCH30); engEvalString(epSim, "Y=TADCH30;"); engEvalString(epSim, "plot(Y);");
    engPutVariable(epSim, "TADCH40", TADCH40); engEvalString(epSim, "Y=TADCH40;"); engEvalString(epSim, "plot(Y);");
    engPutVariable(epSim, "TADSum0", TADSum0); engEvalString(epSim, "Y=TADSum0;"); engEvalString(epSim, "plot(Y);");
    engEvalString(epSim, "legend('CH1','CH2','CH3','CH4','Sum');");
    engEvalString(epSim, "title('四通道波形');");
    // 画积分
    engPutVariable(epSim, "TAcAll", TAcAll);         // put data to engine
    engEvalString(epSim, "figure(2);");
    engEvalString(epSim, "set(gcf, 'Position', [50, 300, 1500, 600]);");
    engEvalString(epSim, "Y=TAcAll;");
    engEvalString(epSim, "plot(Y,'b.-');");
    engEvalString(epSim, "title('积分');");
    // 画系数
    engPutVariable(epSim, "TRatio", TRatio);         // put data to engine
    engEvalString(epSim, "figure(3);");
    engEvalString(epSim, "set(gcf, 'Position', [50, 300, 1500, 600]);");
    engEvalString(epSim, "Y=TRatio;");
    engEvalString(epSim, "plot(Y,'b.-');");
    engEvalString(epSim, "title('系数');");
    // 仿真力度
    engPutVariable(epSim, "TSim", TSim);         // put data to engine
    engEvalString(epSim, "figure(4);");
    engEvalString(epSim, "set(gcf, 'Position', [50, 300, 1500, 600]);");
    engEvalString(epSim, "Y=TSim;");
    engEvalString(epSim, "plot(Y,'b.-');");
    engEvalString(epSim, "hold on;");
    // 软件力度
    engPutVariable(epSim, "TSoft", TSoft);         // put data to engine
    engEvalString(epSim, "Y=TSoft;");
    engEvalString(epSim, "plot(Y,'r.-');");
    engEvalString(epSim, "title('仿真力度和软件力度对比');");
    engEvalString(epSim, "xlabel('Pos');");
    engEvalString(epSim, "ylabel('Force');");
    engEvalString(epSim, "legend('Sim','Soft');");
    printf("Hit return to continue\n");
    fgetc(stdin);
    mxDestroyArray(TSim);
    engEvalString(epSim, "close;");

    // close engine
    engClose(epSim);
}