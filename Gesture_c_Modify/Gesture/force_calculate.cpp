
#include "force_process.h"
#include "force_calculate.h"

extern AcDataGather_T t_AcDataGather;
extern  TF_PARAMETER g_tTFParaMeter;

//力度计算
void force_calculate(void)
{
    float force_temp = 0;

    /**************** 单指 *******************/
    if (g_lThisFingersStatus == FINGER_1 || g_lThisFingersStatus == FINGER_2)
    {
        if (g_lFlagDragFinger1 == 0 && g_lFlagDragFinger2 == 0)  //非拖拽
        {
            g_ulTF_force = t_AcDataGather.AcAll * g_TFRatio / (g_tTFParaMeter.g_lForceDivider * 0.0001f);
        }
        else    //是拖拽，保持
        {
            //一指按，另一指滑，第一指抬起，应该释放 ||  14寸一体式：一指滑，另一指按，按的手指抬离，应该掉落_20221012
            if (gesture_1fingerleave_2fingertouch == 1 || gesture_1fingertouch2fingerleave == 1)
            {
                if (gesture_1fingerleave_2fingertouch == 1) gesture_1fingerleave_2fingertouch = 0;

                if (gesture_1fingertouch2fingerleave == 1) gesture_1fingertouch2fingerleave = 0;

                g_ulTF_force = 0;
            }

            else
            {
                force_temp = t_AcDataGather.AcAll * g_TFRatio / (g_tTFParaMeter.g_lForceDivider * 0.0001f);

                if (g_tTFParaMeter.DragForceLock == 1)  //拖拽力为锁定状态
                {
                    if (force_temp < g_tTFParaMeter.DragForce)     //对0~100之前给出大于100的假值	两指的马达释放阈值比单指大
                    {
                        g_ulTF_force = random(g_tTFParaMeter.DragForce, g_tTFParaMeter.DragForce + 80);
                    }

                    else
                    {
                        g_ulTF_force = force_temp;
                    }
                }
                else  //拖拽力为非锁定状态
                {
                    g_ulTF_force = force_temp * g_tTFParaMeter.DragForceReleaseRate;
                }
            }
        }
    }

    /************ 双指 *****************/
    else if (g_lThisFingersStatus == (FINGER_1 | FINGER_2))
    {
        if (g_lFlagDragFinger1 == 0 && g_lFlagDragFinger2 == 0)
        {
            if (t_AcDataGather.AcComponent[g_lTFCh] > 0 && t_AcDataGather.AcComponentLast[g_lTFCh] > 0 && t_AcDataGather.AcComponent[g_lTFCh] > t_AcDataGather.AcComponentLast[g_lTFCh] && g_tForceTriggerStatus == FORCE_NOTRIGGER)
            {
                g_ulTF_force = t_AcDataGather.AcAll * g_TFRatio / (g_tTFParaMeter.g_lForceDivider * 0.0001f);
            }
            else
            {
                g_ulTF_force = t_AcDataGather.AcAll * g_TFRatio / (g_tTFParaMeter.g_lForceDivider * 0.0001f);
            }
        }

        else//是拖拽，保持
        {
            force_temp = t_AcDataGather.AcAll * g_TFRatio / (g_tTFParaMeter.g_lForceDivider * 0.0001f);
            if (force_temp < g_tTFParaMeter.DragForce)//对0~100之前给出大于100的假值	两指的马达释放阈值比单指大
            {
                g_ulTF_force = random(g_tTFParaMeter.DragForce, g_tTFParaMeter.DragForce + 80);
            }
            else
            {
                g_ulTF_force = force_temp;
            }
        }
    }
    else//更多手指
    {
        g_ulTF_force = t_AcDataGather.AcAll * g_TFRatio / (g_tTFParaMeter.g_lForceDivider * 0.0001f);
    }

#if SINGLEBYTE_FORCE_UPDATE
    if (g_ulTF_force > 255)  g_ulTF_force = 255;
#else
    if (g_ulTF_force > 500)  g_ulTF_force = 500;
#endif
}



//生成随机

int random(int a, int b) //a:最小值，b:最大值
{
    int Rand_val = 0;
    Rand_val = rand() % (b - a + 1) + a;
    return Rand_val;
}
