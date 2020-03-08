#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"
#include<stack>		//引用栈文件 
using namespace std;
#include<iostream>

//void PrintNode(syntaxtree p,int step);	//打印树结点函数前置声明 
status Lexer::Program()	//程序语法分析函数开始
{
	root=new syntaxnode; 
	if(root==NULL)	//判断内存是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	root->kind=rt;	//根节点特殊结点 
	root->child=NULL;//初始化孩子
	root->sibling=NULL;//初始化兄弟结点 
	root->listindex=-1;	//初始化索引 
	syntaxtree p=NULL;	//声明一个中间指针
	syntaxtree q=NULL;	//声明一个中间指针 

	while(tokenlist[index].tokentype==COMMENT)	//过滤掉开头注释
		index++;			//取词索引自增
	
	while(tokenlist[index].tokentype==INCLUDE)	//处理开头的include语句
	{
		if(root->child)					//根节点已经有孩子,说明新的节点在孩子的兄弟上
		{
			p=root->child->child; 	//重新定义移动指针的位置 
			while(p->sibling)	//找到兄弟节点为空的那个节点
				p=p->sibling;	//移动到下一个兄弟节点
			q=new syntaxnode;	//为新结点申请空间 
			p->sibling=q;		//新引用的文件名作为之前引用文件名节点的兄弟
			q->kind=inclufile;	//设置该结点的识别码为inclufile
			q->child=NULL;		//新结点的孩子置空
			q->sibling=NULL;	//新结点的兄弟置空
			q->listindex=++index;//没有报错的情况下,include的后一个就是文件名 
			index++;			//索引自增 
		 } 
		else		//根节点还没有孩子的情况，即第一次遇到include
		{ 
			p=new syntaxnode;	//为include节点申请空间
			p->kind=inclu;					//设置该节点类别码为inclu
			p->sibling=NULL;				//设置兄弟节点为NULL
			p->listindex=index;				//记录在tokenlist中的序列
			q=new syntaxnode;	//为include的孩子申请空间,即引用的文件名
			q->kind=inclufile;				//设置该节点的识别码为inclufile
			q->child=NULL;					//引用文件名不会有孩子,其他文件名作为兄弟
			q->sibling=NULL;				//初始化兄弟节点
			q->listindex=++index;			//没有报错的情况下,include的后一个就是文件名
			root->child=p;					//include节点作为root的第一个孩子
			p->child=q;						//include节点的孩子为按照顺序引用的文件名节点
			index++;		 				//索引自增 
		}	//处理第一次遇到include生成节点情况
		
		while(tokenlist[index].tokentype==COMMENT)	//处理行尾注释
			index++;				//取词索引自增,相当于过滤掉注释
				 
	 } //处理include的while结束 
	 
	p=root->child;	//p指向根节点的孩子 
	while(p&&p->sibling)	//如果根节点有孩子,且兄弟节点不为NULL 
	{
		p=p->sibling;	//p指向该兄弟节点,一直找到最后一个兄弟节点为止 
	 } 
	if(DeclarationList()==OK)		//调用外部声明语句,最后一个兄弟节点的地址作为形参传入 
	{
		printf("\n\n语法分析成功\n"); 
	}
	 
//	 PrintTree(root); 
  
 } 



