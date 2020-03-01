#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"


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

	while(tokenlist[index].tokentype==COMMENT)	//过滤掉开头注释
		index++;			//取词索引自增
	
	while(tokenlist[index].tokentype==INCLUDE)	//处理开头的include语句
	{
		if(root->child)					//根节点已经有孩子,说明新的节点在孩子的兄弟上
		{
			syntaxtree ptr=root->child->child; 	//声明移动指针
			while(ptr->sibling)	//找到兄弟节点为空的那个节点
				ptr=ptr->sibling;	//移动到下一个兄弟节点
			ptr->sibl 
		 } 
		else		//根节点还没有孩子的情况，即第一次遇到include
		{ 
			syntaxtree p=new syntaxnode;	//为include节点申请空间
			p->kind=inclu;					//设置该节点类别码为inclu
			p->sibling=NULL;				//设置兄弟节点为NULL
			p->listindex=index;				//记录在tokenlist中的序列
			syntaxtree q=new syntaxnode;	//为include的孩子申请空间,即引用的文件名
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
	
	if(DeclarationList(root->child)!=ERROR)			//调用外部声明序列 处理函数 
	{
		printf("\n\n语法树打印如上！");
	}
	else
	{
		printf("\n\n出现语法错误，生成语法树失败"); 
	 } 		 	  
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
 
 
 
 
 
#endif
