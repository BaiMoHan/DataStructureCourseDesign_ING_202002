#ifndef LEXICAL_ANALYSIS_CPP
#define LEXICAL_ANALYSIS_CPP
#include "Lexical_analysis.h"
#include <stdio.h>
#include <iostream>	//C++的输入输出流 
#include <iomanip>	//C++的格式控制
#define width 15 
int judge_ch_id(char ch);//判断是否是构成标识符的函数 
Lexer::Lexer()
{//Lexer类构造函数 
	linecount=0;	//初始化行号 
	vectorindex=0;	//初始化tokenlist索引 
	state=0;		//自动机初始状态 
//	IDIndex=0;		//标识符数组索引初始化
//	KeyIndex=0;		//关键字数组索引初始化 
//	ConstIndex=0;	//常量数组索引初始化 
//	OperatorIndex=0;//操作数数组索引初始化 
//	CommentIndex=0; //注释数组索引初始化 
	DFAflag=1;		//初始化DFA分析标记为成功标记1
	//提示用户输入文件名 
	printf("请输入要分析的源程序文件名(不超过30个字符),filename=");
	//读取用户输入的文件名 
	scanf("%s",filename);
	//输入调用词法识别器前的提示信息 
	printf("需要分析的源程序文件为%s",filename);
	printf("\n开始进行词法分析：\n");
	//调用词法识别器 
	analysis(filename);
	if(DFAflag)			//如果词法分析成功
		 PrintWords();	//打印词法分析的结果 

 } 
