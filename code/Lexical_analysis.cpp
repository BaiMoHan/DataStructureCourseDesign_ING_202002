#ifndef LEXICAL_ANALYSIS_CPP
#define LEXICAL_ANALYSIS_CPP
#include "Lexical_analysis.h"
#include <stdio.h>
#include <iostream>	//C++的输入输出流 
#include <iomanip>	//C++的格式控制
#define width 15 
#define ERROR -1
int judge_ch_id(char ch);//判断是否是构成标识符的函数 


Lexer::Lexer()
{//Lexer类构造函数 
	index=0;		//初始化取词索引 
	linecount=1;	//初始化行号 
	vectorindex=0;	//初始化tokenlist索引 
	state=0;		//自动机初始状态 
	DFAflag=1;		//初始化DFA分析标记为成功标记1
	
	//提示用户输入文件名 
	printf("请输入要分析的源程序文件名(不超过30个字符),filename=");
	//读取用户输入的文件名 
	scanf("%s",filename);

	//调用词法识别器 
	analysis(filename);
	if(DFAflag)			//如果词法分析成功
	{ 
		 PrintWords();	//打印词法分析的结果 
		 printf("\n\n上述为词法分析结果，无错误，下面进行语法分析"); 
		 Program();
	} 
	else
	{
		printf("词法分析出现错误！是否选择查看词法分析结果？");
		printf("输入1代表查看结果,0代表不查看结果：\nchosee=");
		scanf("%d",&DFAflag);		//读取用户输入的结果 
		if(DFAflag)		//如果输入的是1
			PrintWords();//打印词法分析的结果
		printf("由于出现错误，无法继续进行语法分析，请纠错后再打开程序分析！\n"); 
	 } 

 } 


