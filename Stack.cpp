/* 用栈的方式实现递归：阶乘计算 */
#include<stdio.h>
#include<malloc.h>

struct SNode{
	int num;      // 本层的数
	int lastnum;  // 上一层返回地址
	struct SNode* Next;
};

typedef struct SNode* Stack;  // 定义结点

Stack CreateStack()
{
	Stack S;
	S = (Stack)malloc(sizeof(struct SNode));
	S->Next = NULL;
	return S;
}                            // 构建一个堆栈的头结点，返回指针 

int IsEmpty(Stack S)
{
	return(S->Next == NULL);
}                           // 判断堆栈是否为空

void Push(int num, int lastnum,  Stack S)
{
	struct SNode*TmpCell;
	TmpCell = (struct SNode*)malloc(sizeof(struct SNode));
	TmpCell->num = num;
	TmpCell->lastnum = lastnum;
	TmpCell->Next = S->Next;
	S->Next = TmpCell;
}                           // 将num压入堆栈

int Pop(Stack S)
{
	struct SNode*FirstCell;
	int TopElem;
	if(IsEmpty(S)){
		printf("堆栈空");
		return NULL;
	}
	else {
		FirstCell = S->Next;
		S->Next = FirstCell->Next;
		TopElem = FirstCell->num;
		free(FirstCell);
		return TopElem;
	}
}                           // 弹出栈顶的num

int main (void)
{
	int n, result = 1;
	printf("用栈的方式实现计算阶乘 n! \n");
	printf("输入您想计算的n（n必须为正整数）：");
	scanf("%d",&n);
	printf("\n计算结果是：");
	Stack S = CreateStack();
	Push(n, n+1, S);
	while(n-- != 1) Push(n,n+1,S);           // 入栈
	while(IsEmpty(S) == 0) result *= Pop(S); // 出栈
	printf("%d",result);
	return 0;
}