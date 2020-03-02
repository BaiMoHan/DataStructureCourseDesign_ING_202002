#ifndef LEXICAL_ANALYSIS_CPP
#define LEXICAL_ANALYSIS_CPP
#include "Lexical_analysis.h"
#include <stdio.h>
#include <iostream>	//C++����������� 
#include <iomanip>	//C++�ĸ�ʽ����
#define width 15 
#define ERROR -1
int judge_ch_id(char ch);//�ж��Ƿ��ǹ��ɱ�ʶ���ĺ��� 


Lexer::Lexer()
{//Lexer�๹�캯�� 
	index=0;		//��ʼ��ȡ������ 
	linecount=1;	//��ʼ���к� 
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
	{ 
		 PrintWords();	//��ӡ�ʷ������Ľ�� 
		 printf("\n\n����Ϊ�ʷ�����������޴�����������﷨����"); 
		 Program();
	} 
	else
	{
		printf("�ʷ��������ִ����Ƿ�ѡ��鿴�ʷ����������");
		printf("����1����鿴���,0�����鿴�����\nchosee=");
		scanf("%d",&DFAflag);		//��ȡ�û�����Ľ�� 
		if(DFAflag)		//����������1
			PrintWords();//��ӡ�ʷ������Ľ��
		printf("���ڳ��ִ����޷����������﷨�������������ٴ򿪳��������\n"); 
	 } 

 } 


void Lexer::analysis(char filename[])
{	//�ʷ�������Ա���� 
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
		} //�»��߿�ͷ״̬���� 
		
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
		 }//��ĸ��ͷ״̬���� 
		 
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
					t.tokentype=FLOATCONST;//����float������ʶ����
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
		 	}//����int�ͳ���״̬����  
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
						DFAflag=0;	//���ִʷ����� 
						printf("ERROR:Bad file name format in include directive;Located on line NO.%d\n",linecount);
						str="";		//����ƴ�� 
						fseek(fp,-1,SEEK_CUR);	//���س������� 
					 } 
					else 	//��Ϊƥ������˳���,˵����ʽ��ȷ
					{
						t.linenum=linecount;	//����<xxxx>���к�
						t.tokenstring=str;		//���¿���ʽ������
						t.times=0;				//����Ҫ��¼�������ֵĴ���
						t.tokentype=STRINGCONST;//�ѿ��������ַ�������������
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
						DFAflag=0;	//���ִʷ����� 
						printf("ERROR:Bad file name format in include directive;Located on line NO.%d\n",linecount);
						str="";		//����ƴ�� 
						fseek(fp,-1,SEEK_CUR);	//���س������� 
					 } 
					else 	//��Ϊƥ������˳���,˵����ʽ��ȷ
					{
						t.linenum=linecount;	//����<xxxx>���к�
						t.tokenstring=str;		//���¿���ʽ������
						t.times=0;				//����Ҫ��¼�������ֵĴ���
						t.tokentype=STRINGCONST;//�ѿ��������ַ�������������
						tokenlist.push_back(t);	//���뵽tokenlist��ȥ
						Const.push_back(vectorindex);//�����ַ���������tokenlist�еļ�¼
						vectorindex++;			//tokenlist��������
						str="";					//����ƴ�� 
					 } 
				}//�ԡ����Ŀ���ƥ��״̬���� 
				else	//�����ø�ʽ���Դ���
				{
					DFAflag=0;	//���ִʷ����� 
					printf("ERROR:Bad file name format in include directive;Located on line NO.%d\n",linecount);
				 } 
			}
			else 				//���ַ������ȣ�˵��#�ǷǷ��ı�ʶ��
			{	//����Ƿ��ַ���ʾ���� 
				DFAflag=0;	//���ִʷ����� 
				printf("ERROR:Illegal character '#';Located on line No.%d\n",linecount); 
				str="";			//����ƴ�Ӵ� 
				rewind(fp);		//���ļ�ָ���Ƶ���ͷ
				ch=fgetc(fp);	//��ͷ#�Ѿ�����ֱ�Ӷ���һ���ַ� 
			 } 
