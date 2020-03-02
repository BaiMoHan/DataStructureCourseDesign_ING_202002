#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"
#include<stack>		//����ջ�ļ� 
using namespace std;
#include<iostream>

//void PrintNode(syntaxtree p,int step);	//��ӡ����㺯��ǰ������ 
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
	syntaxtree p=NULL;	//����һ���м�ָ��
	syntaxtree q=NULL;	//����һ���м�ָ�� 

	while(tokenlist[index].tokentype==COMMENT)	//���˵���ͷע��
		index++;			//ȡ����������
	
	while(tokenlist[index].tokentype==INCLUDE)	//����ͷ��include���
	{
		if(root->child)					//���ڵ��Ѿ��к���,˵���µĽڵ��ں��ӵ��ֵ���
		{
			p=root->child->child; 	//���¶����ƶ�ָ���λ�� 
			while(p->sibling)	//�ҵ��ֵܽڵ�Ϊ�յ��Ǹ��ڵ�
				p=p->sibling;	//�ƶ�����һ���ֵܽڵ�
			q=new syntaxnode;	//Ϊ�½������ռ� 
			p->sibling=q;		//�����õ��ļ�����Ϊ֮ǰ�����ļ����ڵ���ֵ�
			q->kind=inclufile;	//���øý���ʶ����Ϊinclufile
			q->child=NULL;		//�½��ĺ����ÿ�
			q->sibling=NULL;	//�½����ֵ��ÿ�
			q->listindex=++index;//û�б���������,include�ĺ�һ�������ļ��� 
		 } 
		else		//���ڵ㻹û�к��ӵ����������һ������include
		{ 
			p=new syntaxnode;	//Ϊinclude�ڵ�����ռ�
			p->kind=inclu;					//���øýڵ������Ϊinclu
			p->sibling=NULL;				//�����ֵܽڵ�ΪNULL
			p->listindex=index;				//��¼��tokenlist�е�����
			q=new syntaxnode;	//Ϊinclude�ĺ�������ռ�,�����õ��ļ���
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
	
//	if(DeclarationList(root->child)!=ERROR)			//�����ⲿ�������� ������ 
//	{
//		printf("\n\n�﷨����ӡ���ϣ�");
//	}
//	else
//	{
//		printf("\n\n�����﷨���������﷨��ʧ��"); 
//	 } 		 
	PrintTree(root);	  
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
 
 void PrintSpace(int step)	//��������ӡ�ո�
{
	printf("\n");			//���һ������ 
	for(int i=0;i<step;i++)//��������Ŀո�
		printf("  "); 		
 } 
 
 void Lexer::DeleteTree(syntaxtree& root)
{
	if(root)		//��������� 
	{
		DeleteTree(root->child);	//�ͷ����������ӽ�� 
		DeleteTree(root->sibling);	//�ͷ�������sibling���
		delete root;				//�ͷŸ���� 
	 } 
	 return ; 
 } 
 
void Lexer::PrintTree(syntaxtree& root)
{
	syntaxtree p=root;	//�����ƶ�ָ��
	int step=0;			//����ջ��˼��,�ǵݹ�ǰ�����,ͨ����ջ������ȷ������ո�Ĳ��� 
	stack<syntaxtree> s;//����һ��ջ,���ǵ����ö�̬�ڴ� 
	while(p!=NULL||!s.empty())	//��ջ����ָ��յ�ʱ���˳�����
	{
		while(p!=NULL)	//һֱ 
		{
			PrintNode(p,step);	//��ӡ��ǰ�ڵ� 
			s.push(p);	//���뵱ǰ�ڵ�
			step++;		//ÿ�μ���һ������,��ӡ����������
			p=p->child;	//p�ƶ�����һ������ 
		
		}
		if(!s.empty())	//ջ��Ϊ��
		{
			p=s.top();	//pΪ��ǰջ��Ԫ��,���ݣ���һ��while�˳�˵���������ߵ�����
			s.pop();	//����ջ��Ԫ��
			p=p->sibling;//�������Ѿ��������,�ƶ����Һ���,���˽����ֵܽ����
			if(p)	//����ֵܽ�㲻Ϊ�����ӡ 
				PrintNode(p,step);//�����������ֵܽ��,���Բ������������Ĳ�����һ���� 
		 } 
	 } 
	printf("��ӡ���");
 } 
 
 void Lexer::PrintNode(syntaxtree p,int step)
 {
 	switch(p->kind){	//���ݽ���������������
	 	
	 	case rt:{		//������ڵ� 
	 		PrintSpace(step);//���ǰ�ÿո� 
	 		printf("����"); //���ڵ������Ϣ 
			break;
		 }
		 
		case inclu:{	//����include�ڵ� 
			PrintSpace(step);//���ǰ�ÿո�
			printf("�ⲿ�ļ�����include:"); 
			break;
		}
		
		case inclufile:{	//���������ⲿ�ļ����ڵ� 
			PrintSpace(step);//���ǰ�ÿո�
			printf("�ⲿ�ļ�:%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
	  
	 }
  } 
 
 
 
#endif
