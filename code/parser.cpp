#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"
#include<stack>		//����ջ�ļ� 
using namespace std;
#include<iostream>

//void PrintNode(syntaxtree p,int step);	//��ӡ����㺯��ǰ������ 
status Lexer::Program()	//�����﷨����������ʼ
{
	root=new syntaxnode; 
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
			index++;			//�������� 
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
			index++;		 				//�������� 
		}	//�����һ������include���ɽڵ����
		
		while(tokenlist[index].tokentype==COMMENT)	//������βע��
			index++;				//ȡ����������,�൱�ڹ��˵�ע��
				 
	 } //����include��while���� 
	 
	p=root->child;	//pָ����ڵ�ĺ��� 
	while(p&&p->sibling)	//������ڵ��к���,���ֵܽڵ㲻ΪNULL 
	{
		p=p->sibling;	//pָ����ֵܽڵ�,һֱ�ҵ����һ���ֵܽڵ�Ϊֹ 
	 } 
	if(DeclarationList()==OK)		//�����ⲿ�������,���һ���ֵܽڵ�ĵ�ַ��Ϊ�βδ��� 
	{
		printf("\n\n�﷨�����ɹ�\n"); 
	}
	 
//	 PrintTree(root); 
  
 } 



status Lexer::DeclarationList()	//���������﷨����
{ 

	if(tokenlist[index].tokentype==ENDFILE)		//��������β
	{
		return OK;			//������ȷֵ 
	 } 
	 
	syntaxtree 	p=root->child;	//�м�ָ��pָ����ڵ�ĺ��� 
	while(p&&p->sibling)	//������ڵ��к���,���ֵܽڵ㲻ΪNULL 
	{
		p=p->sibling;	//pָ����ֵܽڵ�,һֱ�ҵ����һ���ֵܽڵ�Ϊֹ 
	 } 
	 
	if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)//�������������
	{
		if(tokenlist[index+2].tokentype==BRACKETL)		//�����������֮��ڶ�������������,˵�����ⲿ������ص�
		{
			int i=index;	//�м�ѭ������ 
			while(tokenlist[i].tokentype!=SEMI)		//�ҷֺ�
			{
				i++;
			 } 
			 
			if(tokenlist[i].linenum==tokenlist[index].linenum)	//����������ԷֺŽ�β,˵���Ǻ���ǰ������
			{
				if(FunctionDeclaration()==OK)	//������������������򷵻���ȷֵ
				{
					DeclarationList();
				 } 
			 }
			else	//��ô���Ǻ�������,���ú������崦���� 
			{
				
			}
		 } 
		else if(tokenlist[index+2].tokentype==SEMI)	//�����������֮��ĵڶ���������ʶ����֮����Ƿֺ�,�������� 
		{
			
		 } 
	}	//����������������
	
	else 
	{
		
	}	
	
 } 
 
 /******************************************************
 �������ܣ������ⲿ�������� 
 ******************************************************/
 status Lexer::FunctionDeclaration()
 {
 	
 	syntaxtree 	T=root->child;	//�м�ָ��pָ����ڵ�ĺ��� 
	while(T&&T->sibling)	//������ڵ��к���,���ֵܽڵ㲻ΪNULL 
	{
		T=T->sibling;	//pָ����ֵܽڵ�,һֱ�ҵ����һ���ֵܽڵ�Ϊֹ 
	 } 
 	syntaxtree p=new syntaxnode;	//�ⲿ���������ڵ�����ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->kind=exfuncdecla;	//�ýڵ�ı�ʶ���趨Ψexfuncdecla
	p->sibling=NULL;		//�ֵܽڵ���ΪNULL
	T->sibling=p;			//p�ڵ���Ϊ�β�T���ֵܽڵ� 
	syntaxtree q=new syntaxnode;	//����ֵ���ͽڵ�����ռ�
	if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	q->kind=functype;		//�趨�ڵ�ʶ����Ϊfunctype
	q->listindex=index++;	//����tokenlist������
	q->child=NULL;			//��������ֵ���ͽڵ��޺���
	p->child=q;				//��������ֵ���ͽڵ����ⲿ���������ڵ�ĺ���
	p=new syntaxnode;		//Ϊ�������ڵ�����ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 }  
	q->sibling=p;	//�������ڵ��Ƿ���ֵ�ڵ���ֵܽڵ�
	p->kind=funcname;//�������ڵ���������Ϊfuncname
	p->listindex=index++;//����tokenlist�еĺ���������,��ʱindexָ�� 
	p->child=NULL;	//�������ڵ��޺���
	p->sibling=NULL;//�ֵܽڵ��ÿ� 
	if(ParameterList(p)==OK)	//���ú����βδ����� 
	{
		return OK; 
	}
 }

