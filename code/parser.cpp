#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"
#include<stack>		//引用栈文件 
using namespace std;
#include<iostream>

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
		return OK;
	}
	else{
		DeleteTree(root);
		printf("\n遇到语法树分析错误，已经释放语法树内存空间完毕！\n"); 
		return ERROR;
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
			while(tokenlist[i].tokentype!=SEMI&&tokenlist[i].tokentype!=ENDFILE)		//找分号
			{
				i++;
			 } 
			 
			if(tokenlist[i].linenum==tokenlist[index].linenum)	//如果该行是以分号结尾,说明是函数前置声明
			{
				if(FunctionDeclaration()==OK)	//如果函数声明分析程序返回正确值
				{
					index=index+2;	//索引自增 
					if(DeclarationList()==ERROR)//如果是声明序列返回值出现问题就返回ERROR 
						return ERROR;
					else
						return OK;
				 } 
				else		//函数声明分析程序返回错误值
				{
					return ERROR;	//出现语法错误,声明序列返回ERROR 
				 } 
			 }
			else	//那么就是函数定义,调用函数定义处理函数 
			{
				if(FunctionDefine()==OK)
				{
					if(DeclarationList()==ERROR)//如果是声明序列返回值出现问题就返回ERROR 
						return ERROR;
					else
						return OK;
				 } 
				else		//函数定义处理函数遇到错误
					return ERROR; 
			}
		 } //处理外部函数定义与外部函数声明结束 
		else //其他情况只可能是外部变量的声明 
		{
			if(ExVarDeclaration()==OK)	//如果外部变量函数声明函数返回正确值
			{
				if(DeclarationList()==ERROR)//如果是声明序列返回值出现问题就返回ERROR 
					return ERROR;
				else
					return OK;
			 } 
			else	//外部变量声明错误
				return ERROR;	//返回错误值 
		 } //处理外部变量声明结束 
	}	//处理类型声明结束
	
	else if(tokenlist[index].tokentype==COMMENT)
	{
		index++;
		if(DeclarationList()==ERROR)//如果是声明序列返回值出现问题就返回ERROR 
			return ERROR;
		else
			return OK;
	 } 	
	else //不合法语句 
	{
		printf("\nERROR:Expected correct statements;\nLocated on No.%d,near character '%s';\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return ERROR;	//返回错误值 
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
	if(tokenlist[index].tokentype!=ID)	//如果类型之后不是标识符
		return ERROR;	//返回错误值 
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
	else		//函数返回错误值 
		return ERROR; //返回错误值 
 }
/***************************************************
函数功能：处理函数定义
***************************************************/
status Lexer::FunctionDefine()
{
	if(FunctionDeclaration()==OK)	//调用函数声明函数 
	{
		syntaxtree 	T=root->child;	//中间指针p指向根节点的孩子 
		while(T&&T->sibling)	//如果根节点有孩子,且兄弟节点不为NULL 
		{
			T=T->sibling;	//p指向该兄弟节点,一直找到最后一个兄弟节点为止 
		 } 
		T->kind=funcdef;	//更改函数节点类型识别码为funcdefine
		index++;	//调用FunctionDeclaration()后index指向的是）
		if(tokenlist[index].tokentype==LP)
		{
//			index++;
			T=T->child;	//移到下一层语法树
			while(T&&T->sibling)	//找到该层最后的兄弟节点
			{
				T=T->sibling;		//移动到下一个兄弟节点 
			 } 
			 T->sibling=CompoundStmd();
			 if(errorflag)	//如果出现了错误
			 {
			 	return ERROR;//返回错误值 
			  } 
			if(!errorflag)
			{
//				index++;
				return OK;
			 } 
			else
				return ERROR;
		 } 
		else	//不是花括号
		{
			return ERROR;
		 } 
	}
	else
		return ERROR;
 } 
/*************************************************
函数功能：复合语句处理
************************************************/
syntaxtree Lexer::CompoundStmd()
{
	index++;
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
	p->kind=compstmd;//设置节点识别码为compstmd 
	syntaxtree q=p;	//保存初始复合语句节点的地址 
	if((1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)||tokenlist[index].tokentype==CONST)
	{
		p->child=LocalVar();
		if(errorflag)
			return NULL;
	}
	if(p->child)	//如果有第一个孩子
	{
		p->child->sibling=StatementList();//调用语句序列处理函数
		if(errorflag)
		{
			return NULL;
		 } 
	 } 
	else	//无第一个孩子
	{
		p->child=StatementList();//调用语句序列处理函数生成第一个孩子 
		if(errorflag)
		{
			return NULL;
		 } 
	 } 
	if(tokenlist[index].tokentype!=RP) 
	{
		errorflag=1;//设置errorflag 
		printf("Error:Expected a '}';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//返回NULL 
	}
	index++;//索引自增 
	return q;	
 } 
/*************************************************
函数功能：语句序列处理函数
*************************************************/
syntaxtree Lexer::StatementList()
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
	p->kind=statelist;//设置节点识别码为statelist 
	syntaxtree q=p;	//保存初始节点地址 
	p->child=Statement();//调用处理语句的子程序
	if(p->child==NULL) //如果为空 
		return NULL;	//不管是错误或是无语句,都是返回NULL,错误之前都标记好了
	else
	{
		p=p->child;	//移动到孩子节点
		p->sibling=StatementList();//递归处理剩余的语句序列
		return q;	//返回初始地址 
	 } 
 } 
/***************************************************
函数功能：语句处理函数
***************************************************/
syntaxtree Lexer::Statement()
{
	syntaxtree p=NULL;	//设置中间指针变量 
	switch(tokenlist[index].tokentype){	//根据词法识别码来处理语句
		
		case IF:{	//分析条件语句 
			p=IfState();	//调用if语句处理程序 
			if(errorflag)	//如果出错 
				return NULL;//返回NULL 
			break;
		}
		
		case FOR:{	//分析for语句
			p=ForState(); //调用for语句处理程序 
			if(errorflag)	//如果出错 
				return NULL;//返回NULL 
			break;
		} 
		
		case INTCONST:
		case FLOATCONST:
		case DOUBLECONST:
		case LONGCONST:
		case ID:{
			if(tokenlist[index+1].tokentype==BRACKETL)	//判断是否是函数调用
			{
				p=CallFunc();//出来时index指向; 
				if(errorflag)	//如果出错 
					return NULL;
			 } 
			else
			{
				p=Expression();
				if(errorflag)
					return NULL;
				if(tokenlist[index].tokentype!=SEMI)
				{
					errorflag=1;
					printf("\nError:Expected ';' after expression;Located on line No.%d;\nnear charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
					return NULL;
				 } 
			}
			index++;//过滤分号 
			break;
		} 
		case RETURN:{
			p=ReturnState();//调用ReturnState处理,出来时指向分号 
			index++;	//读取下一个词 
			break;
		} 
		
		case BRACKETL:{	//遇到（
			index++;
			p=Expression();//出来时指向） 
			if(errorflag||tokenlist[index].tokentype!=BRACKETR)//如果有错误或者不是因为）结束
			{ 
				errorflag=1;	//错误标记置1
				printf("\nError:Expected the end of experssion is ')';Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL;
			} 
			else
			{
				index++;	//表达式正确,取词索引自增 
			}
			break;
		} 
		
		case BREAK:{	//break语句
			p=BreakState(); //出来时指向分号的下一位 
			break;
		} 
		
		case CONTINUE:{	//continue语句 
			p=ContinueState(); //出来时指向分号的下一位 
			break;
		} 
		
		case LP:{	//遇到左花括号，就是复合语句
			p=CompoundStmd();//调用复合语句处理
			if(errorflag)	//如果遇到错误
				return NULL; 
			break;
		} 
		
		case RP:{	//遇到右花括号,就是语句序列结束 
			return NULL; 
			break;
		}
		
		
		case COMMENT:{	//遇到注释就跳过
			index++; 
			break;
		} 
		
	} //end of switch
	return p;
 } 
/*************************************************
函数功能：表达式处理函数
***************************************************/
syntaxtree Lexer::Expression()
{
	syntaxtree t2=NULL;
	syntaxtree t1=NULL;
	syntaxtree t=NULL; 
	stack<syntaxtree> op;//定义运算符栈,指针元素 
	syntaxtree p=new syntaxnode; //为初始化#申请内存空间
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 }  
	p->kind=spec;	//设置#节点的识别码为spe
	p->child=NULL;	//初始化孩子节点 
	p->sibling=NULL; //初始化兄弟节点
	p->listindex=vectorindex+1;//#在tokenlist中附加区放置 
	op.push(p);		//加入操作符栈
	stack<syntaxtree> opn;	//定义操作数栈
	TokenType w=tokenlist[index].tokentype;//保存当前读取词的识别码 
	while((w!=SPEC||op.top()->kind!=spec)&&!errorflag)	//当运算符栈栈顶不是起止符号或者没有读到表达式末尾时，并且没有错误时 
	{
		if(w==ID||(7<=w&&w<=11)) //如果是标识符或者常量
		{
			//生成一个节点 
			syntaxtree q=new syntaxnode; //为初始化#申请内存空间
			if(q==NULL)		//判断空间是否申请成功
			{
				//输出信息 
				printf("内存申请失败！\n内存不够，自动关闭\n");
				getchar();getchar();	//等待用户响应 
				exit(0);
			 }  
			 q->kind=exp;	//设置节点类型为表达式组成
			 q->listindex=index;//保存tokenlist中的索引
			 q->child=NULL; //孩子节点置空 
			 q->sibling=NULL;//兄弟节点置空
			 opn.push(q);	//操作数进opn栈 
			 index++;	//读取下一个词 
			 w=tokenlist[index].tokentype;//更新w 
		 } 
		 else if(37<=w&&w<=50) //如果是算符
		 {
		 	switch(JudgeLevel(op.top()->listindex,w)){
		 		case -1:{	//当前优先级低，出栈形成节点 
		 			if(opn.size()>=2&&op.size())	//如果操作数栈至少有两个元素，操作符栈有一个元素 
		 			{
					 	t2=opn.top();//获取当前操作数栈顶元素 
					 	opn.pop();//弹出栈顶 
						t1=opn.top();//获取当前操作数栈顶元素
						opn.pop();//弹出栈顶
						t=op.top();//获取当前操作符栈顶元素
						op.pop();	//弹出栈顶
						t->kind=expre;//表达式根节点类型 
						t->child=t1;//t1作为t的孩子
						t1->sibling=t2;//t1的兄弟节点为t2
						opn.push(t);	//连接成功后,根节点指针进操作数栈 
					} 
					else{
						printf("\nError:Expected correct expresion;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
						errorflag=1;//设置错误标记 
					} 
					break;
				 }//处理优先级低的出栈结束 
				 
				case 1:{	//当前优先级高，进栈 
					syntaxtree z=new syntaxnode; //为该操作符申请内存空间 
					if(z==NULL)		//判断空间是否申请成功
					{
						//输出信息 
						printf("内存申请失败！\n内存不够，自动关闭\n");
						getchar();getchar();	//等待用户响应 
						exit(0);
					 }  
					z->listindex=index;	//保存在tokenlist中的索引
					z->child=NULL;	//初始化孩子节点
					z->sibling=NULL;//初始化兄弟节点
					z->kind=exp;	//设置节点类型为表达式组成部分
					op.push(z);		//该操作符进栈
					index++;		//取下一个词 
					w=tokenlist[index].tokentype;//更新w 
					break;
				}//处理优先级高的进栈结束 
			 }
		  }
		  else	if(w==SEMI||w==BRACKETR)	//如果是分号或者)，就是结束符
		  	w=SPEC;	//w换成#
		  else //其他符号均为错
		  {
			  	printf("\nError:Expected correct expression;Located on line No.%d;\nnear charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
			  	errorflag=1;
		   } 
	 }//end of while 
	 if(opn.size()==1&&op.top()->kind==spec&&!errorflag)//操作数栈只剩一个，操作符只剩#，无错误
	 	return opn.top();	//返回操作数栈顶最后一个元素 
	 else 
	 { 
	 	errorflag=1;	//错误标记置1 
	 	printf("\nError:Expected correct expression;Located on line No.%d\nahead of charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
	 	return NULL; 
	 } 
 } 
/************************************************
函数功能：处理函数内的局部变量声明
**********************************************/
syntaxtree Lexer::LocalVar()
{
	syntaxtree p=new syntaxnode;//为局部变量节点申请空间 
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->child=NULL;	//初始化孩子节点
	p->sibling=NULL;//初始化兄弟节点
	p->kind=locvardecla;//设置节点的类型识别码为locvardecla 
	syntaxtree q=p;		//保存初始p的地址 
	while((1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)||tokenlist[index].tokentype==CONST)
	{
		if(tokenlist[index].tokentype==CONST)	//如果是const 
			index++; 	//索引自增
		if(p->kind==id||p->kind==array)	//如果节点类型是标识符或者数组,非第一次处理局部变量 
		{
			while(p->sibling)	//找到最后一个兄弟节点 
			{
				p=p->sibling;//移动到下一个兄弟节点 
			}
			p->sibling=TypeSpecifier();//调用类型声明处理函数
			if(errorflag) //处理类型声明错误 
			{
				return NULL;
			}
			else
			{
				p=p->sibling;//移动到下一个兄弟节点 
				p->sibling=Identifier(1);//调用标识符处理函数 
				if(p->sibling==NULL)
					return NULL ;
				while(tokenlist[index].tokentype==COMMA&&!errorflag)//可能声明的不止一个变量
				{
					index++;//过滤逗号
					while(p&&p->sibling)	//移到最后的兄弟节点处 
						p=p->sibling;	//p移动到兄弟节点处
					p->sibling=Identifier(1);//调用标识符处理函数 
				 } 
				if(errorflag)	//如果遇到错误
					return NULL;
				else if(tokenlist[index].tokentype!=SEMI)	//判断是否有分号
				{
					errorflag=1;
					printf("\Error:Expected a ';' on line No.%d;near charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
					return NULL; 
				 }
				 while(p&&p->sibling) 
				 	p=p->sibling;	//移动到兄弟节点 
				 index++;//过滤掉分号 
			 }
		}
		else	//第一次处理局部变量
		{
			p->child=TypeSpecifier();//调用类型声明处理函数
			if(p->child==NULL) //处理类型声明错误 
			{
				return NULL;
			}
			else
			{
				p=p->child; 
				p->sibling=Identifier(1);//调用标识符处理函数 
				if(p->sibling==NULL)
					return NULL ;
				while(tokenlist[index].tokentype==COMMA&&!errorflag)//可能声明的不止一个变量
				{
					index++;//过滤逗号
					while(p&&p->sibling)	//移到最后的兄弟节点处 
						p=p->sibling;	//p移动到兄弟节点处
					p->sibling=Identifier(1);//调用标识符处理函数 
				 } 
				if(errorflag)	//如果遇到错误
					return NULL;
				else if(tokenlist[index].tokentype!=SEMI)	//判断是否有分号
				{
					errorflag=1;
					printf("\Error:Expected a ';' on line No.%d;near charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
					return NULL; 
				 } 
				while(p&&p->sibling)
					p=p->sibling;//移到到最后的兄弟节点处 
				index++;//过滤了分号 
			}
		 } 
	}//end of while
	return q;	//返回初始地址q 
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
		syntaxtree q=NULL;	//设立中间指针 
		q=Parameter();		//保存形参处理程序的返回值
		if(errorflag) 	//如果错误标记位制置1
			return ERROR;	//说明形参处理出现了错误，返回错误值 
		syntaxtree p=new syntaxnode;//为函数形参序列节点申请空间
		if(p==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		if(!q)	//说明形参为空
		{
			p->kind=nonfuncparam;//设置形参序列节点类型识别码为nonfuncparam
			p->child=NULL;	//其孩子节点设为NULL
			p->sibling=NULL;//其兄弟节点设为NULL
			T->sibling=p;	//该节点为传入形参的兄弟节点 
			return OK;
		 } 
		else 
		{
			p->kind=funcparam;//设置形参序列节点类型识别码为funcparam
			p->child=q;	//其孩子节点设为q
			p->sibling=NULL;//其兄弟节点设为NULL
			T->sibling=p;	//该节点为传入形参的兄弟节点 
			return OK;
		 }  
	}
} 
/*******************************************************
函数功能：形参处理函数 
此时index指向的是(的下一位 
*******************************************************/
syntaxtree Lexer::Parameter()
{
	while(tokenlist[index].tokentype!=BRACKETR) 	//当不是右括号就说明是形参序列,进行处理 
	{
		syntaxtree p=NULL;//声明中间指针p 
		p=TypeSpecifier();	//类型声明节点作为兄弟
		syntaxtree q=p;		//保存类型声明节点作为返回值 
		if(p)	//如果类型声明正确,则P不为NULL 
		{ 
			p->sibling=Identifier(1); //识别标识符，函数形参声明，传递参数1 
			if(p->sibling)
			{
				if(tokenlist[index].tokentype==COMMA)	//如果遇到逗号,说明有后续的形参 
					index++;	//逗号过滤 
				if(p->sibling->sibling)	//判断是否有数组形参节点存在 
					p->sibling->sibling->sibling=Parameter();//递归调用自身
				else
					p->sibling->sibling=Parameter();//递归调用自身 
				return q;	//返回最初节点的地址 
			} 
			else			//递归调用自身返回NULL,说明出现错误 
				return NULL; 
		} 
		else  
			return NULL;	//类型声明识别程序返回NULL，说明形参的类型声明有问题,返回NULL 
	}
	return NULL;	//while中没有返回，退出while就返回NULL 
 } 
/**************************************************************
函数功能：处理类型声明,此时传入的索引index理想情况下指向类型声明 
*************************************************************/ 
syntaxtree Lexer::TypeSpecifier()			//类型声明处理函数
{
	syntaxtree p=NULL;	//声明中间指针
	p=new syntaxnode;	//为类型设明 
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->child=NULL;	//孩子节点初始化
	p->sibling=NULL;//兄弟节点初始化 
	if(tokenlist[index].tokentype==CONST)//如果是const类型正确声明
	{
		index++; //移动到下一位词 
	 } 	//处理const类型声明结束 
	if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<6)//如果是普通类型声明
	{
		p->kind=type;	//设置类型节点识别码为type
		p->listindex=index;	//保存在tokenlist中的索引
		index++;		//索引自增
		return p; 
	 }
	else		//不满足以上两种情况,则是错误的类型声明
	{
		printf("\nError:wrong typespecifier;\nLocatede on line No.%d,near character '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		errorflag=1;	//设置错误标志为1 
		return NULL;	//返回NULL 
	 } 
 } 
/**************************************************
函数功能：识别标识符并生成对应节点,
传入的形参state决定了数组是否为函数形参声明 
**************************************************/ 
syntaxtree Lexer::Identifier(int state)
{
	if(tokenlist[index].tokentype==ID)	//如果是标识符
	{
		syntaxtree p=new syntaxnode;	//为类型设明 
		if(p==NULL)		//判断空间是否申请成功
		{
			//输出信息 
			printf("内存申请失败！\n内存不够，自动关闭\n");
			getchar();getchar();	//等待用户响应 
			exit(0);
		 } 
		p->kind=id;		//设置节点类型为id 标识符变量名 
		p->child=NULL;	//孩子节点置空
		p->sibling=NULL;//兄弟节点置空
		p->listindex=index;//保存在tokenlist中的索引
		index++;	//索引自增
		if(tokenlist[index].tokentype==PML)	//判断是否是遇到数组
		{
			if(state)	//处理函数内形参数组类型
			{
				int level=0;	//记录数组的维数 
				do{
					 index++;	//索引自增
					 if(tokenlist[index].tokentype!=PMR)
					{
						errorflag=1;//设置错误标记
						printf("\nError:expected correct array parameter define;\nLocated on line No.%d;near characters'%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
						return NULL;//返回NULL						 
					 } 
					index++; //索引自增 
					level++;//维数自增 
				}while(tokenlist[index].tokentype==PML);//遇到不是[退出循环 
				p->sibling=new syntaxnode;//为数组节点申请空间
				if(p->sibling==NULL) //判断空间是否申请成功
				{
					//输出信息 
				printf("内存申请失败！\n内存不够，自动关闭\n");
				getchar();getchar();	//等待用户响应 
				exit(0);
			 	}  
			 	p->sibling->child=NULL;//初始化孩子节点
				p->sibling->sibling=NULL;//初始化 
				p->sibling->listindex=level;//保存数组的维数 
				p->sibling->kind=array;//设置节点的识别码为array 
				return p; 
			 } 
			else	//非函数形参数组
			{
				int level=0;	//记录数组的维数
				do{
					 index++;	//索引自增
					 if(tokenlist[index].tokentype!=PMR)
					{
						errorflag=1;//设置错误标记
						printf("\nError:expected correct array parameter define;\nLocated on line No.%d;near characters'%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
						return NULL;//返回NULL						 
					 } 
					index=index+2; //索引自增 
					level++;//维数自增 
				}while(tokenlist[index].tokentype==PML);//遇到不是[退出循环 
				p->sibling=new syntaxnode;//为数组节点申请空间
				if(p->sibling==NULL) //判断空间是否申请成功
				{
					//输出信息 
				printf("内存申请失败！\n内存不够，自动关闭\n");
				getchar();getchar();	//等待用户响应 
				exit(0);
			 	}  
			 	p->sibling->child=NULL;//初始化孩子节点
				p->sibling->sibling=NULL;//初始化 
				p->sibling->listindex=level;//保存数组的维数 
				p->sibling->kind=array;//设置节点的识别码为array 
				return p;  
			 } 
		}
		return p;		//返回该节点地址 
	}
	  
	else	//不是标识符就是错误 
	{
		printf("\nError:expected an Identifier;Located on line No.%d;\nnear characters'%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		errorflag=1;//设置错误标记为1 
		return NULL;	//返回NULL 
	}
 } 
/****************************************************
函数功能：外部变量处理
******************************************************/
status Lexer::ExVarDeclaration()
{
	syntaxtree 	T=root->child;	//中间指针p指向根节点的孩子 
	while(T&&T->sibling)	//如果根节点有孩子,且兄弟节点不为NULL 
	{
		T=T->sibling;	//p指向该兄弟节点,一直找到最后一个兄弟节点为止 
	 } 
 	syntaxtree p=new syntaxnode;	//外部变量声明节点申请空间
	if(p==NULL)		//判断空间是否申请成功
	{
		//输出信息 
		printf("内存申请失败！\n内存不够，自动关闭\n");
		getchar();getchar();	//等待用户响应 
		exit(0);
	 } 
	p->kind=exvardecla;	//该节点的标识码设定唯exfuncdecla
	p->sibling=NULL;		//兄弟节点设为NULL
	T->sibling=p;			//p节点作为形参T的兄弟节点 
	p->child=TypeSpecifier();//调用类型处理函数 
	syntaxtree q=p->child;	//设置中间过渡指针 
	do{
		while(q&&q->sibling)	//找到最后一个兄弟节点
			q=q->sibling;		//移动到下一个兄弟节点 
		q->sibling=Identifier(0);//调用标识符处理函数
		if(q->sibling==NULL)	//如果标识符处理函数为NULL 
			return ERROR;	//返回错误值		 
	}while(tokenlist[index].tokentype==COMMA);//有逗号,说明是多变量声明 
	if(tokenlist[index].tokentype==SEMI)	//如果退出后的词恰好是分号
	{
		index++;	//索引自增
		return OK;	//返回正确值 
	 } 
	else
	{
		printf("\nError:Expected ';' after var declaration;Located on line No.%d;\nnear character '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return ERROR;
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
	 root=NULL;		//只是释放了内存空间,指针不变,将根指针指向NULL
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
	   
	    case nonfuncparam:{	//处理空形参序列节点
			PrintSpace(step);//输出前置空格
			printf("函数形参序列：空"); 
			break;
		} 
		
	    case funcparam:{	//处理非空形参序列节点
			PrintSpace(step);//输出前置空格
			printf("函数形参序列："); 
			break;
		} 
		
		case type:{	//处理数据类型声明节点
			PrintSpace(step);//输出前置空格
			if(tokenlist[p->listindex-1].tokentype==CONST)//如果前一位有const 
				printf("类型声明：const %s",tokenlist[p->listindex].tokenstring.c_str()); 
			else	//前一位不是const 
				printf("类型声明：%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
		
		case id:{	//处理标识符类型声明节点
			PrintSpace(step);//输出前置空格
			printf("变量名：%s",tokenlist[p->listindex].tokenstring.c_str());
			break;
		} 
		
		case array:{	//处理数组类型节点
			printf(" %d维数组",p->listindex);//输出数组的维数 
			break;
		} 
		
		case exvardecla:{	//处理外部变量声明节点
			PrintSpace(step);//输出前置空格
			printf("外部变量声明节点："); 
			break;
		}
		
		case funcdef:{	//处理函数定义节点
			PrintSpace(step);	//输出前置空格
			printf("函数定义节点："); 
			break;
		} 
		
		case compstmd:{	//处理复合语句节点
			PrintSpace(step);	//输出前置空格
			printf("复合语句："); 
			break;
		}
		
		case locvardecla:{ //处理局部变量声明节点
			PrintSpace(step);	//输出前置空格
			printf("局部变量声明："); 
			break;
		}
		
		case statelist:{	//处理语句序列节点
			PrintSpace(step);	//输出前置空格
			printf("语句序列:"); 
			break;
		}
		
		case breaknode:{	//处理break节点
			PrintSpace(step);//输出前置空格
			printf("break"); 
			break;
		}
		
		case exp:{	//处理表达式组成部分节点
			PrintSpace(step);	//输出前置空格
			printf("%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
		
		case expre:{//处理表达式子树根节点
			PrintSpace(step);	//输出前置空格
			printf("表达式：");
			PrintSpace(step);
			printf("%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
		
		case returnnode:{	//处理return语句
			PrintSpace(step);	//输出前置空格
			printf("返回语句return："); 
			break;
		}
		
		case continuenode:{	//处理continue
			PrintSpace(step);	//输出前置空格
			printf("continue"); 
			break;
		}
		
		case ifnode:{	//处理单纯if句
			PrintSpace(step);	//输出前置空格
			printf("分支结构 if型"); 
			break;
		}
		
		case ifelsenode:{	//处理ifelse型
			PrintSpace(step);	//输出前置空格
			printf("分支结构 if-else型"); 
			break;
		}
		
		case ifjudge:{	//处理条件节点
			PrintSpace(step);
			printf("条件句："); 
			break;
		}
		
		case ifcmsd:{	//处理if子句
			PrintSpace(step);
			printf("if子句："); 
			break;
		}
		
		case elsenode:{	//处理else节点
			PrintSpace(step);
			printf("else："); 
			break;
		}
		
		case fornode:{	//处理for节点
			PrintSpace(step);//输出前置空格
			printf("for循环语句："); 
			break;
		}
		
		case forstart:{	//处理for初始条件节点
			PrintSpace(step);//输出前置空格
			printf("for初始表达式："); 
			if(p->child==NULL)	//判断是否为空
				printf("空"); 
			break;
		}
		
		case forjudge:{	//处理for判断句
			PrintSpace(step);//输出前置空格
			printf("for循环判断表达式："); 
			if(p->child==NULL)//判断是否为空
				printf("空"); 
			break;
		}
		
		
		case forchange:{//处理for更新语句
			PrintSpace(step);//输出前置空格
			printf("for迭代表达式：");
			if(p->child==NULL)	//判断是否为空
				printf("空"); 
			break;
		}
		
		case forstate:{	//处理for子句
			PrintSpace(step);//输出前置空格
			printf("for子句："); 
			if(p->child==NULL)	//判断是否为空
				printf("空"); 
			break;
		}
		
		case callfunc:{	//函数调用
			PrintSpace(step);	//输出前置空格
			printf("函数%s调用",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
		
		case callmeter:{	//处理函数调用的形参
			PrintSpace(step);	//输出前置空格
			printf("传入形参：%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
		
	 }//end of switch
  } 
/***********************************************************
函数功能：判断算符的优先级
**************************************************************/
int Lexer::JudgeLevel(int index,TokenType w)
{
	if(oplevel[tokenlist[index].tokentype-37]<=oplevel[w-37])	//如果当前算符的优先级低 
		return -1;
	if(oplevel[tokenlist[index].tokentype-37]>oplevel[w-37])	//如果当前优先级高 
		return  1; 
 } 
/******************************************************************
函数功能：插入p的兄弟节点q
******************************************************************/
status Lexer::InsertSibling(syntaxtree& p,syntaxtree& q)
{
	p->sibling=q;
	return OK;
}
/****************************************************************
函数功能：插入p的孩子节点q
*****************************************************************/
status Lexer::InsertChild(syntaxtree& p,syntaxtree& q)
{
	p->child=q;
	return OK;
 } 
 
 
#endif
