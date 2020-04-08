#ifndef PrintFile_CPP
#define PrintFile_CPP
#include "Lexical_analysis.h"
#include<stack>		//����ջ�ļ� 
#include<cstring>	//����C��string�ļ� 
using namespace std;
#include<iostream>
/********************************************* 
�������ܣ������������CԴ�����ļ�
**********************************************/ 
status Lexer::PrintCFile()
{
	int len=strlen(filename);
	filename[len]='p';//�޸��ļ���
	filename[len+1]='\0';//���ļ���ĩβ
	int step=1;		//����	
	FILE* fp;//�����ļ�ָ��
	fp=fopen(filename,"w");//��ֻд�ķ�ʽ��Դ�����ļ� 	
	if(fp==NULL)		//�ж��ļ��Ƿ��ʧ�� 
	{
		printf("�ļ���ʧ��,�����´򿪳���");//�ļ���ʧ����ʾ��Ϣ 
		getchar();getchar();				//�ȴ��û������ַ�,������Ϣ��ʾʱ�� 
		exit(-1) ; 				//�رճ��� 
	} 
	while(tokenlist[current].tokentype!=ENDFILE)	//һֱ�����ĩβ
	{ 
		switch(tokenlist[current].tokentype){
			
			//׷�������ո� 
			case INT:
			case CHAR:
			case FLOAT:
			case DOUBLE:
			case LONG:
			case VOID:{
				fprintf(fp,"%s  ",tokenlist[current].tokenstring.c_str()); 
				current++;	//ȡ����������
				break;
			}
			
			case INCLUDE:{	//ǰ��#
				fprintf(fp,"#%s",tokenlist[current].tokenstring.c_str()); 
				current++;	//ȡ����������
				break;
			}
			
			//��黻��
			case STRINGCONST: 
			case SEMI: 
			case COMMENT:{	
				fprintf(fp,"%s",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum)
					fprintf(fp,"\n");
				current++;	//ȡ���������� 
				break;
			}
			
			case BRACKETR:{	//��黻�в�׷�ӿո�
			 	fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum)
					fprintf(fp,"\n");
				current++;	//ȡ���������� 
				break;
			}
			
			case LP:{
//				fprintf(fp,"%s\n",tokenlist[current].tokenstring.c_str()); 
//				current++;
				PrintBlock(fp,step);	//�������� 
//				fprintf(fp,"\n%s\n",tokenlist[current].tokenstring.c_str()); 
				break;
			} 
			
			//Ĭ��׷��һ���ո� 
			default:{
				fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				current++;	//ȡ���������� 
				break;
			}

		}//end of switch 
	 } //end of while
	 fprintf(fp,"\n\n\\\\�����ʽ���ɹ���\n");
	 fclose(fp);		//�ر��ļ�
	 return OK; 
}

status Lexer::PrintBlock(FILE* fp,int step)	//���鴦��
{

	//���ǰ�ÿո� 
	for(int i=0;i<step-1;i++)
		fprintf(fp,"    ");
	fprintf(fp,"%s   ",tokenlist[current].tokenstring.c_str()); 
	current++;
	if(tokenlist[current].tokentype==COMMENT&&tokenlist[current-1].linenum==tokenlist[current].linenum)	//���{����ע�� 
	{
		fprintf(fp,"%s",tokenlist[current].tokenstring.c_str());
		current++;
	}
	fprintf(fp,"\n");
	if(tokenlist[current].tokentype!=LP)
		for(int i=0;i<step;i++)
			fprintf(fp,"    ");
	while(tokenlist[current].tokentype!=RP&&tokenlist[current].tokentype!=ENDFILE)	//��������}���˳� 
	{ 
		switch(tokenlist[current].tokentype){
			
			//׷�������ո� 
			case INT:
			case CHAR:
			case FLOAT:
			case DOUBLE:
			case LONG:
			case VOID:{
				fprintf(fp,"%s  ",tokenlist[current].tokenstring.c_str()); 
				current++;	//ȡ����������
				break;
			}
			
			case INCLUDE:{	//ǰ��#
				fprintf(fp,"#%s",tokenlist[current].tokenstring.c_str()); 
				current++;	//ȡ����������
				break;
			}
			
			//��黻�� 
			case SEMI: 
			case COMMENT:{	
				fprintf(fp,"%s",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum&&tokenlist[current+1].tokentype!=RP)
				{ 
					fprintf(fp,"\n"); 
					for(int i=0;i<step;i++)	//���ǰ�ÿո� 
						fprintf(fp,"    ");		
				} 
				current++;	//ȡ���������� 
				break;
			}
			
			case BRACKETR:{	//��黻�в�׷�ӿո�
			 	fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				if(tokenlist[current].linenum<tokenlist[current+1].linenum&&tokenlist[current+1].tokentype!=RP)
				{ 
					fprintf(fp,"\n");
					for(int i=0;i<step;i++)	//���ǰ�ÿո� 
						fprintf(fp,"    ");
				} 
				current++;	//ȡ���������� 
				break;
			}
			
			case LP:{
//				fprintf(fp,"%s\n",tokenlist[current].tokenstring.c_str()); 
//				current++;
				PrintBlock(fp,step+1);	//�������� 
//				for(int i=0;i<step;i++)	//���ǰ�ÿո� 
//						fprintf(fp,"    ");
//				fprintf(fp,"%s\n",tokenlist[current].tokenstring.c_str()); 
//				current++;
				break;
			} 
			
			//Ĭ��׷��һ���ո� 
			default:{
				fprintf(fp,"%s ",tokenlist[current].tokenstring.c_str()); 
				current++;	//ȡ���������� 
				break;
			}

		}//end of switch 
	 } //end of while
	//�������}��ʽ 
	fprintf(fp,"\n");
	for(int i=0;i<step-1;i++)
		fprintf(fp,"    ");
	fprintf(fp,"%s  ",tokenlist[current].tokenstring.c_str()); 
	current++;
	if(tokenlist[current].tokentype==COMMENT&&tokenlist[current-1].linenum==tokenlist[current].linenum)	//���}����ע�� 
	{
		fprintf(fp,"%s",tokenlist[current].tokenstring.c_str());
		current++;
	}
	fprintf(fp,"\n"); 
	 return OK;
 } 
