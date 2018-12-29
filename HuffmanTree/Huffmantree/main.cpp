#define _CRT_SECURE_NO_WARNINGS // fscanf 不警告
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#pragma warning(disable: 4996) // strrev 函数不警告
#define MAXMESSAGE  10000      // 用于测试的message文件的最大长度
 
typedef struct{
	int weight; // 权重
	int parent, lchild, rchild; // 双亲结点、左孩子结点、右孩子结点
}HTNode, *HuffmanTree; 
// 动态分配数组存储哈夫曼树
typedef char ** HuffmanCode;  
// 动态分配数组存储哈夫曼编码表
typedef struct {
	char c_of_code;
	char* code;
}codes;          // 定义结构体存储编码表
codes codelist[30]; // 定义编码表结构体数组
HuffmanTree HT;  // HT huffman树
HuffmanCode HC;  // HC huffman表
char ch[30];
int w[30];       // 存储字符及其权值
int n = 0;       // 存储频率不为0的字符，即需要编码的字符总个数
/* 函数声明 */
void GetFrequency(char* filename,int w[], char ch[], int* n);
void Select(HuffmanTree HT, int i, int *s1, int *s2);
void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int * w, int n,char* codefile);
void WriteTreefile(char *treefile, int n);
void encoding(char* messagefile, char* encodingfile);
void decoding(char* encodingfile, char* decodingfile);

/***************************************************************************************/
/* 注：因为题中没有给“？”“，”等标点符号设定替代的符号，本程序一律用“.”来代表。   */
/*也就是说，经过先编码再解码后的message标点符号将全用“.”表示。                       */
/* 用到的文件：buildtree <frequencyfile> <treefile> <codefile>                         */
/*             encode <messagefile> <encodingfile>                                     */
/*             decode <decodingfile>                                                   */
/***************************************************************************************/
int main(void)
{
	char frequencyfile[32], treefile[32], codefile[32], messagefile[32], encodingfile[32], decodingfile[32];
	printf("输入频率文件的文件名(示例输入: frequency.txt): \n");
	scanf("%s",frequencyfile);
	GetFrequency(frequencyfile,w, ch, &n);
	printf("\n输入存储哈夫曼编码表的文件名（示例输入：codes.txt)：\n");
	scanf("%s",codefile);
	HuffmanCoding(HT,HC,w,n,codefile);
	printf("\n输入存储哈夫曼树的文件名（示例输入：huffmantree.txt)：\n");
	scanf("%s",treefile);
	WriteTreefile(treefile, n);
	printf("\n输入待编码message的文件名（示例输入：message.txt)：\n");
	scanf("%s",messagefile);
	printf("\n输入存储编码的文件名（示例输入：encode.bin)：\n");
	scanf("%s",encodingfile);
	encoding(messagefile, encodingfile);
	printf("\n输入存储解码后文件的文件名（示例输入：decode.txt)：\n");
	scanf("%s",decodingfile);
	decoding(encodingfile, decodingfile);
	return 0;
}

