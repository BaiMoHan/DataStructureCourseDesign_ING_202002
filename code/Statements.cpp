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
	p->kind=returnnode;	//设置节点类型为returnnode 
	p->child=NULL;	//初始化孩子节点 
	p->sibling=NULL; //初始化兄弟节点
	index++;	//读取下一个单词
	p->child=Expression();	//return后面肯定为表达式
	if(errorflag)
		return NULL;
	else
		return p;	//返回p 
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
	 	printf("\nError:Expected a ';';\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//返回NULL 
	  } 
 } 
/*********************************************************
函数功能：处理continue语句，并返回根节点地址
********************************************************/
syntaxtree Lexer::ContinueState()
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
	p->kind=continuenode;//设置节点识别码为continuenode
	index++;	//取一下词
	if(tokenlist[index].tokentype==SEMI)	//正确状况下,下一个词是分号
	{
		index++;	//索引自增
		return p;	//返回根节点 
	 } 
	 else	//不是分号，则是错误
	 {
	 	errorflag=1;	//设置错误标记 
	 	printf("\nError:Expected a ';';\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//返回NULL 
	  } 
 }  
/*****************************************************************
函数功能：处理for语句
*****************************************************************/
syntaxtree Lexer::ForState()
{
	syntaxtree p=new syntaxnode;	//为复合语句节点申请空间
	syntaxtree q=NULL;	//中间指针
	syntaxtree t=NULL;	//中间指针 
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->child=NULL;	//初始化孩子节点
	p->sibling=NULL;//初始化兄弟节点
	p->kind=fornode;//节点类型为fornode
	index++;	//读取for后面的下一个词
	if(tokenlist[index].tokentype==BRACKETL)	//如果是（,就是正确
	{
		index++;	//读取（后的一个词
		q=new syntaxnode;	//为forstart节点申请内存 
		if(q==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		q->child=NULL;	//初始化孩子节点
		q->sibling=NULL;//初始化兄弟节点
		q->kind=forstart;//设置节点类型为forstart
		p->child=q;	//forstart节点为for节点的第一个孩子 
		if(tokenlist[index].tokentype!=SEMI)//如果不是for(;XXX;XXX)形式
		{
			q->child=Expression();//调用表达式语句处理程序
			if(errorflag)	//如果遇到错误 
				return NULL;
			if(tokenlist[index].tokentype!=SEMI)	//如果不是因为分号退出的
			{
				errorflag=1;	//错误标记
				printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
				return NULL;//返回NULL  
			 } 
		 } 
		index++;	//索引自增,读取;后的下一个词
		t=new syntaxnode;	//为forjudge节点申请内存 
		if(t==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		t->child=NULL;	//初始化孩子节点
		t->sibling=NULL;//初始化兄弟节点
		t->kind=forjudge;//设置节点类型为forjudge
		q->sibling=t;	//forjudge节点为forstart节点的兄弟 
		q=q->sibling;	//q指针移动到forjudge节点上 
		if(tokenlist[index].tokentype!=SEMI)//如果不是for(;;XXX)形式
		{
			t->child=Expression();//调用表达式语句处理程序
			if(errorflag)	//如果遇到错误 
				return NULL;
			if(tokenlist[index].tokentype!=SEMI)	//如果不是因为分号退出的
			{
				errorflag=1;	//错误标记
				printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
				return NULL;//返回NULL  
			 } 
		 } 
		index++;	//索引自增,读取;后的下一个词
		t=new syntaxnode;	//为forchange节点申请内存 
		if(t==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		t->child=NULL;	//初始化孩子节点
		t->sibling=NULL;//初始化兄弟节点
		t->kind=forchange;//设置节点类型为forchange
		q->sibling=t;	//forchange节点为forjudge节点的兄弟 
		q=q->sibling;	//q指针移动到forchange节点上 
		if(tokenlist[index].tokentype!=BRACKETR)//如果不是for(XX;XX;)形式
		{
			t->child=Expression();//调用表达式语句处理程序
			if(errorflag)	//如果遇到错误 
				return NULL;
			if(tokenlist[index].tokentype!=BRACKETR)	//如果不是因为)退出的
			{
				errorflag=1;	//错误标记
				printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
				return NULL;//返回NULL  
			 } 
		 } 
		index++;	//索引自增
		t=new syntaxnode;	//为forstate节点申请内存 
		if(t==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		t->child=NULL;	//初始化孩子节点
		t->sibling=NULL;//初始化兄弟节点
		t->kind=forstate;//设置节点类型为forstate
		q->sibling=t;	//forstate节点为forchange节点的兄弟节点
		t->child=Statement();	//for子句调用语句处理函数
		if(errorflag)	//如果遇到错误
		{
			printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
			return NULL;//返回NULL  
		 } 
		return p;	//返回p节点地址 
	 } 
	else	//for后面不是跟着（,就是错误
	{
		errorflag=1;	//错误标记
		printf("\nError:Expected () after for;\nLocaterd on line No.%d\nnear chararctor '%s' \n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//返回NULL 	 
	 } 
 } 
/**************************************************************
函数功能：处理if语句
*****************************************************************/
syntaxtree Lexer::IfState()
{
	syntaxtree p=new syntaxnode;	//为复合语句节点申请空间
	syntaxtree q=NULL;	//中间指针
	syntaxtree t=NULL;	//中间指针 
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->child=NULL;	//初始化孩子节点 
	p->sibling=NULL; //初始化兄弟节点
	p->kind=ifnode;//设置节点识别码为ifnode
	index++;	//取一下词
	if(tokenlist[index].tokentype==BRACKETL)	//if后紧跟着条件句
	{
		t=new syntaxnode;	//为条件语句节点申请内存 
		if(t==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		t->child=NULL;//初始化孩子节点
		t->sibling=NULL;//初始化兄弟节点
		t->kind=ifjudge;//节点类型为if条件句
		index++;	//进入表达式程序时index指向括号后的一位 
		t->child=Expression();//条件节点的孩子调用表达式处理函数
		if(errorflag||t->child==NULL)	//判断if条件句是否合法
		{
			errorflag=1;	//错误标记置1
			printf("\nError:Expected correct if condition expression;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
			return NULL;		 
		 } 
		 p->child=t;	//条件节点作为if节点的孩子
		if(tokenlist[index].tokentype==SEMI)//检查条件句退出是否是因为出现分号
		{
			errorflag=1;	//错误标记置1
			printf("\nError:wrong semi used;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
			return NULL;		  
		 } 
		 index++;	//读取下一个词，开始判断if子句部分,expression出来时指向;或者） 
		q=new syntaxnode;	//为if子句节点申请内存
		if(q==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		q->child=NULL;	//初始化孩子节点
		q->sibling=NULL;//初始化兄弟节点
		q->kind=ifcmsd;	//节点类型为if子句
		t->sibling=q;	//if子句节点为if节点的孩子,条件句的兄弟节点 
		if(tokenlist[index].tokentype==LP)	//遇到花括号{
		{
			q->child=CompoundStmd();	//调用复合语句处理程序,复合语句处理完后index指向}下一位 
			if(errorflag)	//如果遇到错误
			{
				printf("\nError:Expected correct if statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL;
			 } 
			if(tokenlist[index].tokentype==ELSE)	//遇到if后跟着else
			{
				p->kind=ifelsenode;	//修改p节点的类型为ifeslenode
				t=t->sibling; //t移到if子句节点上
				q=new syntaxnode;	//为else节点申请内存
				if(q==NULL)		//判断空间是否申请成功
				{
					//输出信息 
					printf("内存申请失败！\n内存不够，自动关闭\n");
					getchar();getchar();	//等待用户响应 
					exit(0);
				 } 
				q->child=NULL;	//初始化孩子节点
				q->sibling=NULL;//初始化兄弟节点
				q->kind=elsenode;	//节点类型为else节点
				t->sibling=q;	//else节点为if子句节点的兄弟
				index++;	//读取else的后面一个词语
				q->child=Statement();	//调用语句处理程序,处理else后面的语句作为else节点的孩子
				if(errorflag||q->child==NULL)//如果else后面处理错误
				{
					errorflag=1;	//错误标记置1
					printf("\nError:Expected correct else statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
					return NULL;
				 } 
				else
					return p;	//if-else处理成功返回p节点地址  
			 }	
			else	//if后没有else,单纯的if型 
			{
				return p;//返回p地址 
			 } 
		 } 
		else	//没有遇到花括号{,if子句只有一条
		{
			q->child=Expression();	//调用表达式处理程序，作为if子句的孩子节点
			if(q->child==NULL||errorflag) //判断是否出错
			{
				errorflag=1;//错误标记
				printf("\nError:wrong if statements;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL; 
			 } 
			//if子句调用expreesion没有出错
			if(tokenlist[index].tokentype!=SEMI)	//如果不是因为分号退出的
			{
				printf("\nError:Expected ; to end a statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL;
			 } 
			index++;	//读取下一个词 
			//if子句没有出错，开始分析是否存在else
			if(tokenlist[index].tokentype==ELSE)	//如果if后有else
			{
				p->kind=ifelsenode;	//修改p的类型为ifelse型节点 
				t=t->sibling;	//t指针移到if子句节点上
				q=new syntaxnode;	//为else节点申请内存
				if(q==NULL)		//判断空间是否申请成功
				{
					//输出信息 
					printf("内存申请失败！\n内存不够，自动关闭\n");
					getchar();getchar();	//等待用户响应 
					exit(0);
				 } 
				q->child=NULL;	//初始化孩子节点
				q->sibling=NULL;//初始化兄弟节点
				q->kind=elsenode;	//节点类型为else节点
				t->sibling=q;	//else节点为if子句节点的兄弟
				index++;	//读取else的后面一个词语
				q->child=Statement();	//调用语句处理程序,处理else后面的语句作为else节点的孩子
				if(errorflag||q->child==NULL)//如果else后面处理错误
				{
					errorflag=1;	//错误标记置1
					printf("\nError:Expected correct else statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
					return NULL;
				 } 
				else
					return p;	//if-else处理成功返回p节点地址 
			 } //处理带else型结束 
			else	//单纯的if型
			{
				return p;//返回节点地址 
			 } 
		 } 
	 } //进入if的（
	else	//if后没有紧跟着（
	{
		errorflag=1;	//错误标记置1
		printf("\nError:Expected if condition charactor '(';Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
		return NULL;		 
	 } 
 } 
/********************************************************************
函数功能：处理函数调用语句,传进来的时候，index指向的还是标识符 
*********************************************************************/
syntaxtree Lexer::CallFunc()
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
	p->kind=callfunc;//设置节点识别码为callfunc
	p->listindex=index;	//保存函数名 
	index=index+2;
	syntaxtree q=p;//保存节点初始地址 
	p->child=Identifier(0);//函数形参调用
	if(errorflag)	//如果出错 
		return NULL; 
	p->child->kind=callmeter;	//调整节点类型 
	p=p->child;	//移到树的下一层，即孩子节点 
	while(tokenlist[index].tokentype==COMMA)	//遇到右括号就退出
	{
		index++;		//过滤逗号 
		p->sibling=Identifier(0);//函数形参调用标识符识别函数
		if(errorflag)	//遇到错误 
			return NULL; 
		p->sibling->kind=callmeter;	//修改节点的类型为callmeter
		while(p&&p->sibling)	//找到最后的兄弟节点 
			p=p->sibling; 	//移到到最后的兄弟节点 	
	 } 
	//上述循环退出来肯定就是遇到了） 
	index++;	//过滤）
	if(tokenlist[index].tokentype==SEMI)	//函数调用后面是分号 
		return q; 
	else
	{
		errorflag=1;	//错误置为1
		printf("Error:Expected a ';';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;
	 } 

 } 

#endif