/*************************************************************************
�������ܣ����ʶ��Ĵʷ����ļ���
************************************************************************/
status Lexer::PrintWordsFile()
{
	int len=strlen(filename);
	filename[len]='w';//�޸��ļ���
	filename[len+1]='\0';//���ļ���ĩβ
	int index=0;		//����	
	FILE* fp;//�����ļ�ָ��
	fp=fopen(filename,"w");//��ֻд�ķ�ʽ��Դ�����ļ� 	
	if(fp==NULL)		//�ж��ļ��Ƿ��ʧ�� 
	{
		printf("�ļ���ʧ��,�����´򿪳���");//�ļ���ʧ����ʾ��Ϣ 
		getchar();getchar();				//�ȴ��û������ַ�,������Ϣ��ʾʱ�� 
		exit(-1) ; 				//�رճ��� 
	} 
	while(tokenlist[index].tokentype!=ENDFILE)//һֱ������ļ�ĩβ
	{
		switch(tokenlist[index].tokentype){
			
			case RETURN:
			case MAIN:
			case CONTINUE:
			case BREAK:
			case FOR:
			case WHILE:
			case ELSE:
			case IF:
			case INCLUDE:
			case CONST:
			case CHAR:
			case FLOAT:
			case DOUBLE:
			case LONG:
			case VOID:
			case INT:{
				fprintf(fp,"�ؼ���:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case INTCONST:
			case CHARCONST:
			case LONGCONST:
			case FLOATCONST:
			case DOUBLECONST:{
				fprintf(fp,"���ݳ���:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case SQUTOE_L:
			case SQUTOE_R:{
				fprintf(fp,"������:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			} 
			
			case DQUTOE_L:
			case DQUTOE_R:{
				fprintf(fp,"˫����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			} 
			
			case COMMENT:{
				fprintf(fp,"ע��:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			} 
			
			case STRINGCONST:{
				fprintf(fp,"�����ļ���:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			} 
			
			case ADDEQ:{
				fprintf(fp,"��������:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case ADDSELF:{
				fprintf(fp,"����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			} 
			
			case PML:{
				fprintf(fp,"������:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case PMR:{
				fprintf(fp,"�ҷ�����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case MUTIPLYEQ:{
				fprintf(fp,"�˵���:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			} 
			
			case MODEQ:{
				fprintf(fp,"ȡģ����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case DIVIDEEQ:{
				fprintf(fp,"���Ե���:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case MOD:{
				fprintf(fp,"ȡģ:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case DIVIDE:{
				fprintf(fp,"����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case EQ:{
				fprintf(fp,"�Ƿ������:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case NOTEQ:{
				fprintf(fp,"������:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case LESS:{
				fprintf(fp,"С�ں�:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case GREATER:{
				fprintf(fp,"���ں�:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case AND:{
				fprintf(fp,"�߼���:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}	
			
			case OR:{
				fprintf(fp,"�߼���:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case ASSIGN:{
				fprintf(fp,"��ֵ����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case ADD:{
				fprintf(fp,"�Ӻ�:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case REDUCE:{
				fprintf(fp,"����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}						
			
			case MUTIPLY:{
				fprintf(fp,"�˺�:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case BRACKETL:{
				fprintf(fp,"��Բ����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case BRACKETR:{
				fprintf(fp,"��Բ����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case LP:{
				fprintf(fp,"������:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case RP:{
				fprintf(fp,"�һ�����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case COMMA:{
				fprintf(fp,"����:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case SEMI:{
				fprintf(fp,"�ֺ�:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			case ID:{
				fprintf(fp,"��ʶ��:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}
			
			default:{
				fprintf(fp,"δʶ��:%s ��������%d\n",tokenlist[index].tokenstring.c_str(),tokenlist[index].linenum);
				index++;
				break;
			}				
		}
	 } 
 } 
#endif
