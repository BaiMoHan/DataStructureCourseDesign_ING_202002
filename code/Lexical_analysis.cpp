#ifndef LEXICAL_ANALYSIS_CPP
#define LEXICAL_ANALYSIS_CPP
#include "Lexical_analysis.h"
#include <stdio.h>
#include <iostream>	//C++����������� 
#include <iomanip>	//C++�ĸ�ʽ����
#define width 15 
int judge_ch_id(char ch);//�ж��Ƿ��ǹ��ɱ�ʶ���ĺ��� 
Lexer::Lexer()
{//Lexer�๹�캯�� 
	linecount=0;	//��ʼ���к� 
	vectorindex=0;	//��ʼ��tokenlist���� 
	state=0;		//�Զ�����ʼ״̬ 
	DFAflag=1;		//��ʼ��DFA�������Ϊ�ɹ����1
	
	//��ʾ�û������ļ��� 
	printf("������Ҫ������Դ�����ļ���(������30���ַ�),filename=");
	//��ȡ�û�������ļ��� 
	scanf("%s",filename);

	//���ôʷ�ʶ���� 
	analysis(filename);
	if(DFAflag)			//����ʷ������ɹ�
		 PrintWords();	//��ӡ�ʷ������Ľ�� 

 } 
void Lexer::Disp()
{
	printf("��������Ҫ�򿪵��ļ���(������30���ַ�),filename=");
	scanf("%s",filename);
	printf("%s",filename);
}
void Lexer::analysis(char filename[])
{//�ʷ�������Ա���� 
	printf("��Ҫ������Դ�����ļ�Ϊ%s",filename);
	printf("\n��ʼ���дʷ�������\n");
	FILE* fp;			//����һ���ļ�ָ�� 
	fp=fopen(filename,"r");//��ֻ���ķ�ʽ��Դ�����ļ� 
	if(fp==NULL)		//�ж��ļ��Ƿ��ʧ�� 
	{
		printf("�ļ���ʧ��,�����´򿪳���");//�ļ���ʧ����ʾ��Ϣ 
		getchar();getchar();				//�ȴ��û������ַ�,������Ϣ��ʾʱ�� 
		exit(-1) ; 				//�رճ��� 
	}
	int i=0;					//ѭ������������ 
	char ch;					//����һ���ַ��м���� 
	string str="",tempstr;				//����ʶ���ַ����д�ű��� 
	token t;	//�����м����t,Ϊ�˴����tokenlist��
	while((ch=fgetc(fp))!=EOF)	//��δ�����ļ�ĩβʱ������״̬�Զ���ʶ�� 			fseek(fp,-1,SEEK_CUR);
	{
		if(ch=='\n')		//����س�
		{
			linecount++;	//��������
			continue;		//������һ��ѭ�� 
		 } //״̬���� 
		else if(ch==' ')	//����ո�
			continue;		//������һ��ѭ�� 
		else if(ch=='_')	//����ͷ���»���,ֻ�����Ǳ�ʶ��
		{
			do{
				str+=ch;	//ƴ���ַ���
				ch=fgetc(fp); //��ȡ��һ���ַ� 
			}while(judge_ch_id(ch));
			if(ch!=-1)		//δ���ļ�ĩβ 
				fseek(fp,-1,SEEK_CUR);	//��ʶ����ʶ������һ���ַ������ļ�ָ��ǰ��һλ 
			t.linenum=linecount;//�����к� 
			t.times=counttimes(str);//�ñ�ʶ����ʱ�ǵڼ��γ��� 
			t.tokenstring=str;	//��ű�ʶ��������ַ��� 
			t.tokentype=ID;		//��ʶ�����ַ��� 
			tokenlist.push_back(t);//���ñ�ʶ���Ž�tokenlist��ȥ 
			vectorindex++;		//tokenlist�������� 
			str="";				//����ƴ���ַ��� 
		} //״̬���� 
		else if(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))	//����ͷ����ĸ�����,�����Ǳ�ʶ��,Ҳ�����ǹؼ��� 
		{
			int kflag=0;	//����Ƿ�����»��߻������� 
			do{
				str+=ch;	//ƴ���ַ���
				ch=fgetc(fp);//��ȡ��һ���ַ� 
				if(ch=='_'||('1'<=ch&&ch<='9'))
					kflag=1;	//��Ǵ����»��߻����֣���϶����ǹؼ��� 
			}while(judge_ch_id(ch));
			if(ch!=-1)		//������ǵ�ĩβ 
				fseek(fp,-1,SEEK_CUR);	//�˳�ѭ��ʱ�����һ���ַ������ļ�ָ��ǰ��һλ 
			if(kflag)	//kflag��Ϊ0˵����һ���Ǳ�ʶ��
			{
				t.linenum=linecount;//�����к� 
				t.times=counttimes(str);//�ñ�ʶ����ʱ�ǵڼ��γ��� 
				t.tokenstring=str;	//��ű�ʶ��������ַ��� 
				t.tokentype=ID;		//��ʶ�����ַ��� 
				tokenlist.push_back(t);//���ñ�ʶ���Ž�tokenlist��ȥ 
				Id.push_back(vectorindex);//��tokenlist�е����������ʶ����¼���� 
				vectorindex++;		//tokenlist�������� 
				str="";				//����ƴ���ַ��� 
			 } 
			 else		//kflagΪ0˵�������ǹؼ���Ҳ�����Ǳ�ʶ�� 
			 {
			 	t.linenum=linecount;//�����к� 
				t.times=counttimes(str);//�ùؼ��ִ�ʱ�ǵڼ��γ��� 
				t.tokenstring=str;	//��Źؼ���������ַ��� 
				t.tokentype=gettokentype(str);		//�ؼ��ֵ�ʶ���� 
				tokenlist.push_back(t);//���ñ�ʶ���Ž�tokenlist��ȥ 
				if(t.tokentype==ID)		//���ʶ�����Ǳ�ʶ�� 
					Id.push_back(vectorindex);		//��tokenlist�е����������ʶ����¼����
				else					//���Ǳ�ʶ�����ǹؼ��� 
					Key.push_back(vectorindex);		//��tokenlist�е���������ؼ��ּ�¼���� 
				vectorindex++;		//tokenlist�������� 
				str="";				//����ƴ���ַ��� 
			  } 
		 }//״̬���� 
		 else if('0'<=ch&&ch<='9') 	//�������ֳ��� 
		 {
		 	int floatflag=0,longflag=0;	//��Ƿ� 
		 	do{
		 		str+=ch;	//ƴ�����ֳ���
				ch=fgetc(fp);//��ȡ��һ���ַ� 
			}while('0'<=ch&&ch<='9');	//���������ַ��Ͳ��ٶ�ȡ�ַ�
			if(ch=='.')		//����С���㣬�����Ǹ�����
			{ 
				do{ 
				str+=ch;	//ƴ�����ֳ���
				ch=fgetc(fp);	//��������һ���ַ� 
				 }while('0'<=ch&&ch<='9'); //�����������־��������ַ� �Զ��˳�
				 if(ch=='f'||ch=='F')	//��׺Ϊf����F,˵����float�ͳ���
				 {
				 	str+=ch;	//����׺����ƴ�ӳ��������� 
				 	t.linenum=linecount;	//�����flaot�������к�
					t.times=0;			//����ó������ֵĴ���Ϊ0����������Ҫ��¼���ִ��� 
					t.tokenstring=str;		//����ó����������ַ�����ʽ
					t.tokentype=FLOAT_CONST;//����float������ʶ����
					tokenlist.push_back(t);	//����tokenlist��ȥ
					Const.push_back(vectorindex);//���ø������ĳ�����������
					vectorindex++;		//tokenlist�������� 
					str="";			//����ƴ���ַ��� 
					floatflag=1;	//��Ƕ�float�ͽ����˴��� 
				  } //�����׺f����F״̬���� 
				}//�������С����״̬����   
			if(ch=='l'||ch=='L')	//����long�ͺ�׺�����
			{
				str+=ch;		//����׺����ƴ�ӳ��������������ʽ
				t.linenum=linecount;//����ó������к�
				t.times=0;//����ó������ֵĴ���Ϊ0����������Ҫ��¼���ִ��� 
				t.tokenstring=str;//����ó����������ַ�����ʽ
				t.tokentype=LONG;//����ó���long�ͳ�����ʶ���� 
				tokenlist.push_back(t);//���뵽tokenlist��ȥ
				Const.push_back(vectorindex);//�������ͳ������������� 
				vectorindex++;		//tokenlist�������� 
				str="";			//����ƴ���ַ���
				longflag=1;		//��Ƕ�long�ͽ����˴��� 
			 } //����long��״̬���� 
			else if(floatflag+longflag<1)	//��δ��float��long����ʱ������int��
			{
				if(ch!=-1)			//���δ�����ļ�β
				 	fseek(fp,-1,SEEK_CUR);		//������ַ���Ҫ����
				t.linenum=linecount;
				t.times=0;	//����ó������ֵĴ���Ϊ0����������Ҫ��¼���ִ��� 
				t.tokenstring=str;//����ó����������ַ�����ʽ
				t.tokentype=INT;//����ó���long�ͳ�����ʶ���� 
				tokenlist.push_back(t);//���뵽tokenlist��ȥ
				Const.push_back(vectorindex);//�������ͳ�������������
				vectorindex++;		//tokenlist��������  
				str="";			//����ƴ���ַ��� 
		 	}//���������ͳ���״̬����  
		}//�������ֳ���״̬����		//while('0'<=ch&&ch<='9');	//���������ַ��Ͳ��ٶ�ȡ�ַ� 
		
		else if(ch=='#')	//����ʱ#include<XXXX>���� 
		{
			for(i=0;i<7;i++)
			{
				ch=fgetc(fp);	//��ȡ8���ַ�,�����������#include�� 
				str+=ch;		//ע��#δ���� 
				if(ch==-1)		//�����ļ�ĩβ
					break;		//��ǰ�˳� 
			}
//			string tempstr;		//����һ����ʱstring����� 
			tempstr="include";	//��������ַ������ 
			if(str==tempstr)	//������ַ������
			{	
			 //�Ƚ��ؼ���include���� 
				t.linenum=linecount;	//����include���к�
				t.tokenstring=tempstr;	//����inlclude�����ַ���
				t.tokentype=INCLUDE;	//��������ʶ����
				t.times=0;				//�ؼ��ֲ���Ҫ��¼���ִ���
				tokenlist.push_back(t);	//����tokenlist��ȥ
				Key.push_back(vectorindex);//��¼include��tokenlist�е�����
				vectorindex++;			//tokenlist�������� 
				str="";					//����ƴ�Ӵ� 
				
				//�����ļ���������ʽ����
				ch=fgetc(fp);	//��ȡ��һ���ַ����������ò���
				while(ch==' ')	//����֮��Ŀո�
					ch=fgetc(fp);//��ȡ��һ���ַ�,֪���ǿո�Ϊֹ 
					
				if(ch=='<')	//ƥ������÷���,ǰ���÷���ƥ��ɹ� 
				{
					str+=ch;		//�����ÿ��ļ�ǰ���ż���ƴ�Ӵ��� 
					for(i=0;ch!='\n';i++)		//�������о��˳�,�������� 
					{
						ch=fgetc(fp);//��ȡ��һ���ַ�
						str+=ch;	//ƴ�� 
						if(ch=='>')	//Ҫ��ǰ���÷���ƥ��
							break;	//�˳�ѭ�� 
					 } 
					if(ch=='\n')	//����ǻ����˳��ģ�˵�����ÿ��ʽ����
					{
						printf("ERROR:Bad file name format in include directive;Located on line NO.%d\n",linecount);
						str="";		//����ƴ�� 
						fseek(fp,-1,SEEK_CUR);	//���س������� 
					 } 
					else 	//��Ϊƥ������˳���,˵����ʽ��ȷ
					{
						t.linenum=linecount;	//����<xxxx>���к�
						t.tokenstring=str;		//���¿���ʽ������
						t.times=0;				//����Ҫ��¼�������ֵĴ���
						t.tokentype=STRING_CONST;//�ѿ��������ַ�������������
						tokenlist.push_back(t);	//���뵽tokenlist��ȥ
						Const.push_back(vectorindex);//�����ַ���������tokenlist�еļ�¼
						vectorindex++;			//tokenlist��������
						str="";					//����ƴ�� 
					 } 
				}//��<�Ŀ���ƥ��״̬���� 
				
				else if(ch=='"')	//ƥ������÷���,ǰ���÷���ƥ��ɹ� 
				{
					str+=ch;		//�����ÿ��ļ�ǰ���ż���ƴ�Ӵ��� 
					for(i=0;ch!='\n';i++)		//�������о��˳�,�������� 
					{
						ch=fgetc(fp);//��ȡ��һ���ַ�
						str+=ch;	//ƴ�� 
						if(ch=='"')	//Ҫ��ǰ���÷���ƥ��
							break;	//�˳�ѭ�� 
					 } 
					if(ch=='\n')	//����ǻ����˳��ģ�˵�����ÿ��ʽ����
					{
						printf("ERROR:Bad file name format in include directive;Located on line NO.%d",linecount);
						str="";		//����ƴ�� 
						fseek(fp,-1,SEEK_CUR);	//���س������� 
					 } 
					else 	//��Ϊƥ������˳���,˵����ʽ��ȷ
					{
						t.linenum=linecount;	//����<xxxx>���к�
						t.tokenstring=str;		//���¿���ʽ������
						t.times=0;				//����Ҫ��¼�������ֵĴ���
						t.tokentype=STRING_CONST;//�ѿ��������ַ�������������
						tokenlist.push_back(t);	//���뵽tokenlist��ȥ
						Const.push_back(vectorindex);//�����ַ���������tokenlist�еļ�¼
						vectorindex++;			//tokenlist��������
						str="";					//����ƴ�� 
					 } 
				}//�ԡ����Ŀ���ƥ��״̬���� 
				else	//�����ø�ʽ���Դ���
				{
					printf("ERROR:Bad file name format in include directive;Located on line NO.%d",linecount);
				 } 
			}
			else 				//���ַ������ȣ�˵��#�ǷǷ��ı�ʶ��
			{	//����Ƿ��ַ���ʾ���� 
				printf("ERROR:Illegal character '#';Located on line No.%d",linecount); 
				str="";			//����ƴ�Ӵ� 
				rewind(fp);		//���ļ�ָ���Ƶ���ͷ
				ch=fgetc(fp);	//��ͷ#�Ѿ�����ֱ�Ӷ���һ���ַ� 
			 } 
//			fseek(fp,-i-1,SEEK_CUR);	//���ļ�ָ��ǰ��(i+1)����λ,�����ض�����ַ� 
		 }//����#״̬���� 
		
		//��������� 
		switch(ch){	//���������ַ���� 
			case '+':{
				str+=ch;	//����ƴ�� 
				ch=fgetc(fp);	//��ȡ��һ���ַ� 
				if(ch=='+')//��������ŵڶ����Ӻ�
				{
					str+=ch;	//�ڶ���+����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=ADDSELF;//������ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; //tokenlist�������� 
					str="";		//����ƴ�Ӵ� 
				 } 
				else if(ch=='=')//���������=
				{
					str+=ch;	//�ڶ���+����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=ADD_EQ;//+=��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; //tokenlist��������
					str="";			//����ƴ�Ӵ� 
				 } 
				else	//���ǽ�����+��= 
				{
					if(ch!=-1)	//���û�ж����ļ�ĩβ 
						fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=ADD;//+��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; 	//�������� 
					str="";			//����ƴ�Ӵ� 
				 } 
				break;
			}
		
		}		 
	}	//end of while(ch!=EOF)
	fclose(fp); 
}
int judge_ch_id(char ch)
{	//��ʶ���������ĸ���������� ��Сд��ĸ 
	return (ch=='_'||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||'1'<=ch&&ch<='9')&&EOF;
}
int Lexer::counttimes(string str)
{
	int i,count=1;	//����ѭ����������������������ʶ��һ��ʼ�ǵ�һ�γ��� 
	for(i=0;i<tokenlist.size();i++)	//�ӱ�ͷ��ʼ��,һֱ����β 
	{
		if(str==tokenlist[i].tokenstring)
			count++;	//���ִ������� 
	 } 
	return count; 
 } 
