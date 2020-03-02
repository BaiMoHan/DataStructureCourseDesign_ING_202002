#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"
#include<stack>		//引用栈文件 
using namespace std;
#include<iostream>

//void PrintNode(syntaxtree p,int step);	//打印树结点函数前置声明 
status Lexer::Program()	//程序语法分析函数开始
{
	syntaxtree root=new syntaxnode; 
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
		}	//处理第一次遇到include生成节点情况
		
		while(tokenlist[index].tokentype==COMMENT)	//处理行尾注释
			index++;				//取词索引自增,相当于过滤掉注释
				 
	 } //处理include的while结束 
	
//	if(DeclarationList(root->child)!=ERROR)			//调用外部声明序列 处理函数 
//	{
//		printf("\n\n语法树打印如上！");
//	}
//	else
//	{
//		printf("\n\n出现语法错误，生成语法树失败"); 
//	 } 		 
	PrintTree(root);	  
 } 



status Lexer::DeclarationList(syntaxtree& T)	//声明序列语法分析
{
	if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)//如果是类型声明
	{
		
	}	//处理类型声明结束
	else if(tokenlist[index].tokentype==INCLUDE)	//include语句
	{
		
	}	//处理include语句结束
	
 } 
 
 void PrintSpace(int step)	//按步长打印空格
{
	printf("\n");			//输出一个换行 
	for(int i=0;i<step;i++)//输出步长的空格
		printf("  "); 		
 } 
 
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
 
void Lexer::PrintTree(syntaxtree& root)
{
	syntaxtree p=root;	//声明移动指针
	int step=0;			//采用栈的思想,非递归前序遍历,通过进栈个数来确定输出空格的步长 
	stack<syntaxtree> s;//定义一个栈,考虑到采用动态内存 
	while(p!=NULL||!s.empty())	//当栈空且指针空的时候退出遍历
	{
		while(p!=NULL)	//一直 
		{
			PrintNode(p,step);	//打印当前节点 
			s.push(p);	//加入当前节点
			step++;		//每次加入一个左孩子,打印步长就自增
			p=p->child;	//p移动到下一个孩子 
		
		}
		if(!s.empty())	//栈不为空
		{
			p=s.top();	//p为当前栈顶元素,回溯，上一步while退出说明左子树走到底了
			s.pop();	//弹出栈顶元素
			p=p->sibling;//左子树已经遍历完毕,移动到右孩子,即此结点的兄弟结点上
			if(p)	//如果兄弟结点不为空则打印 
				PrintNode(p,step);//此右子树是兄弟结点,所以步长与左子树的步长是一样的 
		 } 
	 } 
	printf("打印完毕");
 } 
 
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
	  
	 }
  } 
 
 
 
#endif