status Lexer::DeclarationList()	//声明序列语法分析
{ 

	if(tokenlist[index].tokentype==ENDFILE)		//读到程序尾
	{
		return OK;			//返回正确值 
	 } 
	 
	syntaxtree 	p=root->child;	//中间指针p指向根节点的孩子 
	while(p&&p->sibling)	//如果根节点有孩子,且兄弟节点不为NULL 
	{
		p=p->sibling;	//p指向该兄弟节点,一直找到最后一个兄弟节点为止 
	 } 
	 
	if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)//如果是类型声明
	{
		if(tokenlist[index+2].tokentype==BRACKETL)		//如果类型声明之后第二个串是左括号,说明是外部函数相关的
		{
			int i=index;	//中间循环变量 
			while(tokenlist[i].tokentype!=SEMI)		//找分号
			{
				i++;
			 } 
			 
			if(tokenlist[i].linenum==tokenlist[index].linenum)	//如果该行是以分号结尾,说明是函数前置声明
			{
				if(FunctionDeclaration()==OK)	//如果函数声明分析程序返回正确值
				{
					DeclarationList();
				 } 
			 }
			else	//那么就是函数定义,调用函数定义处理函数 
			{
				
			}
		 } 
		else if(tokenlist[index+2].tokentype==SEMI)	//如果类型声明之后的第二个串（标识符）之后的是分号,变量声明 
		{
			
		 } 
	}	//处理类型声明结束
	
	else 
	{
		
	}	
	
 } 
 
 /******************************************************
 函数功能：处理外部函数声明 
 ******************************************************/
 status Lexer::FunctionDeclaration()
 {
 	
 	syntaxtree 	T=root->child;	//中间指针p指向根节点的孩子 
	while(T&&T->sibling)	//如果根节点有孩子,且兄弟节点不为NULL 
	{
		T=T->sibling;	//p指向该兄弟节点,一直找到最后一个兄弟节点为止 
	 } 
 	syntaxtree p=new syntaxnode;	//外部函数声明节点申请空间
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->kind=exfuncdecla;	//该节点的标识码设定唯exfuncdecla
	p->sibling=NULL;		//兄弟节点设为NULL
	T->sibling=p;			//p节点作为形参T的兄弟节点 
	syntaxtree q=new syntaxnode;	//返回值类型节点申请空间
	if(q==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	q->kind=functype;		//设定节点识别码为functype
	q->listindex=index++;	//保存tokenlist中索引
	q->child=NULL;			//函数返回值类型节点无孩子
	p->child=q;				//函数返回值类型节点是外部函数声明节点的孩子
	p=new syntaxnode;		//为函数名节点申请空间
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 }  
	q->sibling=p;	//函数名节点是返回值节点的兄弟节点
	p->kind=funcname;//函数名节点的类别码设为funcname
	p->listindex=index++;//保存tokenlist中的函数名索引,此时index指向（ 
	p->child=NULL;	//函数名节点无孩子
	p->sibling=NULL;//兄弟节点置空 
	if(ParameterList(p)==OK)	//调用函数形参处理函数 
	{
		return OK; 
	}
 }

/***************************************************
函数功能：处理函数形参序列 
****************************************************/
status Lexer::ParameterList(syntaxtree& T)
{
	index++;	//进入的时候是（，此条语句是移动索引到下一位
	if(tokenlist[index].tokentype==BRACKETR) //如果是右括号,说明无形参声明序列
	{
		syntaxtree p=NULL;	//声明中间指针
		p=new syntaxnode;	//为函数形参 
		if(p==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 }  
		p->kind=nonfuncparam;	//设置函数空形参序列节点的识别码为nonfuncparam
		p->child=NULL;		//孩子置空
		p->sibling=NULL;	//兄弟节点置空
		T->sibling=p;		//该节点作为传入形参的兄弟节点
		return OK;			//返回正确值 
 	}
 	else //不是右括号,说明应该是形参序列
	{ 
		syntaxtree p=NULL;	//声明中间指针
		p=new syntaxnode;	//为函数形参 
		if(p==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 }  
		p->kind=funcparam;	//设置函数形参序列节点的识别码为funcparam
		p->child=NULL;		//孩子置空
		p->sibling=NULL;	//兄弟节点置空
		T->sibling=p;		//该节点作为传入形参的兄弟节点
		if(Parameter(p)==OK);			//处理函数形参 
	}
} 
/*******************************************************
函数功能：形参处理函数 
此时index指向的是(的下一位 
*******************************************************/
status Lexer::Parameter(syntaxtree &T)
{
	syntaxtree p=NULL;	//声明中间指针
	p=new syntaxnode;	//为形参具体声明申请空间
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 }  
	if(tokenlist[index].tokentype==CONST)	//如果遇到const类型
	{
		
	 } 
	else //不是const类型 
	{
		if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)//如果是数据类型声明
		{
			if(tokenlist[index+1]==ID) 
			{
				syntaxtree q=NULL;	//声明中间指针
				q=new syntaxnode;	//为具体形参声明节点申请空间
				if(q==NULL)		//判断空间是否申请成功
				{
					//输出信息 
					printf("内存申请失败！\n内存不够，自动关闭\n");
					getchar();getchar();	//等待用户响应 
					exit(0);
				 } 
				q->kind=funcparam;  //设置具体形参声明节点的识别码为funcparam
				q->child=NULL;		//孩子置空
				q->sibling=NULL;	//兄弟置空
				q->listindex=index;	//保存tokenlist中的index
				T->child=q;			//q为传入形参的孩子节点
				index+2;			 
				if(tokenlist[index+2]==COMMA)	//如果标识符后是逗号,说明还有其他形参 
					;
					
			}
			else //类型声明后不是标识符,说明形参声明错误
			{
				printf("\nERROR:function parameter declaration error;Located on line No.%d,near %s",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
				return ERROR; 
			 } 
		 }
		else //不是数据类型声明
		{
			printf("\nERROR:function parameter declaration error;Located on line No.%d,near %s",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
			return ERROR; 
		 } 
	}
 } 