//			fseek(fp,-i-1,SEEK_CUR);	//���ļ�ָ��ǰ��(i+1)����λ,�����ض�����ַ� 
		 }//����#״̬���� 
		
		//���������������Ͷ���� 
		else switch(ch){	//���������ַ���� 
		
			case '+':{		//����+ += ++ 
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
				 } //����++״̬���� 
				else if(ch=='=')//���������=
				{
					str+=ch;	//�ڶ���=����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=ADDEQ;//+=��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼+=��tokenlist�е�����
					vectorindex++; //tokenlist��������
					str="";			//����ƴ�Ӵ� 
				 } //����+=״̬���� 
				else	//���治�ǽ�����+��= 
				{
					if(ch!=-1)	//���û�ж����ļ�ĩβ 
						fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=ADD;//+��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼+��tokenlist�е�����
					vectorindex++; 	//�������� 
					str="";			//����ƴ�Ӵ� 
				 } //����+״̬���� 
				break;
			}	//���� + ++ += ״̬���� 
			
			case '-':{		//����- -= -- 
				str+=ch;	//����ƴ�� 
				ch=fgetc(fp);	//��ȡ��һ���ַ� 
				if(ch=='-')//��������ŵڶ����Ӻ�
				{
					str+=ch;	//�ڶ���-����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=REDUCESELF;//������ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼--��tokenlist�е�����
					vectorindex++; //tokenlist�������� 
					str="";		//����ƴ�Ӵ� 
				 } 	//����--״̬���� 
				else if(ch=='=')//���������=
				{
					str+=ch;	//�ڶ���=����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=REDUCEEQ;//+=��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼-=��tokenlist�е�����
					vectorindex++; //tokenlist��������
					str="";			//����ƴ�Ӵ� 
				 } 	//����-=״̬���� 
				else	//���ǽ�����-��= 
				{
					if(ch!=-1)	//���û�ж����ļ�ĩβ 
						fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=REDUCE;//-��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼-��tokenlist�е�����
					vectorindex++; 	//�������� 
					str="";			//����ƴ�Ӵ� 
				 } //���� - ״̬���� 
				break;
			}	//���� - -= -- ״̬���� 
			
			case '*':{	//����* *= ��� 
				str+=ch;	//����ƴ�� 
				ch=fgetc(fp);	//��ȡ��һ���ַ� 
				 if(ch=='=')//���������=
				{
					str+=ch;	//�ڶ���=����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=MUTIPLYEQ;//*=��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; //tokenlist��������
					str="";			//����ƴ�Ӵ� 
				 } //����*=״̬���� 
				else	//���ǽ�����= 
				{
					if(ch!=-1)	//���û�ж����ļ�ĩβ 
						fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=MUTIPLY; //*��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; 	//�������� 
					str="";			//����ƴ�Ӵ� 
				 } //����*״̬���� 
				break;
			}	//����* *= ״̬���� 
			
			case '%':{	//����% %= ��� 
				str+=ch;	//����ƴ�� 
				ch=fgetc(fp);	//��ȡ��һ���ַ� 
				 if(ch=='=')//���������=
				{
					str+=ch;	//�ڶ���=����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=MODEQ;//*=��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; //tokenlist��������
					str="";			//����ƴ�Ӵ� 
				 } //����*=״̬���� 
				else	//���ǽ�����= 
				{
					if(ch!=-1)	//���û�ж����ļ�ĩβ 
						fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=MOD; //*��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; 	//�������� 
					str="";			//����ƴ�Ӵ� 
				 } //����*״̬���� 
				break;
			}	//����* *= ״̬���� 
			
			case '/':{	//����* /= ��� �Լ� /* // ע������ 
				str+=ch;	//����ƴ�� 
				ch=fgetc(fp);	//��ȡ��һ���ַ� 
				
				 if(ch=='=')//���������=
				{
					str+=ch;	//�ڶ���=����ƴ��
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=DIVIDEEQ;//*=��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; //tokenlist��������
					str="";			//����ƴ�Ӵ� 
				 } //����/=״̬���� 
				 
				else if(ch=='*')	//����/*ע������ 
				{
					t.linenum=linecount;//����ע�͵���ʼ�� 
					do{
						str+=ch;	//ƴ��ע���ַ���
						ch=fgetc(fp);//��ȡ��һ���ַ�
						if(ch=='\n')//ͳ�ƻ���
							linecount++;	//�������� 
						if(ch=='/')		//�����*/��ʽ����ע����β����ʱ���ǰһλ�ַ��ǲ���*����
						{
							fseek(fp,-2,SEEK_CUR);	//�ļ�ָ�������λ
							if(fgetc(fp)=='*')		//����*/ע�ͽ�β
							{
								str+=ch;		//��ĩβ��/����ƴ�� 
								break;				//�˳�ע�͵�ѭ�� 
							 } 
							else				//δ����*/ע�ͽ�β 
							{
								fgetc(fp);		//���ļ�ָ���ԭ 
							 } 
						 } 
					}while(ch!=-1);
					if(ch==EOF)	//��ĩβ�˳��ģ�˵��ע��û�н���
					{
						printf("ERROR:Coments missing ending;Located on Line No.%d\n",linecount);
						DFAflag=0;	//���ִʷ����� 
						str="";		//����ƴ�Ӵ� 
					 } 
					else	//˵����break�˳���ע��û������
					{
						fgetc(fp);	//�ļ�ָ�����һλ����Ϊ֮ǰ��ǰ�ƶ�����λ 
						t.times=0;	//�����ڼ�¼ע�ͳ��ֵĴ��� 
						t.tokenstring=str;//����ƴ�Ӵ�
						t.tokentype=COMMENT;	//ע�͵�ʶ���� 
						tokenlist.push_back(t);//����tokenlist��ȥ
						Comment.push_back(vectorindex);//����ע����tokenlist�е�����
						vectorindex++;		//tokenlist��������
						str="";			//����ƴ�Ӵ� 
					 } 
				 } //����/*״̬���� 
				
				
				else if(ch=='/')	//����//ע������
				{
					do{
						str+=ch;	//ƴ��ע��
						ch=fgetc(fp);//������ע���ַ� 
					}while((ch!='\n')&&(ch!=-1));	//�������л����ļ�β����˵��ע�ͽ��� 
					t.linenum=linecount;	//ע���ʱδ�����к�
					t.times=0;				//ע�Ͳ���Ҫ��¼����
					t.tokenstring=str;		//����ע�͵�ԭʼֵ
					t.tokentype=COMMENT;	//ע�͵�ʶ����
					tokenlist.push_back(t);	//����tokenlist��ȥ
					Comment.push_back(vectorindex);//��¼ע����tokenlist�е�����
					vectorindex++;			//tokenlist�е��������� 
					linecount++;			//����ע�ͽ������������� 
					str="";					//����ƴ�Ӵ� 
				 } //����//ע��״̬���� 
				
				 
				else	//���ǽ�����= 
				{
					if(ch!=-1)	//���û�ж����ļ�ĩβ 
						fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					t.linenum=linecount;//�����к�
					t.times=0;		//����������Ҫ��¼����
					t.tokenstring=str;//����ƴ�ӵ�������
					t.tokentype=DIVIDE; //*��ʶ����
					tokenlist.push_back(t);//����tokenlist�� 
					Operator.push_back(vectorindex);//��¼++��tokenlist�е�����
					vectorindex++; 	//�������� 
					str="";			//����ƴ�Ӵ� 
				 } //����*״̬���� 
				break;
			}	//����* *= ״̬���� 
			
			case ',':{		//����,�ָ���
				str+=ch;				//ƴ�� 
				t.linenum=linecount;	//�����к�
				t.times=0;				//�ָ�������Ҫ��¼���� 
				t.tokenstring=str;		//����ƴ�Ӵ�
				t.tokentype=COMMA;		//Ӣ�İ�Ƕ��ŵ�ʶ����
				tokenlist.push_back(t);	//���뵽tokenlist��ȥ
				Delimiter.push_back(vectorindex);//��¼��tokenlist�е�����
				vectorindex++;			//tokenlist��������
				str="";					//����ƴ�Ӵ�
				ch=fgetc(fp); 			//��������һ���ַ� 
				if(ch!=',')
					fseek(fp,-1,SEEK_CUR);
				break;
			}//����,�ָ���״̬����
			
			case ';':{	//����;�ָ��� 
				str+=ch;				//ƴ�� 
				t.linenum=linecount;	//�����к�
				t.times=0;				//�ָ�������Ҫ��¼���� 
				t.tokenstring=str;		//����ƴ�Ӵ�
				t.tokentype=SEMI;		//Ӣ�İ�Ƿֺŵ�ʶ����
				tokenlist.push_back(t);	//���뵽tokenlist��ȥ
				Delimiter.push_back(vectorindex);//��¼��tokenlist�е�����
				vectorindex++;			//tokenlist��������
				str="";					//����ƴ�Ӵ�
				ch=fgetc(fp);			//��������һ���ַ�
				if(ch!=';')		
					fseek(fp,-1,SEEK_CUR); 
				break;
			} //����;�ָ���״̬���� 			
			
			case '&':{		//����&&�����,��������Ҫ�󲻿���&λ��������
				str+=ch;				//ƴ�� 
				ch=fgetc(fp);			//��������һ���ַ�
				if(ch=='&')				//����ܹ���&&
				{
					str+=ch;			//����ƴ�� 
					t.linenum=linecount;	//�����к�
					t.times=0;				//�ָ�������Ҫ��¼���� 
					t.tokenstring=str;		//����ƴ�Ӵ�
					t.tokentype=AND;		//Ӣ�İ�Ƿֺŵ�ʶ����
					tokenlist.push_back(t);	//���뵽tokenlist��ȥ
					Operator.push_back(vectorindex);//��¼��tokenlist�е�����
					vectorindex++;			//tokenlist��������
					str="";					//����ƴ�Ӵ� 
				 } 
				else		//���ܹ���&&,���Ǵ��� 
				{
					DFAflag=0;	//���ִʷ����� 
					fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					printf("ERROR:Undefined label &;Located on line No. %d",linecount);
					str="";		//����ƴ�Ӵ� 
				 } 
				break;
			}	//����&&״̬���� 
			
			case '|':{		//����&&�����,��������Ҫ�󲻿���&λ��������
				str+=ch;				//ƴ�� 
				ch=fgetc(fp);			//��������һ���ַ�
				if(ch=='|')				//����ܹ���&&
				{
					str+=ch;			//����ƴ�� 
					t.linenum=linecount;	//�����к�
					t.times=0;				//�ָ�������Ҫ��¼���� 
					t.tokenstring=str;		//����ƴ�Ӵ�
					t.tokentype=OR;		//Ӣ�İ�Ƿֺŵ�ʶ����
					tokenlist.push_back(t);	//���뵽tokenlist��ȥ
					Operator.push_back(vectorindex);//��¼��tokenlist�е�����
					vectorindex++;			//tokenlist��������
					str="";					//����ƴ�Ӵ� 
				 } 
				else		//���ܹ���&&,���Ǵ��� 
				{
					DFAflag=0;	//���ִʷ����� 
					fseek(fp,-1,SEEK_CUR);	//���˶�����ַ�
					printf("ERROR:Undefined label |;Located on line No. %d",linecount);
					str="";		//����ƴ�Ӵ� 
				 } 
				break;
			}	//����&&״̬���� 
			
			case  '{':{		//����������״̬ 
				str+=ch;	//����ƴ�� 
				t.linenum=linecount;	//�����к�
				t.times=counttimes(str);//���´���
				t.tokenstring=str;		//����ƴ�Ӵ�
				t.tokentype=LP;			//�����ŵ�ʶ����
				tokenlist.push_back(t);	//����tokenlist��ȥ
				Delimiter.push_back(vectorindex);//��¼tokenlist�и÷ָ���������
				vectorindex++;			//tokenlist��������
				str="";				//����ƴ�Ӵ� 
				break;
			} 	//����������״̬����
			
			case '}':{		//�����һ�����״̬ 
				str+=ch;	//����ƴ��
				t.linenum=linecount;	//�����к�
				t.times=counttimes(str);//���´���
				tempstr='{';		//����������
				if(counttimes(tempstr)==1)	//���֮ǰû�г���������
				{
					//�����},���������Ϣ 
					printf("ERROR:Character '}' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//����ƴ�Ӵ� 
				 } 
				else if(t.times>counttimes(tempstr))//���ǰ�����е�{����ƥ��
				{
					//�����},���������Ϣ
					printf("ERROR:Character '}' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//����ƴ�Ӵ� 	 
				 } 
				else	//��}�޴��󣬼���tokenlist��ȥ
				{
					t.tokenstring=str;	//����ƴ�Ӵ�
					t.tokentype=RP;		//����}��ʶ���� 
					tokenlist.push_back(t);//����tokenlist��ȥ
					Delimiter.push_back(vectorindex);//��¼�ָ�����tokenlist�е�ʶ����
					vectorindex++;		//tokenlist��������
					str="";				//����ƴ�Ӵ� 
				 } 
				break;
			} 	//�����һ�����״̬���� 
			
			case  '(':{		//������Բ����״̬ 
				str+=ch;	//����ƴ�� 
				t.linenum=linecount;	//�����к�
				t.times=counttimes(str);//���´���
				t.tokenstring=str;		//����ƴ�Ӵ�
				t.tokentype=BRACKETL;	//��Բ���ŵ�ʶ����
				tokenlist.push_back(t);	//����tokenlist��ȥ
				Delimiter.push_back(vectorindex);//��¼tokenlist�и÷ָ���������
				vectorindex++;			//tokenlist��������
				str="";				//����ƴ�Ӵ� 
				break;
			} 	//����������״̬����
			
			case ')':{		//������Բ����״̬ 
				str+=ch;	//����ƴ��
				t.linenum=linecount;	//�����к�
				t.times=counttimes(str);//���´���
				tempstr='(';		//������Բ����
				if(counttimes(tempstr)==1)	//���֮ǰû�г���������
				{
					//�����),���������Ϣ 
					printf("ERROR:Character ')' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//����ƴ�Ӵ� 
				 } 
				else if(t.times>counttimes(tempstr))//���ǰ�����е�{����ƥ��
				{
					//�����),���������Ϣ
					printf("ERROR:Character ')' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//����ƴ�Ӵ� 	 
				 } 
				else	//��)�޴��󣬼���tokenlist��ȥ
				{
					t.tokenstring=str;	//����ƴ�Ӵ�
					t.tokentype=BRACKETR;	//����)��ʶ���� 
					tokenlist.push_back(t);//����tokenlist��ȥ
					Delimiter.push_back(vectorindex);//��¼�ָ�����tokenlist�е�ʶ����
					vectorindex++;		//tokenlist��������
					str="";				//����ƴ�Ӵ� 
				 } 
				break;
			} 	//������Բ����״̬���� 
			
			case  '[':{		//������Բ����״̬ 
				str+=ch;	//����ƴ�� 
				t.linenum=linecount;	//�����к�
				t.times=counttimes(str);//���´���
				t.tokenstring=str;		//����ƴ�Ӵ�
				t.tokentype=PML;	//�����ŵ�ʶ����
				tokenlist.push_back(t);	//����tokenlist��ȥ
				Delimiter.push_back(vectorindex);//��¼tokenlist�и÷ָ���������
				vectorindex++;			//tokenlist��������
				str="";				//����ƴ�Ӵ� 
				break;
			} 	//����������״̬����
			
			case ']':{		//������Բ����״̬ 
				str+=ch;	//����ƴ��
				t.linenum=linecount;	//�����к�
				t.times=counttimes(str);//���´���
				tempstr='[';		//����������
				if(counttimes(tempstr)==1)	//���֮ǰû�г���������
				{
					//�����],���������Ϣ 
					printf("ERROR:Character ']' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//����ƴ�Ӵ� 
				 } 
				else if(t.times>counttimes(tempstr))//���ǰ�����е�{����ƥ��
				{
					//�����],���������Ϣ
					printf("ERROR:Character ']' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//����ƴ�Ӵ� 	 
				 } 
				else	//��]�޴��󣬼���tokenlist��ȥ
				{
					t.tokenstring=str;	//����ƴ�Ӵ�
					t.tokentype=PMR;	//����]��ʶ���� 
					tokenlist.push_back(t);//����tokenlist��ȥ
					Delimiter.push_back(vectorindex);//��¼�ָ�����tokenlist�е�ʶ����
					vectorindex++;		//tokenlist��������
					str="";				//����ƴ�Ӵ� 
				 } 
				break;
			} 	//�����ҷ�����״̬���� 
			
			case '<':{	//����С�ں�״̬��ʼ 
				str+=ch;	//����ƴ�� 
				t.linenum=linecount;	//�����к�
				t.times=0;				//����������Ҫ��¼���ִ���
				t.tokenstring=str;		//����������
				t.tokentype=LESS;		//С�ںŵı�ʶ��
				tokenlist.push_back(t);	//����tokenlist��ȥ
				Operator.push_back(vectorindex);//����ò�������tokenlist�е�����
				vectorindex++;			//tokenlist��������
				str="";					//����ƴ�Ӵ� 
				break;
			}		//����С�ں�״̬����
			
			case '>':{	//������ں�״̬��ʼ 
				str+=ch;	//����ƴ�� 
				t.linenum=linecount;	//�����к�
				t.times=0;				//����������Ҫ��¼���ִ���
				t.tokenstring=str;		//����������
				t.tokentype=GREATER;		//С�ںŵı�ʶ��
				tokenlist.push_back(t);	//����tokenlist��ȥ
				Operator.push_back(vectorindex);//����ò�������tokenlist�е�����
				vectorindex++;			//tokenlist��������
				str="";					//����ƴ�Ӵ� 
				break;
			}		//������ں�״̬���� 
			
			case  '\'':{	//��������״̬,Ӣ�����浥����������֮�֣���Ϊ�������� 
				str+=ch;		//��ƴ�ӳ������� 
				t.linenum=linecount;//�����к�
				t.times=0;			//��������Ҫ��¼���ֵĴ���
				t.tokenstring=str;	//���³�����������
				t.tokentype=SQUTOE_L;//�����ŵ�ʶ����
				tokenlist.push_back(t);//���뵽tokenlist��ȥ
				Delimiter.push_back(vectorindex);//�����ַ�������tokenlist������
				vectorindex++;		//tokenlist��������	 
				do{
					ch=fgetc(fp);	//��ȡ��һ���ַ� 
					str+=ch;	//����ƴ��
					if(ch=='\'')//ƥ���һ������ 
						break; //��������ƥ��ɹ����˳� 
				}while(ch!='\n'&&ch!=';');//�������л��߷ֺž��˳� 
				if(ch=='\'')	//�������Ϊƥ�䵽�����˳�
				{
					if(str.size()>3)	//������ǵ��ַ�
					{
						//���������Ϣ 
						printf("ERROR:%s is not the char-const;Located on line No.%d\n",str.c_str(),linecount);
						str="";		//����ƴ�Ӵ� 
					 }
					else 	//�����ַ������ĸ�ʽ
					{
						t.linenum=linecount;//�����к�
						t.times=0;			//��������Ҫ��¼���ֵĴ���
						t.tokenstring=str;	//���³�����������
						t.tokentype=CHARCONST;//�ַ�������ʶ����
						tokenlist.push_back(t);//���뵽tokenlist��ȥ
						Const.push_back(vectorindex);//�����ַ�������tokenlist������
						vectorindex++;		//tokenlist��������
						str="";				//����ƴ�Ӵ� 
						str='\'';			//�������������ţ���Ϊ����������֮��
						t.linenum=linecount;//�����к�
						t.times=0;			//��������Ҫ��¼���ֵĴ���
						t.tokenstring=str;	//���³�����������
						t.tokentype=SQUTOE_R;//�ҵ����ŵ�ʶ����
						tokenlist.push_back(t);//���뵽tokenlist��ȥ
						Delimiter.push_back(vectorindex);//�����ַ�������tokenlist������
						vectorindex++;		//tokenlist��������	
						str="";			//����ƴ�Ӵ�	 
					 } 
				}
				else		//��Ϊ���л��߷ֺ��˳���
				{
					//������ɶԳ��ֵ�ƥ�������Ϣ 
					printf("ERROR:Character ' is lack of matching;Located on line No.%d\n",linecount);
					fseek(fp,-1,SEEK_CUR);//�ļ�ָ����� 
					str="";			//����ƴ�Ӵ� 
				 } 
				break;
			} 	//��������״̬����
			
			case  '\"':{	//����˫����״̬,Ӣ�����浥����������֮�֣���Ϊ�������� 
				str+=ch;		//��ƴ�ӳ������� 
				t.linenum=linecount;//�����к�
				t.times=0;			//��������Ҫ��¼���ֵĴ���
				t.tokenstring=str;	//���³�����������
				t.tokentype=DQUTOE_L;//�����ŵ�ʶ����
				tokenlist.push_back(t);//���뵽tokenlist��ȥ
				Delimiter.push_back(vectorindex);//�����ַ�������tokenlist������
				vectorindex++;		//tokenlist��������	 
				do{
					ch=fgetc(fp);	//��ȡ��һ���ַ� 
					str+=ch;	//����ƴ��
					if(ch=='\"')//ƥ���һ������ 
						break; //��������ƥ��ɹ����˳� 
				}while(ch!='\n'&&ch!=';');//�������л��߷ֺž��˳� 
				if(ch=='\"')	//�������Ϊƥ�䵽�����˳�
				{
					t.linenum=linecount;//�����к�
					t.times=0;			//��������Ҫ��¼���ֵĴ���
					t.tokenstring=str;	//���³�����������
					t.tokentype=CHARCONST;//�ַ�������ʶ����
					tokenlist.push_back(t);//���뵽tokenlist��ȥ
					Const.push_back(vectorindex);//�����ַ�������tokenlist������
					vectorindex++;		//tokenlist��������
					str="";				//����ƴ�Ӵ� 
					str='\"';			//�������������ţ���Ϊ����������֮��
					t.linenum=linecount;//�����к�
					t.times=0;			//��������Ҫ��¼���ֵĴ���
					t.tokenstring=str;	//���³�����������
					t.tokentype=DQUTOE_R;//�ҵ����ŵ�ʶ����
					tokenlist.push_back(t);//���뵽tokenlist��ȥ
					Delimiter.push_back(vectorindex);//�����ַ�������tokenlist������
					vectorindex++;		//tokenlist��������	
					str="";			//����ƴ�Ӵ�	 	  
				}
				else		//��Ϊ���л��߷ֺ��˳���
				{
					//������ɶԳ��ֵ�ƥ�������Ϣ 
					printf("ERROR:Character \" is lack of matching;Located on line No.%d\n",linecount);
					fseek(fp,-1,SEEK_CUR);//�ļ�ָ����� 
					str="";			//����ƴ�Ӵ� 
				 } 
				break;
			} 	//����˫����״̬����
			
			case '=':{	//����= == ״̬
				str+=ch;	//ƴ�Ӵ�
				if((ch=fgetc(fp))=='=')	//�ж��ǲ���==
				{
					str+=ch;	//ƴ�ӳ�==
					t.linenum=linecount;//�����к� 
					t.times=0;	//����Ҫ��¼���������ֵĴ���
					t.tokenstring=str;//����ƴ�Ӵ�
					t.tokentype=EQ;		//==��ʶ����
					tokenlist.push_back(t);//���뵽tokenlist��ȥ
					Operator.push_back(vectorindex);//����==��tokenlist�е�����
					vectorindex++;	//tokenlist��������
					str="";			//����ƴ�Ӵ� 
				 } 
				else 		//���˶�����ַ�
				{
					if(ch!=EOF)	//������Ƕ����ļ�β�� 
						fseek(fp,-1,SEEK_CUR);//���ļ�ָ�����һ�� 
					t.linenum=linecount;//�����к�
					t.times=0;	//����������Ҫͳ�Ƴ��ִ���
					t.tokenstring=str;	//����ƴ�Ӵ�
					t.tokentype=ASSIGN;	//����=��ʶ����
					tokenlist.push_back(t);//����tokenlist��ȥ
					Operator.push_back(vectorindex);//�����������tokenlist�е�����
					vectorindex++;		//tokenlist��������
					str="";				//����ƴ�Ӵ� 
				 } 
				
				break;
			} 
			
			case '!':{		//����!=״̬��ʼ
				str+=ch;	//����ƴ��
				ch=fgetc(fp);	//������һ���ַ�
				if(ch=='=')		//�����һ���ַ���=�����ܹ���!=
				{
					str+=ch;	//����ƴ��
					t.linenum=linecount;	//�����к�
					t.times=0;		//����������Ҫͳ�Ƴ��ִ���
					t.tokenstring=str;	//��������ƴ�Ӵ�
					t.tokentype=NOTEQ;	//!=��ʶ����
					tokenlist.push_back(t);	//����tokenlist��ȥ
					Operator.push_back(vectorindex);//������tokenlist�е�����
					vectorindex++;		//tokenlist��������
					str="";				//����ƴ�Ӵ� 
				 } 
				else		//���ܹ���!=
				{
					//���ǷǷ��ַ� 
					printf("ERROR:Illegal character '!';Located on line No.%d\n",linecount); 
					fseek(fp,-1,SEEK_CUR);	//���˶�����ַ� 
				 } 
				break;
			}
			
			
			default :{		//�����Ƿ��ַ� 
				printf("ERROR:Illegal character '%c';Located on line No.%d\n",ch,linecount); 
				DFAflag=0;		//�ʷ��������ִ��� 
				break;
			}
		
		} //end of switch	
			 
	}	//end of while(ch!=EOF)
	t.linenum=linecount+1;	//����ʱ������һ��
	t.times=0;				//������ǲ���Ҫ��¼
	t.tokenstring=ENDFILE;	//��������
	str='\0';				//�����ַ�
	t.tokenstring=str;		//��������ַ� 
	tokenlist.push_back(t);	//����tokenlist��ȥ��Ϊ������ֹ 
	
	fclose(fp); 	//�������ر��ļ� 
	
}	//end of analysis 


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
	else if(str=="main")//main
		return MAIN; 
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
	printf("\n--------------------------------------------------\n");
	printf("�����(�ָ���)��\n"); 
	for(i=0;i<Delimiter.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Delimiter[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("ע�ͣ�\n"); 
	for(i=0;i<Comment.size();i++)
	{
		cout<<tokenlist[Comment[i]].tokenstring<<endl;
	 } 
	printf("-----------------------------------------------------\n");
}


 
//void Lexer::Program()	//�����﷨����������ʼ
//{
//	root=new syntaxnode; 
//	if(root==NULL)	//�ж��ڴ��Ƿ�����ɹ�
//	{
//		//�����Ϣ 
//		printf("�ڴ�����ʧ�ܣ�\n�ڴ治�����Զ��ر�\n");
//		getchar();getchar();	//�ȴ��û���Ӧ 
//		exit(0);
//	 } 
//	root->kind=rt;	//���ڵ������� 
//	root->child=NULL;//��ʼ������
//	root->sibling=NULL;//��ʼ���ֵܽ�� 
//	root->listindex=-1;	//��ʼ������ 
//
//	if(DeclarationList(root)!=ERROR)			//������������ 
//	{
//		printf("\n\n�﷨����ӡ���ϣ�");
//	}
//	else
//	{
//		printf("\n\n�����﷨���������﷨��ʧ��"); 
//	 } 
//		 
//	  
// } 
 
 
//int Lexer::DeclarationList(syntaxtree &root)	//�������д����� 
//{
//	while(tokenlist[index].tokentype==COMMENT)	//���˵�ע��
//		index++;
//	if(tokenlist[index].tokentype>=1&&tokenlist[index].tokentype<=6) //������������� 
//	{
//		if(tokenlist[index+1].tokentype==ID)	//��������ʱ��϶��Ǳ�ʶ��
//		{
////			Declaration();				//�������������� 
//		}
//		else //��������֮���Ǳ�ʶ����������Ǵ���
//		{
//			printf("ERROR:type-specifier needs Identifier;Locatied on line No.%d '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
//			printf("�﷨���������������Զ��ͷſռ�");
//			printf("���������������﷨��\n");
//			DeleteTree(root);		//�ͷ����ռ� 
//			return ERROR; 			//����ERRORֵ 
//		 } 
//	}//���������������� 
//	else if(tokenlist[index].tokentype==CONST)		//const�ͺ������������
//	{
//		
//	 }
//	
// } 
// 

 
 
 
 
 
 
 
 
 
#endif
