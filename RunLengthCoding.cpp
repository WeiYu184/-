/***********************************************************************************************************
*   2)����һ���ı������ļ�input.txt�� ��������г̱������������output.txt�ļ����Ƚ������ļ�ǰ��Ĵ�С *
* ����:����output.txt�����ԭ���ļ�output2.txt���Ƚ�input.txt��output2.txt�Ĳ��죬����ͨ����������Ϣ *
* ��û�з����ı䡣                                                                                         *
************************************************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<sys/stat.h>
#define N 500 //ÿ���ַ��ظ������������ɸ����û�Ҫ��������ٶ�Ҫ���޸����С������ʱ��Ϊ500

// ��������
void Input(char input[]); 
void encoding(char input[], char output[]);
void decoding(char output[], char rev_output[]);
int file_size(char* filename);
void file_size_cmp(char* filename1, char* filename2);
char Character[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };

int main(void)
{
	char input[32] = "\0", output[32] = "\0", rev_output[32] = "\0";
	printf("Please input the input filename: (�ļ���������ɵģ�����������input.txt���в���)\n");
	scanf("%s", input);
	Input(input);
	printf("\nPlease input the output filename: (����������output1.txt���в���)\n");
	scanf("%s", output);
	encoding(input, output);
	printf("\nPlease input the reverse output filename: (����������output2.txt���в���)\n");
	scanf("%s", rev_output);
	decoding(output, rev_output);
	file_size_cmp(input, rev_output);
	system("pause");
	return 0;
}

// �������input�ļ�
void Input(char input[])
{
	FILE*fp = fopen(input, "w+");
	if (!fp) {
		printf("Error!can not open %s!\n", input);
		exit(EXIT_FAILURE);
	}

	srand((unsigned)time(NULL));//ͨ����time�����ķ���ֵ���ݸ�srand,������������������ɵ���ĸ��ͬ

	char ch;
	int num = rand() % N + 1;//���������󡰶���������������1W��56BΪ��һ�ԡ���

	for (int i = 0; i < num; i++) 
	{
		int n_word = rand() % 26;
		ch = Character[n_word];//�������
		int count = rand() % N + 1;
		for (int j = 0; j < count; j++) fprintf(fp,"%c",ch);
	}
	fclose(fp);
}
// ��input���룬���ɽ���ļ�output
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
	} //��input��output�ļ�

	char s_input[N*N] = "\0", s_output[N*N] = "\0";
	fscanf(fp3, "%s", s_input);   // ��input�ļ�д���ַ��������⼴ת��Ϊ��һС��
	char ch[2] = "\0", cnt[4] = "\0", *p1 = s_input; // �洢�ַ����ַ����ֵĴ���
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
// ��output���룬���ɽ����ļ�rev_output
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
	}//��output��rev_output�ļ�
	
	char s_output[N*N] = "\0", s_rev_output[N*N] = "\0"; 
	fscanf(fp1, "%s",s_output);
	char* p2 = s_output, ch[2] = "\0", cnt[4] = "\0"; // ch��cnt�����洢�ַ�������ֵĴ���
	while(*p2 != '\0')
	{
		int i = 0;
		ch[0] = *p2;
		while(ch[0]>= '0' && ch[0]<='9') 
		{
			cnt[i++] = ch[0]; 
			ch[0] = *++p2;
		} // ���˴����ַ����ֵĴ����Ѵ���cnt�У�����ch�д洢���Ǹ��ַ�
		cnt[i] = '\0';
		int count = atoi(cnt); // ���ַ���ת����������
		for(int i = 0; i<count; i++) strcat(s_rev_output, ch); // ��count���ַ�д��string of reverse output
		p2++;
	} 
	fprintf(fp2,"%s",s_rev_output); // ��������д��rev_output
	fclose(fp1);
	fclose(fp2); // �������,�ر��ļ�
}
// ������������ȡ�ļ���С
int file_size(char* filename)
{
	struct stat statbuf;
	stat(filename, &statbuf);
	return statbuf.st_size;
}
// ���������� �Ƚ��ļ���С,����ӡ�ļ���С����ʾ��Ϣ
void file_size_cmp(char* filename1, char* filename2)
{
	int size1 = file_size(filename1);
	int size2 = file_size(filename2);
	printf("%s�Ĵ�СΪ��%d\n%s�Ĵ�СΪ��%d\n",filename1, size1, filename2, size2);
	if(size1 == size2) printf("�ɹ������ļ���С���\n");
}