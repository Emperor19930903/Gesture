#pragma once
// 方案2：不浪费存储空间
// 设置一个变量size，记录队列当中存储元素的个数
// 队尾指针指向 队尾元素的后一个位置（下一个应该插入的位置）

#define MaxSize 20
typedef int ElemType;
typedef struct {
	ElemType data[MaxSize];
	int front;
	int rear;
	int size;	//队列当前长度 
}SeqQueue;

// 初始化队列
void InitQueue(SeqQueue& q)
{
	q.front = q.rear = 0;
	q.size = 0;		//队列当前长度为0
}

// 判断队列是否为空
bool QueueEmpty(SeqQueue& q)
{
	if (q.size == 0)	//队空条件
		return true;
	else
		return false;
}

// 入队
bool EnQueue(SeqQueue& q, ElemType x)
{
	if (q.size == MaxSize)
		return false;		//队列满则报错

	q.data[q.rear] = x;		//将x插入队尾
	q.rear = (q.rear + 1) % MaxSize;    //队尾指针后移
	q.size++;
	return true;
}

// 出队
bool DeQueue(SeqQueue& q, ElemType& x)
{
	if (q.size == 0)
		return false;	//队空则报错
	for (int i = 0; i < q.size-1; ++i)
	{
		q.data[i] = q.data[i + 1];
	}
	x = q.data[q.front];
	q.front = (q.front + 1) % MaxSize; //队头指针后移
	q.size--;
	return true;
}

// 获取队头元素
bool GetHead(SeqQueue& q, ElemType& x)
{
	if (q.size == 0)
		return false;	//队空则报错

	x = q.data[q.front];
	return true;
}

// 队列中元素的个数
int QueueNum(SeqQueue& q)
{
	return q.size;
}