TokenType Lexer::gettokentype(string str)
{		//��ȡstr��Ӧ��ʶ���� 
	if(str=="int")		//int 
		return INT;
	else if(str=="long")//long
		return LONG;
	else if(str=="float")//float
		return FLOAT;
	else if(str=="void")//void
		return VOID;
	else if(str=="char")//char
		return CHAR;
	else if(str=="return")//return
		return RETURN;
	else if(str=="if")	//if
		return IF;
	else if(str=="else")//else
		return ELSE;
	else if(str=="while")//while
		return WHILE;
	else if(str=="for")	//for
		return FOR;
	else if(str=="break")//break
		return BREAK;
	else if(str=="continue")//continue
		return CONTINUE; 
	else if(str=="double")//double
		return DOUBLE;
	else if(str=="include")//include
		return INCLUDE; 
	else if(str=="const")//const
		return CONST;
	else					// 
		return ID;
}
void Lexer::PrintWords()
{
	int i;	//ѭ������
	printf("--------------------------------------------------");
	printf("\n��ʶ����\n"); 
	for(i=0;i<Id.size();i++)		//��˳���������ʶ�𵽵ı�ʶ�� 
	{ 
		cout<<std::left<<setw(width)<<tokenlist[Id[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������ 
			cout<<endl;
	} 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("�ؼ��֣�\n"); 
	for(i=0;i<Key.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Key[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("������\n"); 
	int k=0;
	for(i=0;i<Const.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Const[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("��������\n"); 
	for(i=0;i<Operator.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Operator[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("ע�ͣ�\n\n"); 
	for(i=0;i<Comment.size();i++)
	{
		cout<<tokenlist[Comment[i]].tokenstring<<endl;
	 } 
	printf("-----------------------------------------------------\n");
}
#endif
