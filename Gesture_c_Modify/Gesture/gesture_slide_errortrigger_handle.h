#ifndef _GESTURE_SLIDE_ERRORTRIGGER_HANDLE_H
#define _GESTURE_SLIDE_ERRORTRIGGER_HANDLE_H

//滑动误触的处理
void gesture_slide_errortrigger_handle(void);//1.34.30 根据赵艳红思想实现抑制滑动误触。主要是在传感器上方的积分

void ReduceSlideTrigger_finger1(void);//1.235 还未判断出滑动时，就有误触，艳红

/* 计算数组最小值?浮点型 */
float FindMin_Float(float array[], int n);
/* ?计算数组最大值?浮点型 */
float FindMax_Float(float array[], int n);

#endif