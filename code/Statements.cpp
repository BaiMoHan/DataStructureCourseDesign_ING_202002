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
	p->child=NULL;	//��ʼ�����ӽڵ� 
	p->sibling=NULL; //��ʼ���ֵܽڵ�
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



#endif
