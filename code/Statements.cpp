#ifndef STATEMENSTS_CPP
#define STATEMENTS_CPP
#include "Lexical_analysis.h"
#include<stack>		//����ջ�ļ� 
using namespace std;
#include<iostream>
/************************************************ 
�������ܣ�����return ��䣬�����ظ��ڵ��ַ
************************************************/ 
syntaxtree Lexer::ReturnState()
{
	syntaxtree p=new syntaxnode;	//Ϊ�������ڵ�����ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->kind=returnnode;	//���ýڵ�����Ϊreturnnode 
	p->child=NULL;	//��ʼ�����ӽڵ� 
	p->sibling=NULL; //��ʼ���ֵܽڵ�
	index++;	//��ȡ��һ������
	p->child=Expression();	//return����϶�Ϊ���ʽ
	if(errorflag)
		return NULL;
	else
		return p;	//����p 
}
/*****************************************************
�������ܣ�����break ��䣬�����ظ��ڵ��ַ
****************************************************/
syntaxtree Lexer::BreakState()
{
	syntaxtree p=new syntaxnode;	//Ϊ�������ڵ�����ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->child=NULL;	//��ʼ�����ӽڵ� 
	p->sibling=NULL; //��ʼ���ֵܽڵ�
	p->kind=breaknode;//���ýڵ�ʶ����Ϊbreaknode
	index++;	//ȡһ�´�
	if(tokenlist[index].tokentype==SEMI)	//��ȷ״����,��һ�����Ƿֺ�
	{
		index++;	//��������
		return p;	//���ظ��ڵ� 
	 } 
	 else	//���Ƿֺţ����Ǵ���
	 {
	 	errorflag=1;	//���ô����� 
	 	printf("Error:Expected a ';';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//����NULL 
	  } 
 } 
/*********************************************************
�������ܣ�����continue��䣬�����ظ��ڵ��ַ
********************************************************/
syntaxtree Lexer::ContinueState()
{
	syntaxtree p=new syntaxnode;	//Ϊ�������ڵ�����ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->child=NULL;	//��ʼ�����ӽڵ� 
	p->sibling=NULL; //��ʼ���ֵܽڵ�
	p->kind=continuenode;//���ýڵ�ʶ����Ϊcontinuenode
	index++;	//ȡһ�´�
	if(tokenlist[index].tokentype==SEMI)	//��ȷ״����,��һ�����Ƿֺ�
	{
		index++;	//��������
		return p;	//���ظ��ڵ� 
	 } 
	 else	//���Ƿֺţ����Ǵ���
	 {
	 	errorflag=1;	//���ô����� 
	 	printf("Error:Expected a ';';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//����NULL 
	  } 
 }  
/**************************************************************
�������ܣ�����if���
*****************************************************************/
syntaxtree Lexer::IfState()
{
	syntaxtree p=new syntaxnode;	//Ϊ�������ڵ�����ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->child=NULL;	//��ʼ�����ӽڵ� 
	p->sibling=NULL; //��ʼ���ֵܽڵ�
	p->kind=ifnode;//���ýڵ�ʶ����Ϊifnode
	index++;	//ȡһ�´�
	if(tokenlist[index].tokentype==BRACKETL)	//��ȷ״����,��һ������(
	{
		syntaxtree q=new syntaxnode;	//Ϊif�������ڵ�����ռ�
		if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		q->child=NULL;	//��ʼ�����ӽڵ� 
		q->sibling=NULL;//��ʼ���ֵܽڵ�
		q->kind=ifjudge;	//�ڵ�����Ϊifdge
		index++;
		q->child=Expression();		//����Expression�ӳ������������ʽ
		if(q->child==NULL&&!errorflag||tokenlist[index].tokentype!=BRACKETR) 	//�ж��������Ƿ����
		{
			errorflag=1;
			return NULL;	//����NULL	
		}
		index++; 
		p->child=q;		//��������Ϊif�ڵ�ĵ�һ������ 
		if(tokenlist[index].tokentype==LP)	//�����{
		{
			syntaxtree t=new syntaxnode;	//Ϊif�Ӿ�ڵ�����ռ�
			if(t==NULL)		//�жϿռ��Ƿ�����ɹ�
			{
				//�����Ϣ 
				printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
				getchar();getchar();	//�ȴ��û���Ӧ 
				exit(0);
			 }  
			t->child=NULL;
			t->sibling=NULL;
			t->kind=ifcmsd;	//����if�Ӿ�ڵ�����Ϊifcmsd 
			t->child=CompoundStmd();	//���ø�����䴦�����
			q->sibling=t;		//��Ϊ�����ڵ���ֵܽڵ� 
			if(errorflag)	//�ж��Ƿ���� 
				return NULL;

			index++;	//�������� 
			if(tokenlist[index].tokentype==ELSE)	//����else�Ӿ� 
			{
				syntaxtree t2=new syntaxnode;		//Ϊelse�ڵ�����ռ�
				if(t2==NULL)		//�жϿռ��Ƿ�����ɹ�
				{
					//�����Ϣ 
					printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
					getchar();getchar();	//�ȴ��û���Ӧ 
					exit(0);
				 }   
				t2->child=NULL;
				t2->sibling=NULL;
				t2->kind=elsenode;
				index++;	//Expression����ʱ��index++���˳�ʱindexָ��;�� 
				t2->child=Expression();//������䴦�����
				if(t2->child==NULL||errorflag)	//������Ӿ������������
				{
					errorflag=1;	//��������1
					printf("\nError:Expected correct else expression;Located on line No.%d\nafter else",tokenlist[index].linenum);
					return NULL; 
				 } 
				p->kind=ifelsenode;	//���û�д����Ҫ�޸�if�ڵ������Ϊifelsenode 
				t->sibling=t2;	//t2Ϊif�Ӿ���ֵܽڵ�
				return p;		//����if����Ľڵ� 
			} 
			return p;	//û��elseֱ�ӷ��� 
		 } //����if�������Ľ��� 
		else	//���Ǹ�����䣬����ֻ��һ�����ʽ
		{
			syntaxtree t=new syntaxnode;	//Ϊif�Ӿ�ڵ�����ռ�
			if(t==NULL)		//�жϿռ��Ƿ�����ɹ�
			{
				//�����Ϣ 
				printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
				getchar();getchar();	//�ȴ��û���Ӧ 
				exit(0);
			 }  
			t->child=NULL;
			t->sibling=NULL;
			t->kind=ifcmsd;	//����if�Ӿ�ڵ�����Ϊifcmsd
			t->child=Expression();	//������䴦�����
			q->sibling=t;	//��Ϊ�����ڵ���ֵܽڵ� 
			if(errorflag||tokenlist[index].tokentype!=SEMI)
			{
				errorflag=1;	//��������1
				printf("\nError:Expected the end of experssion is ';';Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL;
			 } 
			index++; 
			if(tokenlist[index].tokentype==ELSE)	//����else�Ӿ� 
			{
				syntaxtree t2=new syntaxnode;		//Ϊelse�ڵ�����ռ�
				if(t2==NULL)		//�жϿռ��Ƿ�����ɹ�
				{
					//�����Ϣ 
					printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
					getchar();getchar();	//�ȴ��û���Ӧ 
					exit(0);
				 }   
				t2->child=NULL;
				t2->sibling=NULL;
				t2->kind=elsenode;
				t2->child=Expression();//������䴦�����
				if(t2->child==NULL||errorflag)	//������Ӿ������������
				{
					errorflag=1;	//��������1
					printf("\nError:Expected correct else expression;Located on line No.%d\nafter else",tokenlist[index].linenum);
					return NULL; 
				 } 
				p->kind=ifelsenode;	//���û�д����Ҫ�޸�if�ڵ������Ϊifelsenode 
				t->sibling=t2;	//t2Ϊif�Ӿ���ֵܽڵ�
				return p;		//����if����Ľڵ� 
			} 
			return p;	//û��elseֱ�ӷ��� 
		 } 
		index++;	//��������
		return p;	//���ظ��ڵ� 
	 } 
	 else	//����(�����Ǵ���
	 {
	 	errorflag=1;	//���ô����� 
	 	printf("Error:Expected a '(';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//����NULL 
	  } 
 } 
/********************************************************************
�������ܣ��������������,��������ʱ��indexָ��Ļ��Ǳ�ʶ�� 
*********************************************************************/
syntaxtree Lexer::CallFunc()
{
	syntaxtree p=new syntaxnode;	//Ϊ�������ڵ�����ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->child=NULL;	//��ʼ�����ӽڵ� 
	p->sibling=NULL; //��ʼ���ֵܽڵ�
	p->kind=callfunc;//���ýڵ�ʶ����Ϊcallfunc
	p->listindex=index;	//���溯���� 
	index=index+2;
	syntaxtree q=p;//����ڵ��ʼ��ַ 
	p->child=Identifier(0);//�����βε���
	if(errorflag)	//������� 
		return NULL; 
	p->child->kind=callmeter;	//�����ڵ����� 
	p=p->child;	//�Ƶ�������һ�㣬�����ӽڵ� 
	while(tokenlist[index].tokentype==COMMA)	//���������ž��˳�
	{
		index++;		//���˶��� 
		p->sibling=Identifier(0);//�����βε��ñ�ʶ��ʶ����
		if(errorflag)	//�������� 
			return NULL; 
		p->sibling->kind=callmeter;	//�޸Ľڵ������Ϊcallmeter
		while(p&&p->sibling)	//�ҵ������ֵܽڵ� 
			p=p->sibling; 	//�Ƶ��������ֵܽڵ� 	
	 } 
	//����ѭ���˳����϶����������ˣ� 
	index++;	//���ˣ�
	if(tokenlist[index].tokentype==SEMI)	//�������ú����Ƿֺ� 
		return q; 
	else
	{
		errorflag=1;	//������Ϊ1
		printf("Error:Expected a ';';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;
	 } 

 } 

#endif
