/***********************************************************************************************************
*   2)基于一个文本输入文件input.txt， 对其进行行程编码后，输出结果到output.txt文件，比较两个文件前后的大小 *
* 差异:并将output.txt解码后还原成文件output2.txt，比较input.txt和output2.txt的差异，看看通过编解码后，信息 *
* 有没有发生改变。                                                                                         *
************************************************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<sys/stat.h>
#define N 500 //每个字符重复的最大次数，可根据用户要求和运行速度要求修改其大小，调试时设为500

// 函数声明
void Input(char input[]); 
void encoding(char input[], char output[]);
void decoding(char output[], char rev_output[]);
int file_size(char* filename);
void file_size_cmp(char* filename1, char* filename2);
char Character[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };

int main(void)
{
	char input[32] = "\0", output[32] = "\0", rev_output[32] = "\0";
	printf("Please input the input filename: (文件是随机生成的，您可以输入input.txt进行测试)\n");
	scanf("%s", input);
	Input(input);
	printf("\nPlease input the output filename: (您可以输入output1.txt进行测试)\n");
	scanf("%s", output);
	encoding(input, output);
	printf("\nPlease input the reverse output filename: (您可以输入output2.txt进行测试)\n");
	scanf("%s", rev_output);
	decoding(output, rev_output);
	file_size_cmp(input, rev_output);
	system("pause");
	return 0;
}

// 随机生成input文件
void Input(char input[])
{
	FILE*fp = fopen(input, "w+");
	if (!fp) {
		printf("Error!can not open %s!\n", input);
		exit(EXIT_FAILURE);
	}

	srand((unsigned)time(NULL));//通过把time函数的返回值传递给srand,避免相邻两次随机生成的字母相同

	char ch;
	int num = rand() % N + 1;//随机生成最大“对数”。定义形如1W，56B为“一对”。

	for (int i = 0; i < num; i++) 
	{
		int n_word = rand() % 26;
		ch = Character[n_word];//生成完毕
		int count = rand() % N + 1;
		for (int j = 0; j < count; j++) fprintf(fp,"%c",ch);
	}
	fclose(fp);
}
// 对input编码，生成结果文件output
void encoding(char input[], char output[])
{
	FILE*fp3 = fopen(input, "rt");
	FILE*fp4 = fopen(output, "wt");
	if (!fp3) {
		printf("Error!can not open %s!\n", input);
		exit(EXIT_FAILURE);
	}
	if (!fp4) {
		printf("Error!can not %s!\n",output);
		exit(EXIT_FAILURE);
	} //打开input和output文件

	char s_input[N*N] = "\0", s_output[N*N] = "\0";
	fscanf(fp3, "%s", s_input);   // 将input文件写入字符串，问题即转化为第一小题
	char ch[2] = "\0", cnt[4] = "\0", *p1 = s_input; // 存储字符和字符出现的次数
	while(*p1 != '\0')
	{
		int count = 1;
		ch[0] = *p1;
		while(*++p1 == ch[0]) count++;
		_itoa(count, cnt, 10);
		strcat(s_output, cnt);
		strcat(s_output, ch);
	}
	fprintf(fp4, "%s", s_output);
	fclose(fp3);
	fclose(fp4);
}
// 对output解码，生成解码文件rev_output
void decoding(char output[], char rev_output[])
{
	FILE*fp1 = fopen(output, "rt");
	FILE*fp2 = fopen(rev_output, "wt");

	if (!fp1) {
		printf("Error!can not open %s!\n", output);
		exit(EXIT_FAILURE);
	}
	if (!fp2) {
		printf("Error!can not open %s!\n", rev_output);
		exit(EXIT_FAILURE);
	}//打开output和rev_output文件
	
	char s_output[N*N] = "\0", s_rev_output[N*N] = "\0"; 
	fscanf(fp1, "%s",s_output);
	char* p2 = s_output, ch[2] = "\0", cnt[4] = "\0"; // ch和cnt用来存储字符及其出现的次数
	while(*p2 != '\0')
	{
		int i = 0;
		ch[0] = *p2;
		while(ch[0]>= '0' && ch[0]<='9') 
		{
			cnt[i++] = ch[0]; 
			ch[0] = *++p2;
		} // 到此处该字符出现的次数已存入cnt中，现在ch中存储的是该字符
		cnt[i] = '\0';
		int count = atoi(cnt); // 将字符串转换成整形数
		for(int i = 0; i<count; i++) strcat(s_rev_output, ch); // 将count个字符写入string of reverse output
		p2++;
	} 
	fprintf(fp2,"%s",s_rev_output); // 将解码结果写入rev_output
	fclose(fp1);
	fclose(fp2); // 解码完成,关闭文件
}
// 辅助函数：获取文件大小
int file_size(char* filename)
{
	struct stat statbuf;
	stat(filename, &statbuf);
	return statbuf.st_size;
}
// 辅助函数： 比较文件大小,并打印文件大小等提示信息
void file_size_cmp(char* filename1, char* filename2)
{
	int size1 = file_size(filename1);
	int size2 = file_size(filename2);
	printf("%s的大小为：%d\n%s的大小为：%d\n",filename1, size1, filename2, size2);
	if(size1 == size2) printf("成功！两文件大小相等\n");
}