#ifndef LEXICAL_ANALYSIS_CPP
#define LEXICAL_ANALYSIS_CPP
#include "Lexical_analysis.h"
#include <stdio.h>
Lexer::Lexer()
{//Lexer�๹�캯�� 
	linecount=0;	//��ʼ���к� 
	vectorindex=0;	//��ʼ��tokenlist���� 
	//��ʾ�û������ļ��� 
	printf("������Ҫ������Դ�����ļ���(������30���ַ�),filename=");
	//��ȡ�û�������ļ��� 
	scanf("%s",filename);
	//������ôʷ�ʶ����ǰ����ʾ��Ϣ 
	printf("��Ҫ������Դ�����ļ�Ϊ%s",filename);
	printf("��ʼ���дʷ�������");
	//���ôʷ�ʶ���� 
	analysis(filename);

 } 
void Lexer::Disp()
{
	printf("��������Ҫ�򿪵��ļ���(������30���ַ�),filename=");
	scanf("%s",filename);
	printf("%s",filename);
}
void Lexer::analysis(char filename[])
{//�ʷ�������Ա���� 
	int state;			//����״̬���� 
	FILE* fp;			//����һ���ļ�ָ�� 
	fp=fopen(filename,"r");//��ֻ���ķ�ʽ��Դ�����ļ� 
	if(fp==NULL)		//�ж��ļ��Ƿ��ʧ�� 
	{
		printf("�ļ���ʧ��,�����´򿪳���");//�ļ���ʧ����ʾ��Ϣ 
		getchar();getchar();				//�ȴ��û������ַ�,������Ϣ��ʾʱ�� 
		exit(-1) ; 				//�رճ��� 
	}
	char ch;					//����һ���ַ��м���� 
	while((ch=fgetc(fp))!=EOF)	//��δ�����ļ�ĩβʱ������״̬�Զ���ʶ�� 
		printf("%c",ch);
}
#endif
