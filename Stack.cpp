/* ��ջ�ķ�ʽʵ�ֵݹ飺�׳˼��� */
#include<stdio.h>
#include<malloc.h>

struct SNode{
	int num;      // �������
	int lastnum;  // ��һ�㷵�ص�ַ
	struct SNode* Next;
};

typedef struct SNode* Stack;  // ������

Stack CreateStack()
{
	Stack S;
	S = (Stack)malloc(sizeof(struct SNode));
	S->Next = NULL;
	return S;
}                            // ����һ����ջ��ͷ��㣬����ָ�� 

int IsEmpty(Stack S)
{
	return(S->Next == NULL);
}                           // �ж϶�ջ�Ƿ�Ϊ��

void Push(int num, int lastnum,  Stack S)
{
	struct SNode*TmpCell;
	TmpCell = (struct SNode*)malloc(sizeof(struct SNode));
	TmpCell->num = num;
	TmpCell->lastnum = lastnum;
	TmpCell->Next = S->Next;
	S->Next = TmpCell;
}                           // ��numѹ���ջ

int Pop(Stack S)
{
	struct SNode*FirstCell;
	int TopElem;
	if(IsEmpty(S)){
		printf("��ջ��");
		return NULL;
	}
	else {
		FirstCell = S->Next;
		S->Next = FirstCell->Next;
		TopElem = FirstCell->num;
		free(FirstCell);
		return TopElem;
	}
}                           // ����ջ����num

int main (void)
{
	int n, result = 1;
	printf("��ջ�ķ�ʽʵ�ּ���׳� n! \n");
	printf("������������n��n����Ϊ����������");
	scanf("%d",&n);
	printf("\n�������ǣ�");
	Stack S = CreateStack();
	Push(n, n+1, S);
	while(n-- != 1) Push(n,n+1,S);           // ��ջ
	while(IsEmpty(S) == 0) result *= Pop(S); // ��ջ
	printf("%d",result);
	return 0;
}