/***************************************************
�������ܣ��������β����� 
****************************************************/
status Lexer::ParameterList(syntaxtree& T)
{
	index++;	//�����ʱ���ǣ�������������ƶ���������һλ
	if(tokenlist[index].tokentype==BRACKETR) //�����������,˵�����β���������
	{
		syntaxtree p=NULL;	//�����м�ָ��
		p=new syntaxnode;	//Ϊ�����β� 
		if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 }  
		p->kind=nonfuncparam;	//���ú������β����нڵ��ʶ����Ϊnonfuncparam
		p->child=NULL;		//�����ÿ�
		p->sibling=NULL;	//�ֵܽڵ��ÿ�
		T->sibling=p;		//�ýڵ���Ϊ�����βε��ֵܽڵ�
		return OK;			//������ȷֵ 
 	}
 	else //����������,˵��Ӧ�����β�����
	{ 
		syntaxtree q=NULL;	//�����м�ָ�� 
		q=Parameter();		//�����βδ������ķ���ֵ
		if(!q)	//˵���βδ���ʧ��
			return ERROR;
		else 
		{
			syntaxtree t=q;	//����q��ǰָ��Ĺ��ɽڵ�ĵ�ַ 
			q=q->sibling;	//�޳����м���ɽڵ�
			delete t;		//�ͷŹ��ɽڵ���ڴ� 
			if(q)		//���qΪ��
			{
				
			 } 
			else
			{
				
			 } 
		 } 
//		syntaxtree p=NULL;	//�����м�ָ��
//		p=new syntaxnode;	//Ϊ�����β� 
//		if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
//		{
//			//�����Ϣ 
//			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
//			getchar();getchar();	//�ȴ��û���Ӧ 
//			exit(0);
//		 }  
//		p->kind=funcparam;	//���ú����β����нڵ��ʶ����Ϊfuncparam
//		p->child=NULL;		//�����ÿ�
//		p->sibling=NULL;	//�ֵܽڵ��ÿ�
//		T->sibling=p;		//�ýڵ���Ϊ�����βε��ֵܽڵ�
//		if(Parameter(p)==NULL);			//�������β� 
	}
} 
/*******************************************************
�������ܣ��βδ����� 
��ʱindexָ�����(����һλ 
*******************************************************/
syntaxtree Lexer::Parameter()
{
	syntaxtree p=new syntaxnode;//�����м�ڵ�p
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 }  
	 p->child=NULL;//��ʼ�����ӽڵ� 
	 p->sibling=NULL;//��ʼ���ֵܽڵ� 
	syntaxtree q=p;	//����p�ĳ�ʼ��ַ 
	while(tokenlist[index].tokentype!=BRACKETR) 	//�����������ž�˵�����β�����,���д��� 
	{
		p->sibling=TypeSpecifier();	//���������ڵ���Ϊ�ֵ�
		p=p-sibling;	//p�ƶ����ֵܽڵ�
		if(p)	//�������������ȷ
		{ 
//			p->sibling=Identifier(); ʶ���ʶ��
			p=p->sibling;	//�����ƶ����ֵܽڵ�
			if(p)			//�����ʶ����ȷ 
				Parameter();//�ݹ���ñ���,������һ���βεĴ���
			else			//��ʶ������������NULL,˵���������� 
			 	return NULL;//����NULL 
		} 
		else  
			return NULL;	//��������ʶ����򷵻�NULL��˵���βε���������������,����NULL 
	}
//	if(T->child)	//���һ��ʼ�к���,˵��Ӧ�õ��ֵܽڵ����
//	{
//	 }
//	else		//��һ�β���
//	{
//		T->child=TypeSpecifier();//������������ķ���ֵ��ַ��Ϊ�β����нڵ�ĺ���
//		if(T->child)	//������Ӳ�ΪNULL,˵������������ȷ,���洦���ʶ��
//		{
//			T->child->sibling=Identifier();//��ʶ���ķ���ֵ��ַ��Ϊ�����������ֵܽڵ�
//			if(T->child->sibling)	//�����Ϊ��,˵����ʶ����ȷ 
//				
//		 } 
//	 } 
//	if(tokenlist[index].tokentype==CONST)	//�������const����
//	{
//		
//	 } 
//	else //����const���� 
//	{
//		if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)//�����������������
//		{
//			if(tokenlist[index+1].tokentype==ID) 
//			{
//				syntaxtree q=NULL;	//�����м�ָ��
//				q=new syntaxnode;	//Ϊ�����β������ڵ�����ռ�
//				if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
//				{
//					//�����Ϣ 
//					printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
//					getchar();getchar();	//�ȴ��û���Ӧ 
//					exit(0);
//				 } 
//				q->kind=funcparam;  //���þ����β������ڵ��ʶ����Ϊfuncparam
//				q->child=NULL;		//�����ÿ�
//				q->sibling=NULL;	//�ֵ��ÿ�
//				q->listindex=index;	//����tokenlist�е�index
//				T->child=q;			//qΪ�����βεĺ��ӽڵ�
//				index+2;			 
//				if(tokenlist[index+2].tokentype==COMMA)	//�����ʶ�����Ƕ���,˵�����������β� 
//					;
//					
//			}
//			else //�����������Ǳ�ʶ��,˵���β���������
//			{
//				printf("\nERROR:function parameter declaration error;Located on line No.%d,near %s",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
//				return ERROR; 
//			 } 
//		 }
//		else //����������������
//		{
//			printf("\nERROR:function parameter declaration error;Located on line No.%d,near %s",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
//			return ERROR; 
//		 } 
//	}
 } 
