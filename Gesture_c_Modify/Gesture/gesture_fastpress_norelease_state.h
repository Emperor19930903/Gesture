#ifndef _GESTURE_FASTPRESS_NORELEASE_STATE_H
#define _GESTURE_FASTPRESS_NORELEASE_STATE_H

//快按不释放的处理
void gesture_fastpress_norelease_state(void);


//清除快按数据
void ClearFastPress(void);

// 判断快按
int JudgeFastPress(int n1 , int n2);

//找波峰波谷
int FindPeak(int n1 , int n2);

// 判断符号
int JudgeSign(int n);





#endif