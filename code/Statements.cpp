#ifndef STATEMENSTS_CPP
#define STATEMENTS_CPP
#include "Lexical_analysis.h"
#include<stack>		//引用栈文件 
using namespace std;
#include<iostream>
/************************************************ 
函数功能：处理return 语句，并返回根节点地址
************************************************/ 
syntaxtree Lexer::ReturnState()
{
	syntaxtree p=new syntaxnode;	//为复合语句节点申请空间
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->child=NULL;	//初始化孩子节点 
	p->sibling=NULL; //初始化兄弟节点
}
/*****************************************************
函数功能：处理break 语句，并返回根节点地址
****************************************************/
syntaxtree Lexer::BreakState()
{
	syntaxtree p=new syntaxnode;	//为复合语句节点申请空间
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->child=NULL;	//初始化孩子节点 
	p->sibling=NULL; //初始化兄弟节点
	p->kind=breaknode;//设置节点识别码为breaknode
	index++;	//取一下词
	if(tokenlist[index].tokentype==SEMI)	//正确状况下,下一个词是分号
	{
		index++;	//索引自增
		return p;	//返回根节点 
	 } 
	 else	//不是分号，则是错误
	 {
	 	errorflag=1;	//设置错误标记 
	 	printf("Error:Expected a ';';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//返回NULL 
	  } 
 } 



#endif
