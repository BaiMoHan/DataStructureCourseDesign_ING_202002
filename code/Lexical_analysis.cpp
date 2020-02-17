#ifndef LEXICAL_ANALYSIS_CPP
#define LEXICAL_ANALYSIS_CPP
#include "Lexical_analysis.h"
#include <stdio.h>
Lexer::Lexer()
{//Lexer类构造函数 
	linecount=0;	//初始化行号 
	vectorindex=0;	//初始化tokenlist索引 
	//提示用户输入文件名 
	printf("请输入要分析的源程序文件名(不超过30个字符),filename=");
	//读取用户输入的文件名 
	scanf("%s",filename);
	//输入调用词法识别器前的提示信息 
	printf("需要分析的源程序文件为%s",filename);
	printf("开始进行词法分析：");
	//调用词法识别器 
	analysis(filename);

 } 
void Lexer::Disp()
{
	printf("请输入需要打开的文件名(不超过30个字符),filename=");
	scanf("%s",filename);
	printf("%s",filename);
}
void Lexer::analysis(char filename[])
{//词法分析成员函数 
	int state;			//设置状态变量 
	FILE* fp;			//声明一个文件指针 
	fp=fopen(filename,"r");//以只读的方式打开源程序文件 
	if(fp==NULL)		//判断文件是否打开失败 
	{
		printf("文件打开失败,请重新打开程序");//文件打开失败提示信息 
		getchar();getchar();				//等待用户输入字符,留出信息显示时间 
		exit(-1) ; 				//关闭程序 
	}
	char ch;					//定义一个字符中间变量 
	while((ch=fgetc(fp))!=EOF)	//当未读到文件末尾时，进行状态自动机识别 
		printf("%c",ch);
}
#endif
