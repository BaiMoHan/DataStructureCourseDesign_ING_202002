#include"Lexical_analysis.h"


using namespace std;
int main()
{
	char ch; 
	while(1)
	{
		printf("----------------------------------------------------------\n");
		printf("--------------��ӭ����߼�����Դ�����ʽ������----------\n");
		printf("ʹ�ù���\n");
		printf("1.�������C�����Ӽ�Դ�������ʶ��\n");
		printf("2.�ʷ�����->�﷨����->�﷨�����->��ʽ��Դ���� �Զ�ִ��\n");
		printf("3.���������м���������ͻ��Զ�ֹͣ������\n");
		printf("4.��������в�Ҫ����ASCII�뷶Χ֮����ַ���\n");	
		Lexer *L=new Lexer; 
		delete L;
		printf("\n�ø߼�����Դ����������\n");
		printf("�˳�������q����Q�������ַ����˳�����"); 
		getchar();
		ch=getchar();
		if(ch=='q'||ch=='Q')
			break;
		system("cls");
	}
	printf("\n�˳�ϵͳ�ɹ�����ӭ�´�ʹ�ã�");
 } 