/************************************************
函数功能：按步长打印空格 
************************************************/ 
 void PrintSpace(int step)	//按步长打印空格
{
	printf("\n");			//输出一个换行 
	for(int i=0;i<step;i++)//输出步长的空格
		printf("  "); 		
 } 
/**************************************************
函数功能：遇到语法分析错误时，销毁该树，释放内存空间 
***************************************************/ 
 void Lexer::DeleteTree(syntaxtree& root)
{
	if(root)		//如果树存在 
	{
		DeleteTree(root->child);	//释放左子树孩子结点 
		DeleteTree(root->sibling);	//释放右子树sibling结点
		delete root;				//释放根结点 
	 } 
	 return ; 
 } 
/******************************************************************************
函数功能：屏幕上打印语法树 
******************************************************************************/ 
void Lexer::PrintTree(syntaxtree& root)
{
	syntaxtree p=root;	//声明移动指针
	int step=0;			//采用栈的思想,非递归前序遍历,通过进栈个数来确定输出空格的步长 
	stack<syntaxtree> s;//定义一个栈,考虑到采用动态内存 
	while(p!=NULL||!s.empty())	//当栈空且指针空的时候退出遍历
	{
		if(p)	//如果当前结点不是NULL
		{
			PrintNode(p,step);	//打印当前节点 
			s.push(p);			//当前节点进栈 
			step++;				//打印步长自增
			p=p->child;			//p变为下一个结点 
		 } 
		else	//当前节点为NULL,说明根左右,根已经到头了,即左子树遍历完了回溯右子树
		{
			p=s.top();		//获得当前栈顶节点
			s.pop();		//栈顶节点弹出 
			p=p->sibling;	//变成兄弟节点
			step--;			//兄弟节点的步长是一样的,减去多自增的一次 
		 } 
	 } 
	printf("\n\n打印完毕");
 } 
 /***********************************************************************************
 函数功能：根据语法树节点的类型，输出对应的信息 
 ************************************************************************************/ 
 void Lexer::PrintNode(syntaxtree p,int step)
 {
 	switch(p->kind){	//根据结点的类型来输出结点
	 	
	 	case rt:{		//处理根节点 
	 		PrintSpace(step);//输出前置空格 
	 		printf("程序："); //根节点输出信息 
			break;
		 }
		 
		case inclu:{	//处理include节点 
			PrintSpace(step);//输出前置空格
			printf("外部文件引用include:"); 
			break;
		}
		
		case inclufile:{	//处理引用外部文件名节点 
			PrintSpace(step);//输出前置空格
			printf("外部文件:%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
		
		case exfuncdecla:{	//处理外部函数声明节点 
			PrintSpace(step);//输出前置空格
			printf("外部函数声明:"); 
			break;
		}
		
		case functype:{		//处理函数返回值类型节点
			PrintSpace(step);//输出前置空格 
			printf("函数返回值类型:%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
		
		case funcname:{		//处理函数名节点
			PrintSpace(step);//输出前置空格
			printf("函数名:%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
	   
	    case nonfuncparam:{	//处理空形参节点
			PrintSpace(step);//输出前置空格
			printf("函数形参序列：空"); 
			break;
		} 
	 }
  } 
 
 
 
#endif
