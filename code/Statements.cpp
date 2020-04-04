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
	 	printf("\nError:Expected a ';';\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
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
	 	printf("\nError:Expected a ';';\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//����NULL 
	  } 
 }  
/*****************************************************************
�������ܣ�����for���
*****************************************************************/
syntaxtree Lexer::ForState()
{
	syntaxtree p=new syntaxnode;	//Ϊ�������ڵ�����ռ�
	syntaxtree q=NULL;	//�м�ָ��
	syntaxtree t=NULL;	//�м�ָ�� 
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->child=NULL;	//��ʼ�����ӽڵ�
	p->sibling=NULL;//��ʼ���ֵܽڵ�
	p->kind=fornode;//�ڵ�����Ϊfornode
	index++;	//��ȡfor�������һ����
	if(tokenlist[index].tokentype==BRACKETL)	//����ǣ�,������ȷ
	{
		index++;	//��ȡ�����һ����
		q=new syntaxnode;	//Ϊforstart�ڵ������ڴ� 
		if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		q->child=NULL;	//��ʼ�����ӽڵ�
		q->sibling=NULL;//��ʼ���ֵܽڵ�
		q->kind=forstart;//���ýڵ�����Ϊforstart
		p->child=q;	//forstart�ڵ�Ϊfor�ڵ�ĵ�һ������ 
		if(tokenlist[index].tokentype!=SEMI)//�������for(;XXX;XXX)��ʽ
		{
			q->child=Expression();//���ñ��ʽ��䴦�����
			if(errorflag)	//����������� 
				return NULL;
			if(tokenlist[index].tokentype!=SEMI)	//���������Ϊ�ֺ��˳���
			{
				errorflag=1;	//������
				printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
				return NULL;//����NULL  
			 } 
		 } 
		index++;	//��������,��ȡ;�����һ����
		t=new syntaxnode;	//Ϊforjudge�ڵ������ڴ� 
		if(t==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		t->child=NULL;	//��ʼ�����ӽڵ�
		t->sibling=NULL;//��ʼ���ֵܽڵ�
		t->kind=forjudge;//���ýڵ�����Ϊforjudge
		q->sibling=t;	//forjudge�ڵ�Ϊforstart�ڵ���ֵ� 
		q=q->sibling;	//qָ���ƶ���forjudge�ڵ��� 
		if(tokenlist[index].tokentype!=SEMI)//�������for(;;XXX)��ʽ
		{
			t->child=Expression();//���ñ��ʽ��䴦�����
			if(errorflag)	//����������� 
				return NULL;
			if(tokenlist[index].tokentype!=SEMI)	//���������Ϊ�ֺ��˳���
			{
				errorflag=1;	//������
				printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
				return NULL;//����NULL  
			 } 
		 } 
		index++;	//��������,��ȡ;�����һ����
		t=new syntaxnode;	//Ϊforchange�ڵ������ڴ� 
		if(t==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		t->child=NULL;	//��ʼ�����ӽڵ�
		t->sibling=NULL;//��ʼ���ֵܽڵ�
		t->kind=forchange;//���ýڵ�����Ϊforchange
		q->sibling=t;	//forchange�ڵ�Ϊforjudge�ڵ���ֵ� 
		q=q->sibling;	//qָ���ƶ���forchange�ڵ��� 
		if(tokenlist[index].tokentype!=BRACKETR)//�������for(XX;XX;)��ʽ
		{
			t->child=Expression();//���ñ��ʽ��䴦�����
			if(errorflag)	//����������� 
				return NULL;
			if(tokenlist[index].tokentype!=BRACKETR)	//���������Ϊ)�˳���
			{
				errorflag=1;	//������
				printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
				return NULL;//����NULL  
			 } 
		 } 
		index++;	//��������
		t=new syntaxnode;	//Ϊforstate�ڵ������ڴ� 
		if(t==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		t->child=NULL;	//��ʼ�����ӽڵ�
		t->sibling=NULL;//��ʼ���ֵܽڵ�
		t->kind=forstate;//���ýڵ�����Ϊforstate
		q->sibling=t;	//forstate�ڵ�Ϊforchange�ڵ���ֵܽڵ�
		t->child=Statement();	//for�Ӿ������䴦����
		if(errorflag)	//�����������
		{
			printf("\nError:Expected a ';' in for condition;\nLocaterd on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
			return NULL;//����NULL  
		 } 
		return p;	//����p�ڵ��ַ 
	 } 
	else	//for���治�Ǹ��ţ�,���Ǵ���
	{
		errorflag=1;	//������
		printf("\nError:Expected () after for;\nLocaterd on line No.%d\nnear chararctor '%s' \n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//����NULL 	 
	 } 
 } 
/**************************************************************
�������ܣ�����if���
*****************************************************************/
syntaxtree Lexer::IfState()
{
	syntaxtree p=new syntaxnode;	//Ϊ�������ڵ�����ռ�
	syntaxtree q=NULL;	//�м�ָ��
	syntaxtree t=NULL;	//�м�ָ�� 
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
	if(tokenlist[index].tokentype==BRACKETL)	//if�������������
	{
		t=new syntaxnode;	//Ϊ�������ڵ������ڴ� 
		if(t==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		t->child=NULL;//��ʼ�����ӽڵ�
		t->sibling=NULL;//��ʼ���ֵܽڵ�
		t->kind=ifjudge;//�ڵ�����Ϊif������
		index++;	//������ʽ����ʱindexָ�����ź��һλ 
		t->child=Expression();//�����ڵ�ĺ��ӵ��ñ��ʽ������
		if(errorflag||t->child==NULL)	//�ж�if�������Ƿ�Ϸ�
		{
			errorflag=1;	//��������1
			printf("\nError:Expected correct if condition expression;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
			return NULL;		 
		 } 
		 p->child=t;	//�����ڵ���Ϊif�ڵ�ĺ���
		if(tokenlist[index].tokentype==SEMI)//����������˳��Ƿ�����Ϊ���ַֺ�
		{
			errorflag=1;	//��������1
			printf("\nError:wrong semi used;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
			return NULL;		  
		 } 
		 index++;	//��ȡ��һ���ʣ���ʼ�ж�if�Ӿ䲿��,expression����ʱָ��;���ߣ� 
		q=new syntaxnode;	//Ϊif�Ӿ�ڵ������ڴ�
		if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		q->child=NULL;	//��ʼ�����ӽڵ�
		q->sibling=NULL;//��ʼ���ֵܽڵ�
		q->kind=ifcmsd;	//�ڵ�����Ϊif�Ӿ�
		t->sibling=q;	//if�Ӿ�ڵ�Ϊif�ڵ�ĺ���,��������ֵܽڵ� 
		if(tokenlist[index].tokentype==LP)	//����������{
		{
			q->child=CompoundStmd();	//���ø�����䴦�����,������䴦�����indexָ��}��һλ 
			if(errorflag)	//�����������
			{
				printf("\nError:Expected correct if statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL;
			 } 
			if(tokenlist[index].tokentype==ELSE)	//����if�����else
			{
				p->kind=ifelsenode;	//�޸�p�ڵ������Ϊifeslenode
				t=t->sibling; //t�Ƶ�if�Ӿ�ڵ���
				q=new syntaxnode;	//Ϊelse�ڵ������ڴ�
				if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
				{
					//�����Ϣ 
					printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
					getchar();getchar();	//�ȴ��û���Ӧ 
					exit(0);
				 } 
				q->child=NULL;	//��ʼ�����ӽڵ�
				q->sibling=NULL;//��ʼ���ֵܽڵ�
				q->kind=elsenode;	//�ڵ�����Ϊelse�ڵ�
				t->sibling=q;	//else�ڵ�Ϊif�Ӿ�ڵ���ֵ�
				index++;	//��ȡelse�ĺ���һ������
				q->child=Statement();	//������䴦�����,����else����������Ϊelse�ڵ�ĺ���
				if(errorflag||q->child==NULL)//���else���洦�����
				{
					errorflag=1;	//��������1
					printf("\nError:Expected correct else statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
					return NULL;
				 } 
				else
					return p;	//if-else����ɹ�����p�ڵ��ַ  
			 }	
			else	//if��û��else,������if�� 
			{
				return p;//����p��ַ 
			 } 
		 } 
		else	//û������������{,if�Ӿ�ֻ��һ��
		{
			q->child=Expression();	//���ñ��ʽ���������Ϊif�Ӿ�ĺ��ӽڵ�
			if(q->child==NULL||errorflag) //�ж��Ƿ����
			{
				errorflag=1;//������
				printf("\nError:wrong if statements;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL; 
			 } 
			//if�Ӿ����expreesionû�г���
			if(tokenlist[index].tokentype!=SEMI)	//���������Ϊ�ֺ��˳���
			{
				printf("\nError:Expected ; to end a statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL;
			 } 
			index++;	//��ȡ��һ���� 
			//if�Ӿ�û�г�����ʼ�����Ƿ����else
			if(tokenlist[index].tokentype==ELSE)	//���if����else
			{
				p->kind=ifelsenode;	//�޸�p������Ϊifelse�ͽڵ� 
				t=t->sibling;	//tָ���Ƶ�if�Ӿ�ڵ���
				q=new syntaxnode;	//Ϊelse�ڵ������ڴ�
				if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
				{
					//�����Ϣ 
					printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
					getchar();getchar();	//�ȴ��û���Ӧ 
					exit(0);
				 } 
				q->child=NULL;	//��ʼ�����ӽڵ�
				q->sibling=NULL;//��ʼ���ֵܽڵ�
				q->kind=elsenode;	//�ڵ�����Ϊelse�ڵ�
				t->sibling=q;	//else�ڵ�Ϊif�Ӿ�ڵ���ֵ�
				index++;	//��ȡelse�ĺ���һ������
				q->child=Statement();	//������䴦�����,����else����������Ϊelse�ڵ�ĺ���
				if(errorflag||q->child==NULL)//���else���洦�����
				{
					errorflag=1;	//��������1
					printf("\nError:Expected correct else statement;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
					return NULL;
				 } 
				else
					return p;	//if-else����ɹ�����p�ڵ��ַ 
			 } //�����else�ͽ��� 
			else	//������if��
			{
				return p;//���ؽڵ��ַ 
			 } 
		 } 
	 } //����if�ģ�
	else	//if��û�н����ţ�
	{
		errorflag=1;	//��������1
		printf("\nError:Expected if condition charactor '(';Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
		return NULL;		 
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
