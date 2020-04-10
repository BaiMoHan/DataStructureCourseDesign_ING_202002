#include"Lexical_analysis.h"


using namespace std;
int main()
{
	char ch; 
	while(1)
	{
		printf("----------------------------------------------------------\n");
		printf("--------------欢迎进入高级语言源程序格式处理工具----------\n");
		printf("使用规则：\n");
		printf("1.仅仅针对C语言子集源程序进行识别\n");
		printf("2.词法分析->语法分析->语法树输出->格式化源程序 自动执行\n");
		printf("3.上述过程中间遇到错误就会自动停止并报错\n");
		printf("4.建议程序中不要出现ASCII码范围之外的字符！\n");	
		Lexer *L=new Lexer; 
		delete L;
		printf("\n该高级语言源程序分析完毕\n");
		printf("退出请输入q或者Q（其他字符则不退出）："); 
		getchar();
		ch=getchar();
		if(ch=='q'||ch=='Q')
			break;
		system("cls");
	}
	printf("\n退出系统成功，欢迎下次使用！");
 } 