/**************************************************************
�������ܣ�������������,��ʱ���������index���������ָ���������� 
*************************************************************/ 
syntaxtree Lexer::TypeSpecifier()			//��������������
{
	syntaxtree p=NULL;	//�����м�ָ��
	p=new syntaxnode;	//Ϊ�������� 
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->child=NULL;	//���ӽڵ��ʼ��
	p->sibling=NULL;//�ֵܽڵ��ʼ�� 
	if(tokenlist[index].tokentype==CONST&&1<=tokenlist[index+1].tokentype&&tokenlist[index+1].tokentype<6)//�����const������ȷ����
	{
		p->kind=consttype;	//���ø����������ڵ�ʶ����Ϊconsttype
		p->listindex=index;	//����tokenlist�е�����
		index=index+2;	//�ƶ�����������֮����ַ�
		return p;		//����ָ�� 
	 } 	//����const������������ 

 } 
/************************************************
�������ܣ���������ӡ�ո� 
************************************************/ 
 void PrintSpace(int step)	//��������ӡ�ո�
{
	printf("\n");			//���һ������ 
	for(int i=0;i<step;i++)//��������Ŀո�
		printf("  "); 		
 } 
/**************************************************
�������ܣ������﷨��������ʱ�����ٸ������ͷ��ڴ�ռ� 
***************************************************/ 
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
/******************************************************************************
�������ܣ���Ļ�ϴ�ӡ�﷨�� 
******************************************************************************/ 
void Lexer::PrintTree(syntaxtree& root)
{
	syntaxtree p=root;	//�����ƶ�ָ��
	int step=0;			//����ջ��˼��,�ǵݹ�ǰ�����,ͨ����ջ������ȷ������ո�Ĳ��� 
	stack<syntaxtree> s;//����һ��ջ,���ǵ����ö�̬�ڴ� 
	while(p!=NULL||!s.empty())	//��ջ����ָ��յ�ʱ���˳�����
	{
		if(p)	//�����ǰ��㲻��NULL
		{
			PrintNode(p,step);	//��ӡ��ǰ�ڵ� 
			s.push(p);			//��ǰ�ڵ��ջ 
			step++;				//��ӡ��������
			p=p->child;			//p��Ϊ��һ����� 
		 } 
		else	//��ǰ�ڵ�ΪNULL,˵��������,���Ѿ���ͷ��,���������������˻���������
		{
			p=s.top();		//��õ�ǰջ���ڵ�
			s.pop();		//ջ���ڵ㵯�� 
			p=p->sibling;	//����ֵܽڵ�
			step--;			//�ֵܽڵ�Ĳ�����һ����,��ȥ��������һ�� 
		 } 
	 } 
	printf("\n\n��ӡ���");
 } 
 /***********************************************************************************
 �������ܣ������﷨���ڵ�����ͣ������Ӧ����Ϣ 
 ************************************************************************************/ 
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
		
		case exfuncdecla:{	//�����ⲿ���������ڵ� 
			PrintSpace(step);//���ǰ�ÿո�
			printf("�ⲿ��������:"); 
			break;
		}
		
		case functype:{		//����������ֵ���ͽڵ�
			PrintSpace(step);//���ǰ�ÿո� 
			printf("��������ֵ����:%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
		
		case funcname:{		//���������ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("������:%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
	   
	    case nonfuncparam:{	//������βνڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("�����β����У���"); 
			break;
		} 
		
		case consttype:{	//����const���������ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("����������%s %s",tokenlist[p->listindex].tokenstring.c_str(),tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
	 }
  } 
/******************************************************************
�������ܣ�����p���ֵܽڵ�q
******************************************************************/
status Lexer::InsertSibling(syntaxtree& p,syntaxtree& q)
{
	p->sibling=q;
	return OK;
}
/****************************************************************
�������ܣ�����p�ĺ��ӽڵ�q
*****************************************************************/
status Lexer::InsertChild(syntaxtree& p,syntaxtree& q)
{
	p->child=q;
	return OK;
 } 
 
 
#endif