void Lexer::Disp()
{
	printf("请输入需要打开的文件名(不超过30个字符),filename=");
	scanf("%s",filename);
	printf("%s",filename);
}
void Lexer::analysis(char filename[])
{//词法分析成员函数 
	printf("需要分析的源程序文件为%s",filename);
	printf("\n开始进行词法分析：\n");
	FILE* fp;			//声明一个文件指针 
	fp=fopen(filename,"r");//以只读的方式打开源程序文件 
	if(fp==NULL)		//判断文件是否打开失败 
	{
		printf("文件打开失败,请重新打开程序");//文件打开失败提示信息 
		getchar();getchar();				//等待用户输入字符,留出信息显示时间 
		exit(-1) ; 				//关闭程序 
	}
	int i=0;
	char ch;					//定义一个字符中间变量 
	string str="";				//定义识别字符序列存放变量 
	token t;	//定义中间变量t,为了存放至tokenlist中
	while((ch=fgetc(fp))!=EOF)	//当未读到文件末尾时，进行状态自动机识别 			fseek(fp,-1,SEEK_CUR);
	{
		if(ch=='\n')		//处理回车
		{
			linecount++;	//行数自增
			continue;		//进行下一次循环 
		 } //状态结束 
		else if(ch==' ')	//处理空格
			continue;		//进入下一次循环 
		else if(ch=='_')	//处理开头是下划线,只可能是标识符
		{
			do{
				str+=ch;	//拼接字符串
				ch=fgetc(fp); //读取下一个字符 
			}while(judge_ch_id(ch));
			if(ch!=-1)		//未到文件末尾 
				fseek(fp,-1,SEEK_CUR);	//标识符的识别多读了一个字符，将文件指针前移一位 
			t.linenum=linecount;//更新行号 
			t.times=counttimes(str);//该标识符此时是第几次出现 
			t.tokenstring=str;	//存放标识符自身的字符串 
			t.tokentype=ID;		//标识符的字符码 
			tokenlist.push_back(t);//将该标识符放进tokenlist中去 
			vectorindex++;		//tokenlist索引自增 
			str="";				//重置拼接字符串 
		} //状态结束 
		else if(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))	//处理开头是字母的情况,可能是标识符,也可能是关键字 
		{
			int kflag=0;	//标记是否带有下划线或者数字 
			do{
				str+=ch;	//拼接字符串
				ch=fgetc(fp);//读取下一个字符 
				if(ch=='_'||('1'<=ch&&ch<='9'))
					kflag=1;	//标记带有下划线或数字，则肯定不是关键字 
			}while(judge_ch_id(ch));
			if(ch!=-1)		//如果不是到末尾 
				fseek(fp,-1,SEEK_CUR);	//退出循环时多读了一个字符，将文件指针前移一位 
			if(kflag)	//kflag不为0说明是一定是标识符
			{
				t.linenum=linecount;//更新行号 
				t.times=counttimes(str);//该标识符此时是第几次出现 
				t.tokenstring=str;	//存放标识符自身的字符串 
				t.tokentype=ID;		//标识符的字符码 
				tokenlist.push_back(t);//将该标识符放进tokenlist中去 
				Id.push_back(vectorindex);//将tokenlist中的索引放入标识符记录表中 
				vectorindex++;		//tokenlist索引自增 
				str="";				//重置拼接字符串 
			 } 
			 else		//kflag为0说明可能是关键字也可能是标识符 
			 {
			 	t.linenum=linecount;//更新行号 
				t.times=counttimes(str);//该关键字此时是第几次出现 
				t.tokenstring=str;	//存放关键字自身的字符串 
				t.tokentype=gettokentype(str);		//关键字的识别码 
				tokenlist.push_back(t);//将该标识符放进tokenlist中去 
				if(t.tokentype==ID)		//如果识别码是标识符 
					Id.push_back(vectorindex);		//将tokenlist中的索引放入标识符记录表中
				else					//不是标识符就是关键字 
					Key.push_back(vectorindex);		//将tokenlist中的索引放入关键字记录表中 
				vectorindex++;		//tokenlist索引自增 
				str="";				//重置拼接字符串 
			  } 
		 }//状态结束 
		 else if('0'<=ch&&ch<='9') 	//处理数字常量 
		 {
//		 	int flag=0;	//标记符 
		 	do{
		 		str+=ch;	//拼接数字常量
				ch=fgetc(fp);//读取下一个字符 
				if(ch=='.')		//遇到小数点，可能是浮点数
				{
					do{ 
						str+=ch;	//拼接数字常量
						ch=fgetc(fp);	//继续读下一个字符 
					 }while('0'<=ch&&ch<='9'); //遇到不是数字就是其他字符 自动退出
					 if(ch=='f'||ch=='F')	//后缀为f或者F,说明是float型常量
					 {
					 	str+=ch;	//将后缀加入拼接成完整常量 
					 	t.linenum=linecount;	//保存该flaot常量的行号
						t.times=0;			//保存该常量出现的次数为0，常量不需要记录出现次数 
						t.tokenstring=str;		//保存该常量完整的字符串形式
						t.tokentype=FLOAT_CONST;//保存float常量的识别码
						tokenlist.push_back(t);	//加入tokenlist中去
						Const.push_back(vectorindex);//将该浮点数的常量索引保存
						cout<<tokenlist[vectorindex].tokenstring;
						vectorindex++;		//tokenlist索引自增 
						str="";			//重置拼接字符串 
					  } //处理后缀f或者F状态结束 
//					if(flag==1)		//说明小数点后没有符合的字符，还是不合法的
//					{ 
//						printf("ERROR:expect digit after '.'"); //输出错误类型
//						printf(";located at line No.%d",linecount);//输出错误行号
//						cout<<"\""<<str<<"\""<<" near";			//输出错误处的字符串 
//					} 
//					if(ch!=-1)		//如果没有读到文件末尾
//						fseek(fp,-1,SEEK_CUR);	//多读的字符需要回退 
				}//处理读到小数点状态结束 
				if(ch=='l'||ch=='L')	//处理long型后缀的情况
				{
					str+=ch;		//将后缀加入拼接成完整常量表达形式
					t.linenum=linecount;//保存该常量的行号
					t.times=0;//保存该常量出现的次数为0，常量不需要记录出现次数 
					t.tokenstring=str;//保存该常量完整的字符串形式
					t.tokentype=LONG;//保存该常量long型常量的识别码 
					tokenlist.push_back(t);//加入到tokenlist中去
					Const.push_back(vectorindex);//将该整型常量的索引保存 
					cout<<tokenlist[vectorindex].tokenstring;
					vectorindex++;		//tokenlist索引自增 
					str="";			//重置拼接字符串 
				 } //处理long型状态结束 
			 }while('0'<=ch&&ch<='9');	//不是数字字符就不再读取字符 
			 //处理普通的int型
			if(ch!=-1)			//如果未读到文件尾
			 	fseek(fp,-1,SEEK_CUR);		//多读的字符需要回退
			t.linenum=linecount;
			t.times=0;	//保存该常量出现的次数为0，常量不需要记录出现次数 
			t.tokenstring=str;//保存该常量完整的字符串形式
			t.tokentype=INT;//保存该常量long型常量的识别码 
			tokenlist.push_back(t);//加入到tokenlist中去
			Const.push_back(vectorindex);//将该整型常量的索引保存
			vectorindex++;		//tokenlist索引自增  
			str="";			//重置拼接字符串 
		 }//处理数字型常量状态结束 
	//	switch(ch){		//处理其他字符情况 
			
	//	}
	}//end of while(ch!=EOF)
	fclose(fp); 
}
int judge_ch_id(char ch)
{	//标识符后面的字母可能是数字 大小写字母 
	return (ch=='_'||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||'1'<=ch&&ch<='9')&&EOF;
}
int Lexer::counttimes(string str)
{
	int i,count=1;	//设置循环变量，计数器变量，标识符一开始是第一次出现 
	for(i=0;i<tokenlist.size();i++)	//从表头开始找,一直到表尾 
	{
		if(str==tokenlist[i].tokenstring)
			count++;	//出现次数自增 
	 } 
	return count; 
 } 
TokenType Lexer::gettokentype(string str)
{		//获取str对应的识别码 
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
	int i;	//循环变量
	printf("--------------------------------------------------");
	printf("\n标识符：\n"); 
	cout<<Const.size();
	for(i=0;i<Id.size();i++)		//按顺序输出所有识别到的标识符 
	{ 
		cout<<std::left<<setw(width)<<tokenlist[Id[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词 
			cout<<endl;
	} 
	printf("\n--------------------------------------------------\n");
	printf("\n关键字：\n"); 
	for(i=0;i<Key.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Key[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n");
	printf("--------------------------------------------------\n");
	printf("\n常量：\n"); 
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
////		if((i+1)%3==0)				//每一行显示三个词
////			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n");
	printf("--------------------------------------------------\n");
	printf("\n操作符：\n"); 
	for(i=0;i<Operator.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Operator[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n");
	printf("--------------------------------------------------\n");
	printf("\n注释：\n"); 
	for(i=0;i<Comment.size();i++)
	{
		cout<<tokenlist[Comment[i]].tokenstring<<endl;
	 } 
	printf("-----------------------------------------------------\n");
}
#endif
