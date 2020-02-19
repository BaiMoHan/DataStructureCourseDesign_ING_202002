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
//	IDIndex=0;		//��ʶ������������ʼ��
//	KeyIndex=0;		//�ؼ�������������ʼ�� 
//	ConstIndex=0;	//��������������ʼ�� 
//	OperatorIndex=0;//����������������ʼ�� 
//	CommentIndex=0; //ע������������ʼ�� 
	DFAflag=1;		//��ʼ��DFA�������Ϊ�ɹ����1
	//��ʾ�û������ļ��� 
	printf("������Ҫ������Դ�����ļ���(������30���ַ�),filename=");
	//��ȡ�û�������ļ��� 
	scanf("%s",filename);
	//������ôʷ�ʶ����ǰ����ʾ��Ϣ 
	printf("��Ҫ������Դ�����ļ�Ϊ%s",filename);
	printf("\n��ʼ���дʷ�������\n");
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
	int i=0;
	char ch;					//����һ���ַ��м���� 
	string str="";				//����ʶ���ַ����д�ű��� 
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
//		 	int flag=0;	//��Ƿ� 
		 	do{
		 		str+=ch;	//ƴ�����ֳ���
				ch=fgetc(fp);//��ȡ��һ���ַ� 
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
						cout<<tokenlist[vectorindex].tokenstring;
						vectorindex++;		//tokenlist�������� 
						str="";			//����ƴ���ַ��� 
					  } //�����׺f����F״̬���� 
//					if(flag==1)		//˵��С�����û�з��ϵ��ַ������ǲ��Ϸ���
//					{ 
//						printf("ERROR:expect digit after '.'"); //�����������
//						printf(";located at line No.%d",linecount);//��������к�
//						cout<<"\""<<str<<"\""<<" near";			//������󴦵��ַ��� 
//					} 
//					if(ch!=-1)		//���û�ж����ļ�ĩβ
//						fseek(fp,-1,SEEK_CUR);	//������ַ���Ҫ���� 
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
					cout<<tokenlist[vectorindex].tokenstring;
					vectorindex++;		//tokenlist�������� 
					str="";			//����ƴ���ַ��� 
				 } //����long��״̬���� 
			 }while('0'<=ch&&ch<='9');	//���������ַ��Ͳ��ٶ�ȡ�ַ� 
			 //������ͨ��int��
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
	//	switch(ch){		//���������ַ���� 
			
	//	}
	}//end of while(ch!=EOF)
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
	cout<<Const.size();
	for(i=0;i<Id.size();i++)		//��˳���������ʶ�𵽵ı�ʶ�� 
	{ 
		cout<<std::left<<setw(width)<<tokenlist[Id[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������ 
			cout<<endl;
	} 
	printf("\n--------------------------------------------------\n");
	printf("\n�ؼ��֣�\n"); 
	for(i=0;i<Key.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Key[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n");
	printf("--------------------------------------------------\n");
	printf("\n������\n"); 
	int k=0;
	for(i=0;i<Const.size();i++)
	{
		printf("%s",tokenlist[Const[i]].tokenstring.c_str());

		if(k==i)
		{
			printf("\n");
			k++;
		}
//		cout<<tokenlist[Const[i]].tokenstring;
//		if(i<3)
//			printf("\n");
////		cout<<"xxx"; 
////		if((i+1)%3==0)				//ÿһ����ʾ������
////			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n");
	printf("--------------------------------------------------\n");
	printf("\n��������\n"); 
	for(i=0;i<Operator.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Operator[i]].tokenstring;
		if((i+1)%3==0)				//ÿһ����ʾ������
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n");
	printf("--------------------------------------------------\n");
	printf("\nע�ͣ�\n"); 
	for(i=0;i<Comment.size();i++)
	{
		cout<<tokenlist[Comment[i]].tokenstring<<endl;
	 } 
	printf("-----------------------------------------------------\n");
}
#endif
