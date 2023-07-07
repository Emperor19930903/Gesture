#ifndef _GESTURE_SLIDE_STATE_H
#define _GESTURE_SLIDE_STATE_H


//判断滑动
void gesture_slide_state(void);

// 判断1指是否滑动
void JudgeFinger1Slide(void);

// 判断1指是否静止
void JudgeFinger1Static(void);
short FindMax(short array[], int n);
short FindMin(short array[], int n);

// 判断2指是否滑动
void JudgeFinger2Slide(void);

// 判断2指是否静止
void JudgeFinger2Static(void);


#endif