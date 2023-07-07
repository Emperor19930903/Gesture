#pragma once
// ����2�����˷Ѵ洢�ռ�
// ����һ������size����¼���е��д洢Ԫ�صĸ���
// ��βָ��ָ�� ��βԪ�صĺ�һ��λ�ã���һ��Ӧ�ò����λ�ã�

#define MaxSize 20
typedef int ElemType;
typedef struct {
	ElemType data[MaxSize];
	int front;
	int rear;
	int size;	//���е�ǰ���� 
}SeqQueue;

// ��ʼ������
void InitQueue(SeqQueue& q)
{
	q.front = q.rear = 0;
	q.size = 0;		//���е�ǰ����Ϊ0
}

// �ж϶����Ƿ�Ϊ��
bool QueueEmpty(SeqQueue& q)
{
	if (q.size == 0)	//�ӿ�����
		return true;
	else
		return false;
}

// ���
bool EnQueue(SeqQueue& q, ElemType x)
{
	if (q.size == MaxSize)
		return false;		//�������򱨴�

	q.data[q.rear] = x;		//��x�����β
	q.rear = (q.rear + 1) % MaxSize;    //��βָ�����
	q.size++;
	return true;
}

// ����
bool DeQueue(SeqQueue& q, ElemType& x)
{
	if (q.size == 0)
		return false;	//�ӿ��򱨴�
	for (int i = 0; i < q.size-1; ++i)
	{
		q.data[i] = q.data[i + 1];
	}
	x = q.data[q.front];
	q.front = (q.front + 1) % MaxSize; //��ͷָ�����
	q.size--;
	return true;
}

// ��ȡ��ͷԪ��
bool GetHead(SeqQueue& q, ElemType& x)
{
	if (q.size == 0)
		return false;	//�ӿ��򱨴�

	x = q.data[q.front];
	return true;
}

// ������Ԫ�صĸ���
int QueueNum(SeqQueue& q)
{
	return q.size;
}
