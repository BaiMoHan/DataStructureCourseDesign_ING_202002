#ifndef PrintFile_CPP
#define PrintFile_CPP
#include "Lexical_analysis.h"
#include<stack>		//引用栈文件 
#include<cstring>	//引用C的string文件 
using namespace std;
#include<iostream>
/********************************************* 
函数功能：输出带缩进的C源程序文件
**********************************************/ 
status Lexer::PrintCFile()
{
	int len=strlen(filename);
	filename[len]='p';//修改文件名
	filename[len+1]='\0';//新文件名末尾
	int step=1;		//步长	
	FILE* fp;//定义文件指针
	fp=fopen(filename,"w");//以只写的方式打开源程序文件 	
	if(fp==NULL)		//判断文件是否打开失败 
	{
		printf("文件打开失败,请重新打开程序");//文件打开失败提示信息 
		getchar();getchar();				//等待用户输入字符,留出信息显示时间 
		exit(-1) ; 				//关闭程序 
	} 
	while(tokenlist[current].tokentype!=ENDFILE)	//一直输出到末尾
	{ 
		switch(tokenlist[current].tokentype){
			
			//追加两个空格 
			case INT:
			case CHAR:
			case FLOAT:
			case DOUBLE:
			case LONG:
			case VOID:{
				fprintf(fp,"%s  ",tokenlist[current].tokenstring.c_str()); 
				current++;	//取词索引自增
				break;
			}
			
			case INCLUDE:{	//前加#
				fprintf(fp,"#%s",tokenlist[current].tokenstring.c_str()); 
				current++;	//取词索引自增
				break;
			}
			
			//检查换行
			case STRINGCONST: 
			case SEMI: 
			case COMMENT:{	
				fprintf(fp,"%s",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum)
					fprintf(fp,"\n");
				current++;	//取词索引自增 
				break;
			}
			
			case BRACKETR:{	//检查换行并追加空格
			 	fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum)
					fprintf(fp,"\n");
				current++;	//取词索引自增 
				break;
			}
			
			case LP:{
//				fprintf(fp,"%s\n",tokenlist[current].tokenstring.c_str()); 
//				current++;
				PrintBlock(fp,step);	//进入语句块 
//				fprintf(fp,"\n%s\n",tokenlist[current].tokenstring.c_str()); 
				break;
			} 
			
			//默认追加一个空格 
			default:{
				fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				current++;	//取词索引自增 
				break;
			}

		}//end of switch 
	 } //end of while
	 fprintf(fp,"\n\n\\\\程序格式化成功！\n");
	 fclose(fp);		//关闭文件
	 return OK; 
}

status Lexer::PrintBlock(FILE* fp,int step)	//语句块处理
{

	//输出前置空格 
	for(int i=0;i<step-1;i++)
		fprintf(fp,"    ");
	fprintf(fp,"%s   ",tokenlist[current].tokenstring.c_str()); 
	current++;
	if(tokenlist[current].tokentype==COMMENT&&tokenlist[current-1].linenum==tokenlist[current].linenum)	//如果{后有注释 
	{
		fprintf(fp,"%s",tokenlist[current].tokenstring.c_str());
		current++;
	}
	fprintf(fp,"\n");
	if(tokenlist[current].tokentype!=LP)
		for(int i=0;i<step;i++)
			fprintf(fp,"    ");
	while(tokenlist[current].tokentype!=RP&&tokenlist[current].tokentype!=ENDFILE)	//遇到语句块}就退出 
	{ 
		switch(tokenlist[current].tokentype){
			
			//追加两个空格 
			case INT:
			case CHAR:
			case FLOAT:
			case DOUBLE:
			case LONG:
			case VOID:{
				fprintf(fp,"%s  ",tokenlist[current].tokenstring.c_str()); 
				current++;	//取词索引自增
				break;
			}
			
			case INCLUDE:{	//前加#
				fprintf(fp,"#%s",tokenlist[current].tokenstring.c_str()); 
				current++;	//取词索引自增
				break;
			}
			
			//检查换行 
			case SEMI: 
			case COMMENT:{	
				fprintf(fp,"%s",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum&&tokenlist[current+1].tokentype!=RP)
				{ 
					fprintf(fp,"\n"); 
					for(int i=0;i<step;i++)	//输出前置空格 
						fprintf(fp,"    ");		
				} 
				current++;	//取词索引自增 
				break;
			}
			
			case BRACKETR:{	//检查换行并追加空格
			 	fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum&&tokenlist[current+1].tokentype!=RP)
				{ 
					fprintf(fp,"\n");
					for(int i=0;i<step;i++)	//输出前置空格 
						fprintf(fp,"    ");
				} 
				current++;	//取词索引自增 
				break;
			}
			
			case LP:{
//				fprintf(fp,"%s\n",tokenlist[current].tokenstring.c_str()); 
//				current++;
				PrintBlock(fp,step+1);	//进入语句块 
//				for(int i=0;i<step;i++)	//输出前置空格 
//						fprintf(fp,"    ");
//				fprintf(fp,"%s\n",tokenlist[current].tokenstring.c_str()); 
//				current++;
				break;
			} 
			
			//默认追加一个空格 
			default:{
				fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				current++;	//取词索引自增 
				break;
			}

		}//end of switch 
	 } //end of while
	//输出后置}格式 
	fprintf(fp,"\n");
	for(int i=0;i<step-1;i++)
		fprintf(fp,"    ");
	fprintf(fp,"%s  ",tokenlist[current].tokenstring.c_str()); 
	current++;
	if(tokenlist[current].tokentype==COMMENT&&tokenlist[current-1].linenum==tokenlist[current].linenum)	//如果}后有注释 
	{
		fprintf(fp,"%s",tokenlist[current].tokenstring.c_str());
		current++;
	}
	fprintf(fp,"\n"); 
	 return OK;
 } 
#endif
