


#include "force_process.h"
#include "ad_dc_calculate.h"
#include "adc_process.h"
extern  TF_PARAMETER g_tTFParaMeter;
extern AcDataGather_T t_AcDataGather;
extern ADCCllect_T ADCCllect;
void ad_dc_calculate(void)
{
  int i;

  for(i = 0; i < CHANNEL; i++)
  {
    #ifdef WAVE_DIR_REVERSE      //如果电路是反向信号，则需要取反
        t_AcDataGather.AcComponent[i] = (ADCCllect.Value[i] - g_tTFParaMeter.g_lDCComponent[i]) * (0 - 1);
        t_AcDataGather.AcComponentOrig[i] = (ADCCllect.Value[i] - g_tTFParaMeter.g_lDCComponent[i]) * (0 - 1);
    #else
        t_AcDataGather.AcComponent[i] = ADCCllect.Value[i] - g_tTFParaMeter.g_lDCComponent[i];   //减去基准值
        t_AcDataGather.AcComponentOrig[i] = ADCCllect.Value[i] - g_tTFParaMeter.g_lDCComponent[i];
	#endif
  }

  //20191014	V5.01 赵艳红两通道相加
  t_AcDataGather.AcComponent[0] = 	t_AcDataGather.AcComponent[0] + t_AcDataGather.AcComponent[1] + t_AcDataGather.AcComponent[2] + t_AcDataGather.AcComponent[3];
  t_AcDataGather.AcComponent[1] = 	t_AcDataGather.AcComponent[0];
  t_AcDataGather.AcComponent[2] = 	t_AcDataGather.AcComponent[0];
  t_AcDataGather.AcComponent[3] = 	t_AcDataGather.AcComponent[0];
}
