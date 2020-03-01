#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"


status Lexer::Program()	//�����﷨����������ʼ
{
	syntaxtree root=new syntaxnode; 
	if(root==NULL)	//�ж��ڴ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	root->kind=rt;	//���ڵ������� 
	root->child=NULL;//��ʼ������
	root->sibling=NULL;//��ʼ���ֵܽ�� 
	root->listindex=-1;	//��ʼ������ 

	while(tokenlist[index].tokentype==COMMENT)	//���˵���ͷע��
		index++;			//ȡ����������
	
	while(tokenlist[index].tokentype==INCLUDE)	//����ͷ��include���
	{
		if(root->child)					//���ڵ��Ѿ��к���,˵���µĽڵ��ں��ӵ��ֵ���
		{
			syntaxtree ptr=root->child->child; 	//�����ƶ�ָ��
			while(ptr->sibling)	//�ҵ��ֵܽڵ�Ϊ�յ��Ǹ��ڵ�
				ptr=ptr->sibling;	//�ƶ�����һ���ֵܽڵ�
			ptr->sibl 
		 } 
		else		//���ڵ㻹û�к��ӵ����������һ������include
		{ 
			syntaxtree p=new syntaxnode;	//Ϊinclude�ڵ�����ռ�
			p->kind=inclu;					//���øýڵ������Ϊinclu
			p->sibling=NULL;				//�����ֵܽڵ�ΪNULL
			p->listindex=index;				//��¼��tokenlist�е�����
			syntaxtree q=new syntaxnode;	//Ϊinclude�ĺ�������ռ�,�����õ��ļ���
			q->kind=inclufile;				//���øýڵ��ʶ����Ϊinclufile
			q->child=NULL;					//�����ļ��������к���,�����ļ�����Ϊ�ֵ�
			q->sibling=NULL;				//��ʼ���ֵܽڵ�
			q->listindex=++index;			//û�б���������,include�ĺ�һ�������ļ���
			root->child=p;					//include�ڵ���Ϊroot�ĵ�һ������
			p->child=q;						//include�ڵ�ĺ���Ϊ����˳�����õ��ļ����ڵ�		 
		}	//�����һ������include���ɽڵ����
		
		while(tokenlist[index].tokentype==COMMENT)	//������βע��
			index++;				//ȡ����������,�൱�ڹ��˵�ע��
				 
	 } //����include��while���� 
	
	if(DeclarationList(root->child)!=ERROR)			//�����ⲿ�������� ������ 
	{
		printf("\n\n�﷨����ӡ���ϣ�");
	}
	else
	{
		printf("\n\n�����﷨���������﷨��ʧ��"); 
	 } 		 	  
 } 



status Lexer::DeclarationList(syntaxtree& T)	//���������﷨����
{
	if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)//�������������
	{
		
	}	//����������������
	else if(tokenlist[index].tokentype==INCLUDE)	//include���
	{
		
	}	//����include������
	
 } 
 
 
 
 
 
#endif