// 读取频率文件
void GetFrequency(char* filename,int w[], char ch[], int* n)
{
	for(int i=0;i<30;i++)
	{
		ch[i] = codelist[i].c_of_code = '\0';
		w[i] = 0;
		codelist[i].code = "\0";
	}  // 初始化存放字符和权值的数组,顺便初始化编码表

	FILE* fp = fopen(filename, "rt");
	if(!fp){
		printf("ERROR! Cannot open %s! ", filename);
			return;
	}  // 文件是否能正常打开
	int i = 0;
	while(!feof(fp))
	{
		for(int j=0;j<30; j++)
		{
			char c = fgetc(fp);
			int a = 0;
		    fscanf(fp, " %d\n", &a);
			if(a!=0){w[i]=a; codelist[i].c_of_code = ch[i] = c; i++;}
		}
	}
	*n = i;    // 获取频率不为0，即需要编码的字符个数
	fclose(fp); // 关闭文件
}
// 辅助函数：寻找权值最小的两个结点的编号
void Select(HuffmanTree HT, int i, int *s1, int *s2)
{
	// 在HT[1...i]的1~i项中选择parent为0且weight最小的两个结点，其序号分别为s1,s2
	// 排序时，若weight相同，lchild及rchild不为0的结点排在无子树的树结点之前
	// 如若都没有子树，则按字母表排序
	int sort[30] = {0}, k=0;
	for(int j=1;j<=i;j++)
		if(HT[j].parent == 0){sort[k] = j; k++;}
		// sort的前k项就是等待排序的编号
	for(int v=0;v<k-1;v++)
		for(int w=0;w<k-1-v;w++)
	{
		int s3=sort[w], s4 = sort[w+1];
		if(HT[s3].weight>HT[s4].weight)
		{
			int m=sort[w];
			sort[w]=sort[w+1];
			sort[w+1]=m;
		}
		else if(HT[s3].weight == HT[s4].weight && HT[s3].rchild == HT[s3].lchild == 0 && HT[s4].lchild != 0)
		{
			int m=sort[w];
			sort[w]=sort[w+1];
			sort[w+1]=m;
		}
		else if(HT[s3].weight == HT[s4].weight && HT[s3].rchild == HT[s3].lchild == HT[s4].rchild == HT[s4].lchild == 0)
		if(ch[s4]<ch[s3])
			{
			int m=sort[w];
			sort[w]=sort[w+1];
			sort[w+1]=m;
		}
	}  // 冒泡法排序完毕
	*s1 = sort[0];
	*s2 = sort[1];
}
// 构建哈夫曼树并编码
void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int * w, int n, char* codefile){
	// w数组存放n个字符的权值（均>0），构造哈夫曼树HT，并求出n个字符的哈夫曼编码HC
	if(n<=1) return;
	int m = 2*n - 1;
	HT = (HuffmanTree)malloc((m+1)*sizeof(HTNode));   // 0号单元不用
	int i;
	for(i=1; i<=n; i++)         // 对叶子结点的初始化
	{
		HT[i].weight = w[i-1];  // 读入权值
		HT[i].parent = 0;       
		HT[i].lchild = 0;
		HT[i].rchild = 0;       // 每个结点均只含一个权值为w[i-1]的根结点，左右子树为空
	}
	for(i=n+1;i<=m;i++)         // 对非叶子结点的初始化
	{
		HT[i].weight = 0;
		HT[i].parent = 0;
		HT[i].lchild = 0;
		HT[i].rchild = 0;
	}
   // 初始化HT表

	for(i=n+1;i<=m;++i)         // 建huffman树
	{
		// 在HT[1...i-1]中选择parent为0且weight最小的两个结点，其序号分别为s1,s2
		int s1, s2;
		Select(HT, i-1, &s1, &s2);
		HT[s1].parent = i; HT[s2].parent = i;
	    HT[i].lchild = s1; HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;  // 合并子树s1, s2
	}

    // 树已建成，开始编码
	// ——从叶子到根逆向求每个字符的哈夫曼编码——
	FILE *fpcode = fopen(codefile, "wt");   // 打开文件用于存储编码表
	if(!fpcode)
	{
		printf("ERROR! Can't open %s!", codefile);
		return;
	} // 文件是否正常打开
	HuffmanCode HC = (HuffmanCode)malloc((n+1)*sizeof(char*));  // 分配n个字符编码的头指针向量
	char* cd = (char*)malloc(n*sizeof(char));       // 分配求编码的工作空间
	cd[n-1] = '\0';                                 // 编码结束符
	for(i=1;i<=n;++i){                              // 逐个字符求huffman编码
		int start = n-1, c;                         // 编码结束符位置
		int f;
		for(c=i,f=HT[i].parent; f!=0; c=f, f=HT[f].parent) // 从叶子到根逆向求编码
			if(HT[f].lchild == c) cd[--start] = '0';
			else cd[--start] = '1';
			HC[i] = (char*)malloc((n-start)*sizeof(char)); // 为第i个字符编码分配空间
			codelist[i-1].code = (char*)malloc((n-start)*sizeof(char));
			int j=0;
			for(j=0 ;j < n-1-start; j++) {HC[i][j] = cd[start+j]; codelist[i-1].code[j] = cd[start+j];}
			// 从cd复制编码到HC和codelist
			HC[i][j] = codelist[i-1].code[j] = '\0';
		    fprintf(fpcode, "%c %s\n", ch[i-1], HC[i]);
	}
	free(cd);    // 释放工作空间
	fclose(fpcode); // 写入完毕，关闭文件
} // HuffmanCoding
// 将哈夫曼树表写入文件
void WriteTreefile(char *treefile, int n)
{
	FILE *fptree = fopen(treefile, "wt");
	if(!fptree){
		printf("ERROR! Cannot open %s! ", treefile);
			return;
	}  // 文件是否能正常打开
	for(int i=1;i<2*n-1; i++)
	{
		fprintf(fptree, "%d %d %d %d %d\n", i, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild);
	}
	fclose(fptree);
}
// 读入待编码文件并进行编码
void encoding(char* messagefile, char* encodingfile)
{
	FILE *fpm = fopen(messagefile, "rt");
	FILE *fpe = fopen(encodingfile, "wb");
	if(!fpm){
		printf("ERROR! Cannot open %s! ", messagefile);
			return;
	}  // 文件是否能正常打开
	if(!fpe){
		printf("ERROR! Cannot open %s! ", encodingfile);
			return;
	}  // 文件是否能正常打开
	char message[1024] = "\0"; // 定义一个字符串用来存储message
	int i = 0;
	while(!feof(fpm))
	{
		char c = fgetc(fpm);
		if(c==' ') c = '-';
		else if(c=='\n') c = '!';
		else if(c<'a'&&c>'z'&&c<'A'&&c>'Z') c = '.';
		else if(c == EOF) c = '+';
		message[i++] = c;
	}                   //‘-’ for space, ‘.’ for period, ‘!’ for new line, and ‘+’ for end-of-message. 
	message[i] = '\0';  // message读取完毕，添加结束符
	fclose(fpm);        // 关闭message文件

	char codes[10000] = "\0";  // 定义字符串用来存储编码后的文件
	int start = 0;
	for(int j=0;j<i;j++)
	{
		int k = 0, l = 0;
		while(message[j] != ch[k])k++;
		int nlen = strlen(codelist[k].code);
		for(int l = 0; l<nlen&&codelist[k].code[l]!='\0'; l++)codes[start++] = codelist[k].code[l];
	}
	codes[start] = '\0';       // codes读取完毕，添加结束符

	for(; start>=8; start-=8)  // 8位8位地写入encode.bin
	{
		char c = '\0';
		for (int i = 0; i<8; i++)
		{
			if (codes[i] == '1')c = (c << 1) | 1;  // 如果是1，左移一位写为1
			else  c = c << 1;                      // 否则继续左移
		}             
		fwrite(&c, sizeof(char), 1, fpe);          // 将得到的一个完整byte写入bin
		strcpy(codes, &codes[8]);                  // 右移8位，重复以上操作直到剩余长度不足8位
			// start -= 8;
	}
	if (start>0)   // 剩余长度不足8位时，用0补齐高位
	{
		char c = '\0';
		int i;
		for (i = 0; i<start; i++)
		{
			if (codes[i] == '1') c = (c << 1) | 1;
			else  c = c << 1; 
		}
		for (; i < 8; i++) c = c << 1;     // 余位全部置0
		fwrite(&c, sizeof(char), 1, fpe);  // 将凑足后的8位写入bin
	}   // 利用位操作完成二进制文件的写入
	fclose(fpe);  // 写入完成，关闭文件
}
// 将二进制文件解码为文本文件
void decoding(char* encodingfile, char* decodingfile)
{
	FILE *fpe = fopen(encodingfile, "rb");
	if(!fpe){
		printf("ERROR! Cannot open %s! ", encodingfile);
		return;
	}  // 文件是否能正常打开
	FILE *fpd = fopen(decodingfile, "wt");
	if(!fpd){
		printf("ERROR! Cannot open %s! ", decodingfile);
		return;
	}  // 文件是否能正常打开

	char code[100000] = "\0";   // 定义一个字符串用于存储编码
	char decode[10000] = "\0";  // 定义一个字符串用来存储解码后得到的message
	char ch[10000] = "\0";      // 定义一个字符串用来存储二进制文件中的内容
	for(int i=0;i<10000;i++) ch[i] = '!';
	fread(&ch,sizeof(char), 10000, fpe);  // 读取二进制文件
	int nlen_ch = 0;
	while(ch[nlen_ch] != '!' )nlen_ch++;
	ch[nlen_ch] = '\0';                   // 截掉字符串ch多出的空间
	for(int i=0; i<nlen_ch; i++)
	{
		char bin[9]= "\0";
		for(int j=0; j<8; j++) _itoa(ch[i]>>j&1, &bin[j], 10);  // 读取二进制码，并以字符串形式存于bin中
		strrev(bin);       // 字符串反转，二进制码读取完毕
		strcat(code, bin); // 添加到编码字符串
	}                      // 将转码记录入code数组
	fclose(fpe);           // 读取待解码文件完毕，关闭文件
	for (int i=0; i<MAXMESSAGE; i++)  // 下面开始解码
	{
		int j = 0;
		for (; j<n; j++)
		{
			char* dist = strstr(code, codelist[j].code); // 获得码在code中的位置
			if (dist == code)break;            // 如果指向首位置不再寻找
		}                                      // 以上循环找到了code字符串的第一个码
		switch(codelist[j].c_of_code)
		{
			case '+': fprintf(fpd, "\0"); fclose(fpd); return;
			case '-': fprintf(fpd,  " "); break;
			case '!': fprintf(fpd, "\n"); break;
			case '.': fprintf(fpd, ","); break;
			default :  fprintf(fpd, "%c", codelist[j].c_of_code);
		} // 将字符写入decode文件
		int len_lastcode = strlen(codelist[j].code);
		strcpy(code, &code[len_lastcode]);         // 右移至下一个码的起点，重复进行操作
	}
	fclose(fpd);  // 解码完成，关闭文件
}
