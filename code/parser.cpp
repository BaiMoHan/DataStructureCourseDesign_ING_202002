#ifndef PARSER_CPP
#define PARSER_CPP
#include "Lexical_analysis.h"
#include<stack>		//����ջ�ļ� 
using namespace std;
#include<iostream>

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
		return OK;
	}
	else{
		DeleteTree(root);
		printf("\n�����﷨�����������Ѿ��ͷ��﷨���ڴ�ռ���ϣ�\n"); 
		return ERROR;
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
			while(tokenlist[i].tokentype!=SEMI&&tokenlist[i].tokentype!=ENDFILE)		//�ҷֺ�
			{
				i++;
			 } 
			 
			if(tokenlist[i].linenum==tokenlist[index].linenum)	//����������ԷֺŽ�β,˵���Ǻ���ǰ������
			{
				if(FunctionDeclaration()==OK)	//������������������򷵻���ȷֵ
				{
					index=index+2;	//�������� 
					if(DeclarationList()==ERROR)//������������з���ֵ��������ͷ���ERROR 
						return ERROR;
					else
						return OK;
				 } 
				else		//���������������򷵻ش���ֵ
				{
					return ERROR;	//�����﷨����,�������з���ERROR 
				 } 
			 }
			else	//��ô���Ǻ�������,���ú������崦���� 
			{
				if(FunctionDefine()==OK)
				{
					if(DeclarationList()==ERROR)//������������з���ֵ��������ͷ���ERROR 
						return ERROR;
					else
						return OK;
				 } 
				else		//�������崦������������
					return ERROR; 
			}
		 } //�����ⲿ�����������ⲿ������������ 
		else //�������ֻ�������ⲿ���������� 
		{
			if(ExVarDeclaration()==OK)	//����ⲿ����������������������ȷֵ
			{
				if(DeclarationList()==ERROR)//������������з���ֵ��������ͷ���ERROR 
					return ERROR;
				else
					return OK;
			 } 
			else	//�ⲿ������������
				return ERROR;	//���ش���ֵ 
		 } //�����ⲿ������������ 
	}	//����������������
	
	else if(tokenlist[index].tokentype==COMMENT)
	{
		index++;
		if(DeclarationList()==ERROR)//������������з���ֵ��������ͷ���ERROR 
			return ERROR;
		else
			return OK;
	 } 	
	else //���Ϸ���� 
	{
		printf("\nERROR:Expected correct statements;\nLocated on No.%d,near character '%s';\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return ERROR;	//���ش���ֵ 
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
	if(tokenlist[index].tokentype!=ID)	//�������֮���Ǳ�ʶ��
		return ERROR;	//���ش���ֵ 
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
	else		//�������ش���ֵ 
		return ERROR; //���ش���ֵ 
 }
/***************************************************
�������ܣ�����������
***************************************************/
status Lexer::FunctionDefine()
{
	if(FunctionDeclaration()==OK)	//���ú����������� 
	{
		syntaxtree 	T=root->child;	//�м�ָ��pָ����ڵ�ĺ��� 
		while(T&&T->sibling)	//������ڵ��к���,���ֵܽڵ㲻ΪNULL 
		{
			T=T->sibling;	//pָ����ֵܽڵ�,һֱ�ҵ����һ���ֵܽڵ�Ϊֹ 
		 } 
		T->kind=funcdef;	//���ĺ����ڵ�����ʶ����Ϊfuncdefine
		index++;	//����FunctionDeclaration()��indexָ����ǣ�
		if(tokenlist[index].tokentype==LP)
		{
//			index++;
			T=T->child;	//�Ƶ���һ���﷨��
			while(T&&T->sibling)	//�ҵ��ò������ֵܽڵ�
			{
				T=T->sibling;		//�ƶ�����һ���ֵܽڵ� 
			 } 
			 T->sibling=CompoundStmd();
			 if(errorflag)	//��������˴���
			 {
			 	return ERROR;//���ش���ֵ 
			  } 
			if(!errorflag)
			{
//				index++;
				return OK;
			 } 
			else
				return ERROR;
		 } 
		else	//���ǻ�����
		{
			return ERROR;
		 } 
	}
	else
		return ERROR;
 } 
/*************************************************
�������ܣ�������䴦��
************************************************/
syntaxtree Lexer::CompoundStmd()
{
	index++;
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
	p->kind=compstmd;//���ýڵ�ʶ����Ϊcompstmd 
	syntaxtree q=p;	//�����ʼ�������ڵ�ĵ�ַ 
	if((1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)||tokenlist[index].tokentype==CONST)
	{
		p->child=LocalVar();
		if(errorflag)
			return NULL;
	}
	if(p->child)	//����е�һ������
	{
		p->child->sibling=StatementList();//����������д�����
		if(errorflag)
		{
			return NULL;
		 } 
	 } 
	else	//�޵�һ������
	{
		p->child=StatementList();//����������д��������ɵ�һ������ 
		if(errorflag)
		{
			return NULL;
		 } 
	 } 
	if(tokenlist[index].tokentype!=RP) 
	{
		errorflag=1;//����errorflag 
		printf("Error:Expected a '}';\nLocaterd on line No.%d near chararctor '%s' ",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return NULL;//����NULL 
	}
	index++;//�������� 
	return q;	
 } 
/*************************************************
�������ܣ�������д�����
*************************************************/
syntaxtree Lexer::StatementList()
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
	p->kind=statelist;//���ýڵ�ʶ����Ϊstatelist 
	syntaxtree q=p;	//�����ʼ�ڵ��ַ 
	p->child=Statement();//���ô��������ӳ���
	if(p->child==NULL) //���Ϊ�� 
		return NULL;	//�����Ǵ�����������,���Ƿ���NULL,����֮ǰ����Ǻ���
	else
	{
		p=p->child;	//�ƶ������ӽڵ�
		p->sibling=StatementList();//�ݹ鴦��ʣ����������
		return q;	//���س�ʼ��ַ 
	 } 
 } 
/***************************************************
�������ܣ���䴦����
***************************************************/
syntaxtree Lexer::Statement()
{
	syntaxtree p=NULL;	//�����м�ָ����� 
	switch(tokenlist[index].tokentype){	//���ݴʷ�ʶ�������������
		
		case IF:{	//����������� 
			p=IfState();	//����if��䴦����� 
			if(errorflag)	//������� 
				return NULL;//����NULL 
			break;
		}
		
		case FOR:{	//����for���
			p=ForState(); //����for��䴦����� 
			if(errorflag)	//������� 
				return NULL;//����NULL 
			break;
		} 
		
		case INTCONST:
		case FLOATCONST:
		case DOUBLECONST:
		case LONGCONST:
		case ID:{
			if(tokenlist[index+1].tokentype==BRACKETL)	//�ж��Ƿ��Ǻ�������
			{
				p=CallFunc();//����ʱindexָ��; 
				if(errorflag)	//������� 
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
			index++;//���˷ֺ� 
			break;
		} 
		case RETURN:{
			p=ReturnState();//����ReturnState����,����ʱָ��ֺ� 
			index++;	//��ȡ��һ���� 
			break;
		} 
		
		case BRACKETL:{	//������
			index++;
			p=Expression();//����ʱָ�� 
			if(errorflag||tokenlist[index].tokentype!=BRACKETR)//����д�����߲�����Ϊ������
			{ 
				errorflag=1;	//��������1
				printf("\nError:Expected the end of experssion is ')';Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
				return NULL;
			} 
			else
			{
				index++;	//���ʽ��ȷ,ȡ���������� 
			}
			break;
		} 
		
		case BREAK:{	//break���
			p=BreakState(); //����ʱָ��ֺŵ���һλ 
			break;
		} 
		
		case CONTINUE:{	//continue��� 
			p=ContinueState(); //����ʱָ��ֺŵ���һλ 
			break;
		} 
		
		case LP:{	//���������ţ����Ǹ������
			p=CompoundStmd();//���ø�����䴦��
			if(errorflag)	//�����������
				return NULL; 
			break;
		} 
		
		case RP:{	//�����һ�����,����������н��� 
			return NULL; 
			break;
		}
		
		
		case COMMENT:{	//����ע�;�����
			index++; 
			break;
		} 
		
	} //end of switch
	return p;
 } 
/*************************************************
�������ܣ����ʽ������
***************************************************/
syntaxtree Lexer::Expression()
{
	syntaxtree t2=NULL;
	syntaxtree t1=NULL;
	syntaxtree t=NULL; 
	stack<syntaxtree> op;//���������ջ,ָ��Ԫ�� 
	syntaxtree p=new syntaxnode; //Ϊ��ʼ��#�����ڴ�ռ�
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 }  
	p->kind=spec;	//����#�ڵ��ʶ����Ϊspe
	p->child=NULL;	//��ʼ�����ӽڵ� 
	p->sibling=NULL; //��ʼ���ֵܽڵ�
	p->listindex=vectorindex+1;//#��tokenlist�и��������� 
	op.push(p);		//���������ջ
	stack<syntaxtree> opn;	//���������ջ
	TokenType w=tokenlist[index].tokentype;//���浱ǰ��ȡ�ʵ�ʶ���� 
	while((w!=SPEC||op.top()->kind!=spec)&&!errorflag)	//�������ջջ��������ֹ���Ż���û�ж������ʽĩβʱ������û�д���ʱ 
	{
		if(w==ID||(7<=w&&w<=11)) //����Ǳ�ʶ�����߳���
		{
			//����һ���ڵ� 
			syntaxtree q=new syntaxnode; //Ϊ��ʼ��#�����ڴ�ռ�
			if(q==NULL)		//�жϿռ��Ƿ�����ɹ�
			{
				//�����Ϣ 
				printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
				getchar();getchar();	//�ȴ��û���Ӧ 
				exit(0);
			 }  
			 q->kind=exp;	//���ýڵ�����Ϊ���ʽ���
			 q->listindex=index;//����tokenlist�е�����
			 q->child=NULL; //���ӽڵ��ÿ� 
			 q->sibling=NULL;//�ֵܽڵ��ÿ�
			 opn.push(q);	//��������opnջ 
			 index++;	//��ȡ��һ���� 
			 w=tokenlist[index].tokentype;//����w 
		 } 
		 else if(37<=w&&w<=50) //��������
		 {
		 	switch(JudgeLevel(op.top()->listindex,w)){
		 		case -1:{	//��ǰ���ȼ��ͣ���ջ�γɽڵ� 
		 			if(opn.size()>=2&&op.size())	//���������ջ����������Ԫ�أ�������ջ��һ��Ԫ�� 
		 			{
					 	t2=opn.top();//��ȡ��ǰ������ջ��Ԫ�� 
					 	opn.pop();//����ջ�� 
						t1=opn.top();//��ȡ��ǰ������ջ��Ԫ��
						opn.pop();//����ջ��
						t=op.top();//��ȡ��ǰ������ջ��Ԫ��
						op.pop();	//����ջ��
						t->kind=expre;//���ʽ���ڵ����� 
						t->child=t1;//t1��Ϊt�ĺ���
						t1->sibling=t2;//t1���ֵܽڵ�Ϊt2
						opn.push(t);	//���ӳɹ���,���ڵ�ָ���������ջ 
					} 
					else{
						printf("\nError:Expected correct expresion;Located on line No.%d\nnear chararctor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
						errorflag=1;//���ô����� 
					} 
					break;
				 }//�������ȼ��͵ĳ�ջ���� 
				 
				case 1:{	//��ǰ���ȼ��ߣ���ջ 
					syntaxtree z=new syntaxnode; //Ϊ�ò����������ڴ�ռ� 
					if(z==NULL)		//�жϿռ��Ƿ�����ɹ�
					{
						//�����Ϣ 
						printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
						getchar();getchar();	//�ȴ��û���Ӧ 
						exit(0);
					 }  
					z->listindex=index;	//������tokenlist�е�����
					z->child=NULL;	//��ʼ�����ӽڵ�
					z->sibling=NULL;//��ʼ���ֵܽڵ�
					z->kind=exp;	//���ýڵ�����Ϊ���ʽ��ɲ���
					op.push(z);		//�ò�������ջ
					index++;		//ȡ��һ���� 
					w=tokenlist[index].tokentype;//����w 
					break;
				}//�������ȼ��ߵĽ�ջ���� 
			 }
		  }
		  else	if(w==SEMI||w==BRACKETR)	//����ǷֺŻ���)�����ǽ�����
		  	w=SPEC;	//w����#
		  else //�������ž�Ϊ��
		  {
			  	printf("\nError:Expected correct expression;Located on line No.%d;\nnear charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
			  	errorflag=1;
		   } 
	 }//end of while 
	 if(opn.size()==1&&op.top()->kind==spec&&!errorflag)//������ջֻʣһ����������ֻʣ#���޴���
	 	return opn.top();	//���ز�����ջ�����һ��Ԫ�� 
	 else 
	 { 
	 	errorflag=1;	//��������1 
	 	printf("\nError:Expected correct expression;Located on line No.%d\nahead of charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
	 	return NULL; 
	 } 
 } 
/************************************************
�������ܣ��������ڵľֲ���������
**********************************************/
syntaxtree Lexer::LocalVar()
{
	syntaxtree p=new syntaxnode;//Ϊ�ֲ������ڵ�����ռ� 
	if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
	{
		//�����Ϣ 
		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
		getchar();getchar();	//�ȴ��û���Ӧ 
		exit(0);
	 } 
	p->child=NULL;	//��ʼ�����ӽڵ�
	p->sibling=NULL;//��ʼ���ֵܽڵ�
	p->kind=locvardecla;//���ýڵ������ʶ����Ϊlocvardecla 
	syntaxtree q=p;		//�����ʼp�ĵ�ַ 
	while((1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<=6)||tokenlist[index].tokentype==CONST)
	{
		if(tokenlist[index].tokentype==CONST)	//�����const 
			index++; 	//��������
		if(p->kind==id||p->kind==array)	//����ڵ������Ǳ�ʶ����������,�ǵ�һ�δ���ֲ����� 
		{
			while(p->sibling)	//�ҵ����һ���ֵܽڵ� 
			{
				p=p->sibling;//�ƶ�����һ���ֵܽڵ� 
			}
			p->sibling=TypeSpecifier();//������������������
			if(errorflag) //���������������� 
			{
				return NULL;
			}
			else
			{
				p=p->sibling;//�ƶ�����һ���ֵܽڵ� 
				p->sibling=Identifier(1);//���ñ�ʶ�������� 
				if(p->sibling==NULL)
					return NULL ;
				while(tokenlist[index].tokentype==COMMA&&!errorflag)//���������Ĳ�ֹһ������
				{
					index++;//���˶���
					while(p&&p->sibling)	//�Ƶ������ֵܽڵ㴦 
						p=p->sibling;	//p�ƶ����ֵܽڵ㴦
					p->sibling=Identifier(1);//���ñ�ʶ�������� 
				 } 
				if(errorflag)	//�����������
					return NULL;
				else if(tokenlist[index].tokentype!=SEMI)	//�ж��Ƿ��зֺ�
				{
					errorflag=1;
					printf("\Error:Expected a ';' on line No.%d;near charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
					return NULL; 
				 }
				 while(p&&p->sibling) 
				 	p=p->sibling;	//�ƶ����ֵܽڵ� 
				 index++;//���˵��ֺ� 
			 }
		}
		else	//��һ�δ���ֲ�����
		{
			p->child=TypeSpecifier();//������������������
			if(p->child==NULL) //���������������� 
			{
				return NULL;
			}
			else
			{
				p=p->child; 
				p->sibling=Identifier(1);//���ñ�ʶ�������� 
				if(p->sibling==NULL)
					return NULL ;
				while(tokenlist[index].tokentype==COMMA&&!errorflag)//���������Ĳ�ֹһ������
				{
					index++;//���˶���
					while(p&&p->sibling)	//�Ƶ������ֵܽڵ㴦 
						p=p->sibling;	//p�ƶ����ֵܽڵ㴦
					p->sibling=Identifier(1);//���ñ�ʶ�������� 
				 } 
				if(errorflag)	//�����������
					return NULL;
				else if(tokenlist[index].tokentype!=SEMI)	//�ж��Ƿ��зֺ�
				{
					errorflag=1;
					printf("\Error:Expected a ';' on line No.%d;near charactor '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
					return NULL; 
				 } 
				while(p&&p->sibling)
					p=p->sibling;//�Ƶ��������ֵܽڵ㴦 
				index++;//�����˷ֺ� 
			}
		 } 
	}//end of while
	return q;	//���س�ʼ��ַq 
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
		if(errorflag) 	//���������λ����1
			return ERROR;	//˵���βδ�������˴��󣬷��ش���ֵ 
		syntaxtree p=new syntaxnode;//Ϊ�����β����нڵ�����ռ�
		if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		if(!q)	//˵���β�Ϊ��
		{
			p->kind=nonfuncparam;//�����β����нڵ�����ʶ����Ϊnonfuncparam
			p->child=NULL;	//�亢�ӽڵ���ΪNULL
			p->sibling=NULL;//���ֵܽڵ���ΪNULL
			T->sibling=p;	//�ýڵ�Ϊ�����βε��ֵܽڵ� 
			return OK;
		 } 
		else 
		{
			p->kind=funcparam;//�����β����нڵ�����ʶ����Ϊfuncparam
			p->child=q;	//�亢�ӽڵ���Ϊq
			p->sibling=NULL;//���ֵܽڵ���ΪNULL
			T->sibling=p;	//�ýڵ�Ϊ�����βε��ֵܽڵ� 
			return OK;
		 }  
	}
} 
/*******************************************************
�������ܣ��βδ����� 
��ʱindexָ�����(����һλ 
*******************************************************/
syntaxtree Lexer::Parameter()
{
	while(tokenlist[index].tokentype!=BRACKETR) 	//�����������ž�˵�����β�����,���д��� 
	{
		syntaxtree p=NULL;//�����м�ָ��p 
		p=TypeSpecifier();	//���������ڵ���Ϊ�ֵ�
		syntaxtree q=p;		//�������������ڵ���Ϊ����ֵ 
		if(p)	//�������������ȷ,��P��ΪNULL 
		{ 
			p->sibling=Identifier(1); //ʶ���ʶ���������β����������ݲ���1 
			if(p->sibling)
			{
				if(tokenlist[index].tokentype==COMMA)	//�����������,˵���к������β� 
					index++;	//���Ź��� 
				if(p->sibling->sibling)	//�ж��Ƿ��������βνڵ���� 
					p->sibling->sibling->sibling=Parameter();//�ݹ��������
				else
					p->sibling->sibling=Parameter();//�ݹ�������� 
				return q;	//��������ڵ�ĵ�ַ 
			} 
			else			//�ݹ����������NULL,˵�����ִ��� 
				return NULL; 
		} 
		else  
			return NULL;	//��������ʶ����򷵻�NULL��˵���βε���������������,����NULL 
	}
	return NULL;	//while��û�з��أ��˳�while�ͷ���NULL 
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
	if(tokenlist[index].tokentype==CONST)//�����const������ȷ����
	{
		index++; //�ƶ�����һλ�� 
	 } 	//����const������������ 
	if(1<=tokenlist[index].tokentype&&tokenlist[index].tokentype<6)//�������ͨ��������
	{
		p->kind=type;	//�������ͽڵ�ʶ����Ϊtype
		p->listindex=index;	//������tokenlist�е�����
		index++;		//��������
		return p; 
	 }
	else		//�����������������,���Ǵ������������
	{
		printf("\nError:wrong typespecifier;\nLocatede on line No.%d,near character '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		errorflag=1;	//���ô����־Ϊ1 
		return NULL;	//����NULL 
	 } 
 } 
/**************************************************
�������ܣ�ʶ���ʶ�������ɶ�Ӧ�ڵ�,
������β�state�����������Ƿ�Ϊ�����β����� 
**************************************************/ 
syntaxtree Lexer::Identifier(int state)
{
	if(tokenlist[index].tokentype==ID)	//����Ǳ�ʶ��
	{
		syntaxtree p=new syntaxnode;	//Ϊ�������� 
		if(p==NULL)		//�жϿռ��Ƿ�����ɹ�
		{
			//�����Ϣ 
			printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
			getchar();getchar();	//�ȴ��û���Ӧ 
			exit(0);
		 } 
		p->kind=id;		//���ýڵ�����Ϊid ��ʶ�������� 
		p->child=NULL;	//���ӽڵ��ÿ�
		p->sibling=NULL;//�ֵܽڵ��ÿ�
		p->listindex=index;//������tokenlist�е�����
		index++;	//��������
		if(tokenlist[index].tokentype==PML)	//�ж��Ƿ�����������
		{
			if(state)	//���������β���������
			{
				int level=0;	//��¼�����ά�� 
				do{
					 index++;	//��������
					 if(tokenlist[index].tokentype!=PMR)
					{
						errorflag=1;//���ô�����
						printf("\nError:expected correct array parameter define;\nLocated on line No.%d;near characters'%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
						return NULL;//����NULL						 
					 } 
					index++; //�������� 
					level++;//ά������ 
				}while(tokenlist[index].tokentype==PML);//��������[�˳�ѭ�� 
				p->sibling=new syntaxnode;//Ϊ����ڵ�����ռ�
				if(p->sibling==NULL) //�жϿռ��Ƿ�����ɹ�
				{
					//�����Ϣ 
				printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
				getchar();getchar();	//�ȴ��û���Ӧ 
				exit(0);
			 	}  
			 	p->sibling->child=NULL;//��ʼ�����ӽڵ�
				p->sibling->sibling=NULL;//��ʼ�� 
				p->sibling->listindex=level;//���������ά�� 
				p->sibling->kind=array;//���ýڵ��ʶ����Ϊarray 
				return p; 
			 } 
			else	//�Ǻ����β�����
			{
				int level=0;	//��¼�����ά��
				do{
					 index++;	//��������
					 if(tokenlist[index].tokentype!=PMR)
					{
						errorflag=1;//���ô�����
						printf("\nError:expected correct array parameter define;\nLocated on line No.%d;near characters'%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str()); 
						return NULL;//����NULL						 
					 } 
					index=index+2; //�������� 
					level++;//ά������ 
				}while(tokenlist[index].tokentype==PML);//��������[�˳�ѭ�� 
				p->sibling=new syntaxnode;//Ϊ����ڵ�����ռ�
				if(p->sibling==NULL) //�жϿռ��Ƿ�����ɹ�
				{
					//�����Ϣ 
				printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
				getchar();getchar();	//�ȴ��û���Ӧ 
				exit(0);
			 	}  
			 	p->sibling->child=NULL;//��ʼ�����ӽڵ�
				p->sibling->sibling=NULL;//��ʼ�� 
				p->sibling->listindex=level;//���������ά�� 
				p->sibling->kind=array;//���ýڵ��ʶ����Ϊarray 
				return p;  
			 } 
		}
		return p;		//���ظýڵ��ַ 
	}
	  
	else	//���Ǳ�ʶ�����Ǵ��� 
	{
		printf("\nError:expected an Identifier;Located on line No.%d;\nnear characters'%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		errorflag=1;//���ô�����Ϊ1 
		return NULL;	//����NULL 
	}
 } 
/****************************************************
�������ܣ��ⲿ��������
******************************************************/
status Lexer::ExVarDeclaration()
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
	p->kind=exvardecla;	//�ýڵ�ı�ʶ���趨Ψexfuncdecla
	p->sibling=NULL;		//�ֵܽڵ���ΪNULL
	T->sibling=p;			//p�ڵ���Ϊ�β�T���ֵܽڵ� 
	p->child=TypeSpecifier();//�������ʹ����� 
	syntaxtree q=p->child;	//�����м����ָ�� 
	do{
		while(q&&q->sibling)	//�ҵ����һ���ֵܽڵ�
			q=q->sibling;		//�ƶ�����һ���ֵܽڵ� 
		q->sibling=Identifier(0);//���ñ�ʶ��������
		if(q->sibling==NULL)	//�����ʶ��������ΪNULL 
			return ERROR;	//���ش���ֵ		 
	}while(tokenlist[index].tokentype==COMMA);//�ж���,˵���Ƕ�������� 
	if(tokenlist[index].tokentype==SEMI)	//����˳���Ĵ�ǡ���Ƿֺ�
	{
		index++;	//��������
		return OK;	//������ȷֵ 
	 } 
	else
	{
		printf("\nError:Expected ';' after var declaration;Located on line No.%d;\nnear character '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
		return ERROR;
	 } 

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
	 root=NULL;		//ֻ���ͷ����ڴ�ռ�,ָ�벻��,����ָ��ָ��NULL
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
	   
	    case nonfuncparam:{	//������β����нڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("�����β����У���"); 
			break;
		} 
		
	    case funcparam:{	//����ǿ��β����нڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("�����β����У�"); 
			break;
		} 
		
		case type:{	//�����������������ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			if(tokenlist[p->listindex-1].tokentype==CONST)//���ǰһλ��const 
				printf("����������const %s",tokenlist[p->listindex].tokenstring.c_str()); 
			else	//ǰһλ����const 
				printf("����������%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
		
		case id:{	//�����ʶ�����������ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("��������%s",tokenlist[p->listindex].tokenstring.c_str());
			break;
		} 
		
		case array:{	//�����������ͽڵ�
			printf(" %dά����",p->listindex);//��������ά�� 
			break;
		} 
		
		case exvardecla:{	//�����ⲿ���������ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("�ⲿ���������ڵ㣺"); 
			break;
		}
		
		case funcdef:{	//����������ڵ�
			PrintSpace(step);	//���ǰ�ÿո�
			printf("��������ڵ㣺"); 
			break;
		} 
		
		case compstmd:{	//���������ڵ�
			PrintSpace(step);	//���ǰ�ÿո�
			printf("������䣺"); 
			break;
		}
		
		case locvardecla:{ //����ֲ����������ڵ�
			PrintSpace(step);	//���ǰ�ÿո�
			printf("�ֲ�����������"); 
			break;
		}
		
		case statelist:{	//����������нڵ�
			PrintSpace(step);	//���ǰ�ÿո�
			printf("�������:"); 
			break;
		}
		
		case breaknode:{	//����break�ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("break"); 
			break;
		}
		
		case exp:{	//������ʽ��ɲ��ֽڵ�
			PrintSpace(step);	//���ǰ�ÿո�
			printf("%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
		
		case expre:{//������ʽ�������ڵ�
			PrintSpace(step);	//���ǰ�ÿո�
			printf("���ʽ��");
			PrintSpace(step);
			printf("%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
		
		case returnnode:{	//����return���
			PrintSpace(step);	//���ǰ�ÿո�
			printf("�������return��"); 
			break;
		}
		
		case continuenode:{	//����continue
			PrintSpace(step);	//���ǰ�ÿո�
			printf("continue"); 
			break;
		}
		
		case ifnode:{	//������if��
			PrintSpace(step);	//���ǰ�ÿո�
			printf("��֧�ṹ if��"); 
			break;
		}
		
		case ifelsenode:{	//����ifelse��
			PrintSpace(step);	//���ǰ�ÿո�
			printf("��֧�ṹ if-else��"); 
			break;
		}
		
		case ifjudge:{	//���������ڵ�
			PrintSpace(step);
			printf("�����䣺"); 
			break;
		}
		
		case ifcmsd:{	//����if�Ӿ�
			PrintSpace(step);
			printf("if�Ӿ䣺"); 
			break;
		}
		
		case elsenode:{	//����else�ڵ�
			PrintSpace(step);
			printf("else��"); 
			break;
		}
		
		case fornode:{	//����for�ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("forѭ����䣺"); 
			break;
		}
		
		case forstart:{	//����for��ʼ�����ڵ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("for��ʼ���ʽ��"); 
			if(p->child==NULL)	//�ж��Ƿ�Ϊ��
				printf("��"); 
			break;
		}
		
		case forjudge:{	//����for�жϾ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("forѭ���жϱ��ʽ��"); 
			if(p->child==NULL)//�ж��Ƿ�Ϊ��
				printf("��"); 
			break;
		}
		
		
		case forchange:{//����for�������
			PrintSpace(step);//���ǰ�ÿո�
			printf("for�������ʽ��");
			if(p->child==NULL)	//�ж��Ƿ�Ϊ��
				printf("��"); 
			break;
		}
		
		case forstate:{	//����for�Ӿ�
			PrintSpace(step);//���ǰ�ÿո�
			printf("for�Ӿ䣺"); 
			if(p->child==NULL)	//�ж��Ƿ�Ϊ��
				printf("��"); 
			break;
		}
		
		case callfunc:{	//��������
			PrintSpace(step);	//���ǰ�ÿո�
			printf("����%s����",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		}
		
		case callmeter:{	//���������õ��β�
			PrintSpace(step);	//���ǰ�ÿո�
			printf("�����βΣ�%s",tokenlist[p->listindex].tokenstring.c_str()); 
			break;
		} 
		
	 }//end of switch
  } 
/***********************************************************
�������ܣ��ж���������ȼ�
**************************************************************/
int Lexer::JudgeLevel(int index,TokenType w)
{
	if(oplevel[tokenlist[index].tokentype-37]<=oplevel[w-37])	//�����ǰ��������ȼ��� 
		return -1;
	if(oplevel[tokenlist[index].tokentype-37]>oplevel[w-37])	//�����ǰ���ȼ��� 
		return  1; 
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