void Lexer::analysis(char filename[])
{	//词法分析成员函数 
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
	
	int i=0;					//循环计数器变量 
	char ch;					//定义一个字符中间变量 
	string str="",tempstr;				//定义识别字符序列存放变量 
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
		} //下划线开头状态结束 
		
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
		 }//字母开头状态结束 
		 
		 else if('0'<=ch&&ch<='9') 	//处理数字常量 
		 {
		 	int floatflag=0,longflag=0;	//标记符 
		 	do{
		 		str+=ch;	//拼接数字常量
				ch=fgetc(fp);//读取下一个字符 
			}while('0'<=ch&&ch<='9');	//不是数字字符就不再读取字符
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
					t.tokentype=FLOATCONST;//保存float常量的识别码
					tokenlist.push_back(t);	//加入tokenlist中去
					Const.push_back(vectorindex);//将该浮点数的常量索引保存
					vectorindex++;		//tokenlist索引自增 
					str="";			//重置拼接字符串 
					floatflag=1;	//标记对float型进行了处理 
				  } //处理后缀f或者F状态结束 
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
				vectorindex++;		//tokenlist索引自增 
				str="";			//重置拼接字符串
				longflag=1;		//标记对long型进行了处理 
			 } //处理long型状态结束 
			else if(floatflag+longflag<1)	//当未被float和long捕获时，处理int型
			{
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
		 	}//处理int型常量状态结束  
		}//处理数字常量状态结束		//while('0'<=ch&&ch<='9');	//不是数字字符就不再读取字符 
		
		else if(ch=='#')	//考虑时#include<XXXX>声明 
		{
			for(i=0;i<7;i++)
			{
				ch=fgetc(fp);	//读取8个字符,理想情况读到#include处 
				str+=ch;		//注意#未加入 
				if(ch==-1)		//读到文件末尾
					break;		//提前退出 
			}
			tempstr="include";	//存放理想字符串情况 
			if(str==tempstr)	//如果两字符串相等
			{	
			 //先将关键字include加入 
				t.linenum=linecount;	//更新include的行号
				t.tokenstring=tempstr;	//更新inlclude自身字符串
				t.tokentype=INCLUDE;	//更新自身识别码
				t.times=0;				//关键字不需要记录出现次数
				tokenlist.push_back(t);	//加入tokenlist中去
				Key.push_back(vectorindex);//记录include在tokenlist中的索引
				vectorindex++;			//tokenlist索引自增 
				str="";					//重置拼接串 
				
				//考虑文件名引用形式错误
				ch=fgetc(fp);	//读取下一个字符，即库引用部分
				while(ch==' ')	//过滤之间的空格
					ch=fgetc(fp);//读取下一个字符,知道非空格为止 
					
				if(ch=='<')	//匹配库引用符号,前引用符号匹配成功 
				{
					str+=ch;		//将引用库文件前括号加入拼接串中 
					for(i=0;ch!='\n';i++)		//遇到换行就退出,库名过滤 
					{
						ch=fgetc(fp);//读取下一个字符
						str+=ch;	//拼接 
						if(ch=='>')	//要与前引用符号匹配
							break;	//退出循环 
					 } 
					if(ch=='\n')	//如果是换行退出的，说明引用库格式错误
					{
						DFAflag=0;	//出现词法错误 
						printf("ERROR:Bad file name format in include directive;Located on line NO.%d\n",linecount);
						str="";		//重置拼接 
						fseek(fp,-1,SEEK_CUR);	//将回车符回退 
					 } 
					else 	//因为匹配符号退出的,说明格式正确
					{
						t.linenum=linecount;	//更新<xxxx>的行号
						t.tokenstring=str;		//更新库形式完整串
						t.times=0;				//不需要记录库名出现的次数
						t.tokentype=STRINGCONST;//把库名当成字符串常量来处理
						tokenlist.push_back(t);	//加入到tokenlist中去
						Const.push_back(vectorindex);//保留字符串常量在tokenlist中的记录
						vectorindex++;			//tokenlist索引自增
						str="";					//重置拼接 
					 } 
				}//对<的库名匹配状态结束 
				
				else if(ch=='"')	//匹配库引用符号,前引用符号匹配成功 
				{
					str+=ch;		//将引用库文件前括号加入拼接串中 
					for(i=0;ch!='\n';i++)		//遇到换行就退出,库名过滤 
					{
						ch=fgetc(fp);//读取下一个字符
						str+=ch;	//拼接 
						if(ch=='"')	//要与前引用符号匹配
							break;	//退出循环 
					 } 
					if(ch=='\n')	//如果是换行退出的，说明引用库格式错误
					{
						DFAflag=0;	//出现词法错误 
						printf("ERROR:Bad file name format in include directive;Located on line NO.%d\n",linecount);
						str="";		//重置拼接 
						fseek(fp,-1,SEEK_CUR);	//将回车符回退 
					 } 
					else 	//因为匹配符号退出的,说明格式正确
					{
						t.linenum=linecount;	//更新<xxxx>的行号
						t.tokenstring=str;		//更新库形式完整串
						t.times=0;				//不需要记录库名出现的次数
						t.tokentype=STRINGCONST;//把库名当成字符串常量来处理
						tokenlist.push_back(t);	//加入到tokenlist中去
						Const.push_back(vectorindex);//保留字符串常量在tokenlist中的记录
						vectorindex++;			//tokenlist索引自增
						str="";					//重置拼接 
					 } 
				}//对“”的库名匹配状态结束 
				else	//库引用格式不对错误
				{
					DFAflag=0;	//出现词法错误 
					printf("ERROR:Bad file name format in include directive;Located on line NO.%d\n",linecount);
				 } 
			}
			else 				//两字符串不等，说明#是非法的标识符
			{	//输出非法字符提示错误 
				DFAflag=0;	//出现词法错误 
				printf("ERROR:Illegal character '#';Located on line No.%d\n",linecount); 
				str="";			//重置拼接串 
				rewind(fp);		//将文件指针移到开头
				ch=fgetc(fp);	//开头#已经处理，直接读下一个字符 
			 } 
//			fseek(fp,-i-1,SEEK_CUR);	//将文件指针前移(i+1)个单位,即返回多读的字符 
		 }//处理#状态结束 
		
		//处理其他操作符和定界符 
		else switch(ch){	//处理其他字符情况 
		
			case '+':{		//处理+ += ++ 
				str+=ch;	//进行拼接 
				ch=fgetc(fp);	//读取下一个字符 
				if(ch=='+')//后面紧跟着第二个加号
				{
					str+=ch;	//第二个+加入拼接
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=ADDSELF;//自增的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录++在tokenlist中的索引
					vectorindex++; //tokenlist索引自增 
					str="";		//重置拼接串 
				 } //处理++状态结束 
				else if(ch=='=')//后面紧跟着=
				{
					str+=ch;	//第二个=加入拼接
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=ADDEQ;//+=的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录+=在tokenlist中的索引
					vectorindex++; //tokenlist索引自增
					str="";			//重置拼接串 
				 } //处理+=状态结束 
				else	//后面不是紧跟着+或= 
				{
					if(ch!=-1)	//如果没有读到文件末尾 
						fseek(fp,-1,SEEK_CUR);	//回退多读的字符
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=ADD;//+的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录+在tokenlist中的索引
					vectorindex++; 	//索引自增 
					str="";			//重置拼接串 
				 } //处理+状态结束 
				break;
			}	//处理 + ++ += 状态结束 
			
			case '-':{		//处理- -= -- 
				str+=ch;	//进行拼接 
				ch=fgetc(fp);	//读取下一个字符 
				if(ch=='-')//后面紧跟着第二个加号
				{
					str+=ch;	//第二个-加入拼接
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=REDUCESELF;//自增的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录--在tokenlist中的索引
					vectorindex++; //tokenlist索引自增 
					str="";		//重置拼接串 
				 } 	//处理--状态结束 
				else if(ch=='=')//后面紧跟着=
				{
					str+=ch;	//第二个=加入拼接
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=REDUCEEQ;//+=的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录-=在tokenlist中的索引
					vectorindex++; //tokenlist索引自增
					str="";			//重置拼接串 
				 } 	//处理-=状态结束 
				else	//不是紧跟着-或= 
				{
					if(ch!=-1)	//如果没有读到文件末尾 
						fseek(fp,-1,SEEK_CUR);	//回退多读的字符
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=REDUCE;//-的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录-在tokenlist中的索引
					vectorindex++; 	//索引自增 
					str="";			//重置拼接串 
				 } //处理 - 状态结束 
				break;
			}	//处理 - -= -- 状态结束 
			
			case '*':{	//处理* *= 情况 
				str+=ch;	//进行拼接 
				ch=fgetc(fp);	//读取下一个字符 
				 if(ch=='=')//后面紧跟着=
				{
					str+=ch;	//第二个=加入拼接
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=MUTIPLYEQ;//*=的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录++在tokenlist中的索引
					vectorindex++; //tokenlist索引自增
					str="";			//重置拼接串 
				 } //处理*=状态结束 
				else	//不是紧跟着= 
				{
					if(ch!=-1)	//如果没有读到文件末尾 
						fseek(fp,-1,SEEK_CUR);	//回退多读的字符
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=MUTIPLY; //*的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录++在tokenlist中的索引
					vectorindex++; 	//索引自增 
					str="";			//重置拼接串 
				 } //处理*状态结束 
				break;
			}	//处理* *= 状态结束 
			
			case '%':{	//处理% %= 情况 
				str+=ch;	//进行拼接 
				ch=fgetc(fp);	//读取下一个字符 
				 if(ch=='=')//后面紧跟着=
				{
					str+=ch;	//第二个=加入拼接
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=MODEQ;//*=的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录++在tokenlist中的索引
					vectorindex++; //tokenlist索引自增
					str="";			//重置拼接串 
				 } //处理*=状态结束 
				else	//不是紧跟着= 
				{
					if(ch!=-1)	//如果没有读到文件末尾 
						fseek(fp,-1,SEEK_CUR);	//回退多读的字符
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=MOD; //*的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录++在tokenlist中的索引
					vectorindex++; 	//索引自增 
					str="";			//重置拼接串 
				 } //处理*状态结束 
				break;
			}	//处理* *= 状态结束 
			
			case '/':{	//处理* /= 情况 以及 /* // 注释问题 
				str+=ch;	//进行拼接 
				ch=fgetc(fp);	//读取下一个字符 
				
				 if(ch=='=')//后面紧跟着=
				{
					str+=ch;	//第二个=加入拼接
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=DIVIDEEQ;//*=的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录++在tokenlist中的索引
					vectorindex++; //tokenlist索引自增
					str="";			//重置拼接串 
				 } //处理/=状态结束 
				 
				else if(ch=='*')	//处理/*注释问题 
				{
					t.linenum=linecount;//保存注释的起始行 
					do{
						str+=ch;	//拼接注释字符串
						ch=fgetc(fp);//读取下一个字符
						if(ch=='\n')//统计换行
							linecount++;	//行数自增 
						if(ch=='/')		//如果是*/形式就是注释收尾，此时检查前一位字符是不是*即可
						{
							fseek(fp,-2,SEEK_CUR);	//文件指针回退两位
							if(fgetc(fp)=='*')		//构成*/注释结尾
							{
								str+=ch;		//把末尾的/加入拼接 
								break;				//退出注释的循环 
							 } 
							else				//未构成*/注释结尾 
							{
								fgetc(fp);		//将文件指针归原 
							 } 
						 } 
					}while(ch!=-1);
					if(ch==EOF)	//到末尾退出的，说明注释没有截至
					{
						printf("ERROR:Coments missing ending;Located on Line No.%d\n",linecount);
						DFAflag=0;	//出现词法错误 
						str="";		//重置拼接串 
					 } 
					else	//说明是break退出，注释没有问题
					{
						fgetc(fp);	//文件指针后移一位，因为之前往前移动了两位 
						t.times=0;	//不存在记录注释出现的次数 
						t.tokenstring=str;//保存拼接串
						t.tokentype=COMMENT;	//注释的识别码 
						tokenlist.push_back(t);//存入tokenlist中去
						Comment.push_back(vectorindex);//保存注释在tokenlist中的索引
						vectorindex++;		//tokenlist索引自增
						str="";			//重置拼接串 
					 } 
				 } //处理/*状态结束 
				
				
				else if(ch=='/')	//处理//注释问题
				{
					do{
						str+=ch;	//拼接注释
						ch=fgetc(fp);//继续读注释字符 
					}while((ch!='\n')&&(ch!=-1));	//遇到换行或者文件尾部就说明注释结束 
					t.linenum=linecount;	//注意此时未更新行号
					t.times=0;				//注释不需要记录次数
					t.tokenstring=str;		//存入注释的原始值
					t.tokentype=COMMENT;	//注释的识别码
					tokenlist.push_back(t);	//加入tokenlist中去
					Comment.push_back(vectorindex);//记录注释在tokenlist中的索引
					vectorindex++;			//tokenlist中的索引自增 
					linecount++;			//改行注释结束，行数自增 
					str="";					//重置拼接串 
				 } //处理//注释状态结束 
				
				 
				else	//不是紧跟着= 
				{
					if(ch!=-1)	//如果没有读到文件末尾 
						fseek(fp,-1,SEEK_CUR);	//回退多读的字符
					t.linenum=linecount;//更新行号
					t.times=0;		//操作符不需要记录次数
					t.tokenstring=str;//更新拼接的完整串
					t.tokentype=DIVIDE; //*的识别码
					tokenlist.push_back(t);//加入tokenlist中 
					Operator.push_back(vectorindex);//记录++在tokenlist中的索引
					vectorindex++; 	//索引自增 
					str="";			//重置拼接串 
				 } //处理*状态结束 
				break;
			}	//处理* *= 状态结束 
			
			case ',':{		//处理,分隔符
				str+=ch;				//拼接 
				t.linenum=linecount;	//更新行号
				t.times=0;				//分隔符不需要记录次数 
				t.tokenstring=str;		//更新拼接串
				t.tokentype=COMMA;		//英文半角逗号的识别码
				tokenlist.push_back(t);	//加入到tokenlist中去
				Delimiter.push_back(vectorindex);//记录在tokenlist中的索引
				vectorindex++;			//tokenlist索引自增
				str="";					//重置拼接串
				ch=fgetc(fp); 			//继续读下一个字符 
				if(ch!=',')
					fseek(fp,-1,SEEK_CUR);
				break;
			}//处理,分隔符状态结束
			
			case ';':{	//处理;分隔符 
				str+=ch;				//拼接 
				t.linenum=linecount;	//更新行号
				t.times=0;				//分隔符不需要记录次数 
				t.tokenstring=str;		//更新拼接串
				t.tokentype=SEMI;		//英文半角分号的识别码
				tokenlist.push_back(t);	//加入到tokenlist中去
				Delimiter.push_back(vectorindex);//记录在tokenlist中的索引
				vectorindex++;			//tokenlist索引自增
				str="";					//重置拼接串
				ch=fgetc(fp);			//继续读下一个字符
				if(ch!=';')		
					fseek(fp,-1,SEEK_CUR); 
				break;
			} //处理;分隔符状态结束 			
			
			case '&':{		//处理&&运算符,本次任务要求不考虑&位运算符情况
				str+=ch;				//拼接 
				ch=fgetc(fp);			//继续读下一个字符
				if(ch=='&')				//如果能构成&&
				{
					str+=ch;			//完整拼接 
					t.linenum=linecount;	//更新行号
					t.times=0;				//分隔符不需要记录次数 
					t.tokenstring=str;		//更新拼接串
					t.tokentype=AND;		//英文半角分号的识别码
					tokenlist.push_back(t);	//加入到tokenlist中去
					Operator.push_back(vectorindex);//记录在tokenlist中的索引
					vectorindex++;			//tokenlist索引自增
					str="";					//重置拼接串 
				 } 
				else		//不能构成&&,则是错误 
				{
					DFAflag=0;	//出现词法错误 
					fseek(fp,-1,SEEK_CUR);	//回退多读的字符
					printf("ERROR:Undefined label &;Located on line No. %d",linecount);
					str="";		//重置拼接串 
				 } 
				break;
			}	//处理&&状态结束 
			
			case '|':{		//处理&&运算符,本次任务要求不考虑&位运算符情况
				str+=ch;				//拼接 
				ch=fgetc(fp);			//继续读下一个字符
				if(ch=='|')				//如果能构成&&
				{
					str+=ch;			//完整拼接 
					t.linenum=linecount;	//更新行号
					t.times=0;				//分隔符不需要记录次数 
					t.tokenstring=str;		//更新拼接串
					t.tokentype=OR;		//英文半角分号的识别码
					tokenlist.push_back(t);	//加入到tokenlist中去
					Operator.push_back(vectorindex);//记录在tokenlist中的索引
					vectorindex++;			//tokenlist索引自增
					str="";					//重置拼接串 
				 } 
				else		//不能构成&&,则是错误 
				{
					DFAflag=0;	//出现词法错误 
					fseek(fp,-1,SEEK_CUR);	//回退多读的字符
					printf("ERROR:Undefined label |;Located on line No. %d",linecount);
					str="";		//重置拼接串 
				 } 
				break;
			}	//处理&&状态结束 
			
			case  '{':{		//处理左花括号状态 
				str+=ch;	//加入拼接 
				t.linenum=linecount;	//更新行号
				t.times=counttimes(str);//更新次数
				t.tokenstring=str;		//更新拼接串
				t.tokentype=LP;			//左花括号的识别码
				tokenlist.push_back(t);	//加入tokenlist中去
				Delimiter.push_back(vectorindex);//记录tokenlist中该分隔符的索引
				vectorindex++;			//tokenlist索引自增
				str="";				//重置拼接串 
				break;
			} 	//处理左花括号状态结束
			
			case '}':{		//处理右花括号状态 
				str+=ch;	//加入拼接
				t.linenum=linecount;	//更新行号
				t.times=counttimes(str);//更新次数
				tempstr='{';		//保存左花括号
				if(counttimes(tempstr)==1)	//如果之前没有出现左花括号
				{
					//多余的},输出错误信息 
					printf("ERROR:Character '}' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//重置拼接串 
				 } 
				else if(t.times>counttimes(tempstr))//如果前面所有的{都有匹配
				{
					//多余的},输出错误信息
					printf("ERROR:Character '}' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//重置拼接串 	 
				 } 
				else	//该}无错误，加入tokenlist中去
				{
					t.tokenstring=str;	//更新拼接串
					t.tokentype=RP;		//更新}的识别码 
					tokenlist.push_back(t);//加入tokenlist中去
					Delimiter.push_back(vectorindex);//记录分隔符在tokenlist中的识别码
					vectorindex++;		//tokenlist索引自增
					str="";				//重置拼接串 
				 } 
				break;
			} 	//处理右花括号状态结束 
			
			case  '(':{		//处理左圆括号状态 
				str+=ch;	//加入拼接 
				t.linenum=linecount;	//更新行号
				t.times=counttimes(str);//更新次数
				t.tokenstring=str;		//更新拼接串
				t.tokentype=BRACKETL;	//左圆括号的识别码
				tokenlist.push_back(t);	//加入tokenlist中去
				Delimiter.push_back(vectorindex);//记录tokenlist中该分隔符的索引
				vectorindex++;			//tokenlist索引自增
				str="";				//重置拼接串 
				break;
			} 	//处理左花括号状态结束
			
			case ')':{		//处理右圆括号状态 
				str+=ch;	//加入拼接
				t.linenum=linecount;	//更新行号
				t.times=counttimes(str);//更新次数
				tempstr='(';		//保存左圆括号
				if(counttimes(tempstr)==1)	//如果之前没有出现左花括号
				{
					//多余的),输出错误信息 
					printf("ERROR:Character ')' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//重置拼接串 
				 } 
				else if(t.times>counttimes(tempstr))//如果前面所有的{都有匹配
				{
					//多余的),输出错误信息
					printf("ERROR:Character ')' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//重置拼接串 	 
				 } 
				else	//该)无错误，加入tokenlist中去
				{
					t.tokenstring=str;	//更新拼接串
					t.tokentype=BRACKETR;	//更新)的识别码 
					tokenlist.push_back(t);//加入tokenlist中去
					Delimiter.push_back(vectorindex);//记录分隔符在tokenlist中的识别码
					vectorindex++;		//tokenlist索引自增
					str="";				//重置拼接串 
				 } 
				break;
			} 	//处理右圆括号状态结束 
			
			case  '[':{		//处理左圆括号状态 
				str+=ch;	//加入拼接 
				t.linenum=linecount;	//更新行号
				t.times=counttimes(str);//更新次数
				t.tokenstring=str;		//更新拼接串
				t.tokentype=PML;	//左方括号的识别码
				tokenlist.push_back(t);	//加入tokenlist中去
				Delimiter.push_back(vectorindex);//记录tokenlist中该分隔符的索引
				vectorindex++;			//tokenlist索引自增
				str="";				//重置拼接串 
				break;
			} 	//处理左花括号状态结束
			
			case ']':{		//处理右圆括号状态 
				str+=ch;	//加入拼接
				t.linenum=linecount;	//更新行号
				t.times=counttimes(str);//更新次数
				tempstr='[';		//保存左方括号
				if(counttimes(tempstr)==1)	//如果之前没有出现左方括号
				{
					//多余的],输出错误信息 
					printf("ERROR:Character ']' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//重置拼接串 
				 } 
				else if(t.times>counttimes(tempstr))//如果前面所有的{都有匹配
				{
					//多余的],输出错误信息
					printf("ERROR:Character ']' is a lack of matching;Located on line No.%d\n",linecount);
					str="";	//重置拼接串 	 
				 } 
				else	//该]无错误，加入tokenlist中去
				{
					t.tokenstring=str;	//更新拼接串
					t.tokentype=PMR;	//更新]的识别码 
					tokenlist.push_back(t);//加入tokenlist中去
					Delimiter.push_back(vectorindex);//记录分隔符在tokenlist中的识别码
					vectorindex++;		//tokenlist索引自增
					str="";				//重置拼接串 
				 } 
				break;
			} 	//处理右方括号状态结束 
			
			case '<':{	//处理小于号状态开始 
				str+=ch;	//加入拼接 
				t.linenum=linecount;	//更新行号
				t.times=0;				//操作符不需要记录出现次数
				t.tokenstring=str;		//更新完整串
				t.tokentype=LESS;		//小于号的标识码
				tokenlist.push_back(t);	//加入tokenlist中去
				Operator.push_back(vectorindex);//保存该操作符在tokenlist中的索引
				vectorindex++;			//tokenlist索引自增
				str="";					//重置拼接串 
				break;
			}		//处理小于号状态结束
			
			case '>':{	//处理大于号状态开始 
				str+=ch;	//加入拼接 
				t.linenum=linecount;	//更新行号
				t.times=0;				//操作符不需要记录出现次数
				t.tokenstring=str;		//更新完整串
				t.tokentype=GREATER;		//小于号的标识码
				tokenlist.push_back(t);	//加入tokenlist中去
				Operator.push_back(vectorindex);//保存该操作符在tokenlist中的索引
				vectorindex++;			//tokenlist索引自增
				str="";					//重置拼接串 
				break;
			}		//处理大于号状态结束 
			
			case  '\'':{	//处理单引号状态,英文里面单引号无左右之分，人为定义左右 
				str+=ch;		//先拼接成左单引号 
				t.linenum=linecount;//更新行号
				t.times=0;			//常量不需要记录出现的次数
				t.tokenstring=str;	//更新常量的完整串
				t.tokentype=SQUTOE_L;//左单引号的识别码
				tokenlist.push_back(t);//加入到tokenlist中去
				Delimiter.push_back(vectorindex);//保存字符常量在tokenlist中索引
				vectorindex++;		//tokenlist索引自增	 
				do{
					ch=fgetc(fp);	//读取下一个字符 
					str+=ch;	//加入拼接
					if(ch=='\'')//匹配后一个引号 
						break; //两单引号匹配成功就退出 
				}while(ch!='\n'&&ch!=';');//遇到换行或者分号就退出 
				if(ch=='\'')	//如果是因为匹配到引号退出
				{
					if(str.size()>3)	//如果不是单字符
					{
						//输出报错信息 
						printf("ERROR:%s is not the char-const;Located on line No.%d\n",str.c_str(),linecount);
						str="";		//重置拼接串 
					 }
					else 	//符合字符变量的格式
					{
						t.linenum=linecount;//更新行号
						t.times=0;			//常量不需要记录出现的次数
						t.tokenstring=str;	//更新常量的完整串
						t.tokentype=CHARCONST;//字符常量的识别码
						tokenlist.push_back(t);//加入到tokenlist中去
						Const.push_back(vectorindex);//保存字符常量在tokenlist中索引
						vectorindex++;		//tokenlist索引自增
						str="";				//重置拼接串 
						str='\'';			//单独设置右引号，认为定义有左右之分
						t.linenum=linecount;//更新行号
						t.times=0;			//常量不需要记录出现的次数
						t.tokenstring=str;	//更新常量的完整串
						t.tokentype=SQUTOE_R;//右单引号的识别码
						tokenlist.push_back(t);//加入到tokenlist中去
						Delimiter.push_back(vectorindex);//保存字符常量在tokenlist中索引
						vectorindex++;		//tokenlist索引自增	
						str="";			//重置拼接串	 
					 } 
				}
				else		//因为换行或者分号退出的
				{
					//输出不成对出现的匹配错误信息 
					printf("ERROR:Character ' is lack of matching;Located on line No.%d\n",linecount);
					fseek(fp,-1,SEEK_CUR);//文件指针回移 
					str="";			//重置拼接串 
				 } 
				break;
			} 	//处理单引号状态结束
			
			case  '\"':{	//处理双引号状态,英文里面单引号无左右之分，人为定义左右 
				str+=ch;		//先拼接成左单引号 
				t.linenum=linecount;//更新行号
				t.times=0;			//常量不需要记录出现的次数
				t.tokenstring=str;	//更新常量的完整串
				t.tokentype=DQUTOE_L;//左单引号的识别码
				tokenlist.push_back(t);//加入到tokenlist中去
				Delimiter.push_back(vectorindex);//保存字符常量在tokenlist中索引
				vectorindex++;		//tokenlist索引自增	 
				do{
					ch=fgetc(fp);	//读取下一个字符 
					str+=ch;	//加入拼接
					if(ch=='\"')//匹配后一个引号 
						break; //两单引号匹配成功就退出 
				}while(ch!='\n'&&ch!=';');//遇到换行或者分号就退出 
				if(ch=='\"')	//如果是因为匹配到引号退出
				{
					t.linenum=linecount;//更新行号
					t.times=0;			//常量不需要记录出现的次数
					t.tokenstring=str;	//更新常量的完整串
					t.tokentype=CHARCONST;//字符常量的识别码
					tokenlist.push_back(t);//加入到tokenlist中去
					Const.push_back(vectorindex);//保存字符常量在tokenlist中索引
					vectorindex++;		//tokenlist索引自增
					str="";				//重置拼接串 
					str='\"';			//单独设置右引号，认为定义有左右之分
					t.linenum=linecount;//更新行号
					t.times=0;			//常量不需要记录出现的次数
					t.tokenstring=str;	//更新常量的完整串
					t.tokentype=DQUTOE_R;//右单引号的识别码
					tokenlist.push_back(t);//加入到tokenlist中去
					Delimiter.push_back(vectorindex);//保存字符常量在tokenlist中索引
					vectorindex++;		//tokenlist索引自增	
					str="";			//重置拼接串	 	  
				}
				else		//因为换行或者分号退出的
				{
					//输出不成对出现的匹配错误信息 
					printf("ERROR:Character \" is lack of matching;Located on line No.%d\n",linecount);
					fseek(fp,-1,SEEK_CUR);//文件指针回移 
					str="";			//重置拼接串 
				 } 
				break;
			} 	//处理双引号状态结束
			
			case '=':{	//处理= == 状态
				str+=ch;	//拼接串
				if((ch=fgetc(fp))=='=')	//判断是不是==
				{
					str+=ch;	//拼接成==
					t.linenum=linecount;//更新行号 
					t.times=0;	//不需要记录操作符出现的次数
					t.tokenstring=str;//更新拼接串
					t.tokentype=EQ;		//==的识别码
					tokenlist.push_back(t);//加入到tokenlist中去
					Operator.push_back(vectorindex);//保存==在tokenlist中的索引
					vectorindex++;	//tokenlist索引自增
					str="";			//重置拼接串 
				 } 
				else 		//回退多读的字符
				{
					if(ch!=EOF)	//如果不是读到文件尾部 
						fseek(fp,-1,SEEK_CUR);//将文件指针回退一步 
					t.linenum=linecount;//更新行号
					t.times=0;	//操作符不需要统计出现次数
					t.tokenstring=str;	//更新拼接串
					t.tokentype=ASSIGN;	//更新=的识别码
					tokenlist.push_back(t);//加入tokenlist中去
					Operator.push_back(vectorindex);//保存操作符在tokenlist中的索引
					vectorindex++;		//tokenlist索引自增
					str="";				//重置拼接串 
				 } 
				
				break;
			} 
			
			case '!':{		//处理!=状态开始
				str+=ch;	//加入拼接
				ch=fgetc(fp);	//读入下一个字符
				if(ch=='=')		//如果下一个字符是=，就能构成!=
				{
					str+=ch;	//加入拼接
					t.linenum=linecount;	//更新行号
					t.times=0;		//操作符不需要统计出现次数
					t.tokenstring=str;	//加入完整拼接串
					t.tokentype=NOTEQ;	//!=的识别码
					tokenlist.push_back(t);	//加入tokenlist中去
					Operator.push_back(vectorindex);//保存在tokenlist中的索引
					vectorindex++;		//tokenlist索引自增
					str="";				//重置拼接串 
				 } 
				else		//不能构成!=
				{
					//！是非法字符 
					printf("ERROR:Illegal character '!';Located on line No.%d\n",linecount); 
					fseek(fp,-1,SEEK_CUR);	//回退多读的字符 
				 } 
				break;
			}
			
			
			default :{		//其他非法字符 
				printf("ERROR:Illegal character '%c';Located on line No.%d\n",ch,linecount); 
				DFAflag=0;		//词法分析出现错误 
				break;
			}
		
		} //end of switch	
			 
	}	//end of while(ch!=EOF)
	t.linenum=linecount+1;	//结束时换到下一行
	t.times=0;				//结束标记不需要记录
	t.tokenstring=ENDFILE;	//结束类型
	str='\0';				//结束字符
	t.tokenstring=str;		//加入结束字符 
	tokenlist.push_back(t);	//加入tokenlist中去作为程序终止 
	
	fclose(fp); 	//操作完后关闭文件 
	
}	//end of analysis 


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
	else if(str=="main")//main
		return MAIN; 
	else					// 
		return ID;
}


void Lexer::PrintWords()
{
	int i;	//循环变量
	printf("--------------------------------------------------");
	printf("\n标识符：\n"); 
	for(i=0;i<Id.size();i++)		//按顺序输出所有识别到的标识符 
	{ 
		cout<<std::left<<setw(width)<<tokenlist[Id[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词 
			cout<<endl;
	} 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("关键字：\n"); 
	for(i=0;i<Key.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Key[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("常量：\n"); 
	int k=0;
	for(i=0;i<Const.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Const[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("操作符：\n"); 
	for(i=0;i<Operator.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Operator[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n");
	printf("定界符(分隔符)：\n"); 
	for(i=0;i<Delimiter.size();i++)
	{
		cout<<std::left<<setw(width)<<tokenlist[Delimiter[i]].tokenstring;
		if((i+1)%3==0)				//每一行显示三个词
			cout<<endl; 
	 } 
	printf("\n--------------------------------------------------\n\n");
	printf("\n--------------------------------------------------\n");
	printf("注释：\n"); 
	for(i=0;i<Comment.size();i++)
	{
		cout<<tokenlist[Comment[i]].tokenstring<<endl;
	 } 
	printf("-----------------------------------------------------\n");
}


 
//void Lexer::Program()	//程序语法分析函数开始
//{
//	root=new syntaxnode; 
//	if(root==NULL)	//判断内存是否申请成功
//	{
//		//输出信息 
//		printf("内存申请失败！\n内存不够，自动关闭\n");
//		getchar();getchar();	//等待用户响应 
//		exit(0);
//	 } 
//	root->kind=rt;	//根节点特殊结点 
//	root->child=NULL;//初始化孩子
//	root->sibling=NULL;//初始化兄弟结点 
//	root->listindex=-1;	//初始化索引 
//
//	if(DeclarationList(root)!=ERROR)			//调用声明序列 
//	{
//		printf("\n\n语法树打印如上！");
//	}
//	else
//	{
//		printf("\n\n出现语法错误，生成语法树失败"); 
//	 } 
//		 
//	  
// } 
 
 
//int Lexer::DeclarationList(syntaxtree &root)	//声明序列处理函数 
//{
//	while(tokenlist[index].tokentype==COMMENT)	//过滤掉注释
//		index++;
//	if(tokenlist[index].tokentype>=1&&tokenlist[index].tokentype<=6) //如果是类型声明 
//	{
//		if(tokenlist[index+1].tokentype==ID)	//类型声明时候肯定是标识符
//		{
////			Declaration();				//调用声明处理函数 
//		}
//		else //类型声明之后不是标识符的情况就是错误
//		{
//			printf("ERROR:type-specifier needs Identifier;Locatied on line No.%d '%s'\n",tokenlist[index].linenum,tokenlist[index].tokenstring.c_str());
//			printf("语法树分析遇到错误，自动释放空间");
//			printf("请纠错后，重新生成语法树\n");
//			DeleteTree(root);		//释放树空间 
//			return ERROR; 			//返回ERROR值 
//		 } 
//	}//处理类型声明结束 
//	else if(tokenlist[index].tokentype==CONST)		//const型后才是类型声明
//	{
//		
//	 }
//	
// } 
// 

 
 
 
 
 
 
 
 
 
#endif
