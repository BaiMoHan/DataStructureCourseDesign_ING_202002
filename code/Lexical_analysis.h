#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//����vectorʵ��tokenlist 
#include<string>	//����String�� 
#define MAXINDEX 100	//����������� 
using namespace std;
typedef enum{		//����ʷ���Ҫ��ʶ���� 
	ERROR_TOKEN=1,LP,RP,PML,PMR,SEMI,COMMA,INT_CONST,FLOAT_CONST,DOUBLE_CONST,
	LONG_CONST,CHAR_CONST,VOID,RETURN,IF,ELSE,WHILE,FOR,BREAK,CONTINUE,ADD,REDUCE,
	MUTIPLEY,DIVIDE,ADDSELF,DIVIDESELF,ASSIGN,EQ,ADD_EQ,DIVIDE_EQ,NUM,STRING_CONST,
	SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,INT,DOUBLE,CHAR,LONG,LESS,GRETER,MOD,NOTEQ,
	BRACKETL,BRACKETR,AND,OR,INCLUDE,OTHERS,ENDFILE,COMMENT,ID,FLOAT 
}TokenType; 	//������൥�ʵ������ 
typedef struct token{
	TokenType tokentype;	//�ʵ�ʶ����
	string tokenstring;		//�ʱ����ֵ
	int times;				//���ֵĴ��� 
	int linenum;			//���ڵ��к� 
}token;

class Lexer	//�ʷ������� 
{
	public:
		Lexer();		//���캯�� 
		void analysis(char filename[]);
		void Disp();
		int IDIndex;//��ʶ����������
		int KeyIndex;//�ؼ�����������
		int ConstIndex;//������������
		int OperatorIndex;//��������������
		int CommentIndex;//ע���������� 
		void PrintWords();//�ʷ������ɹ������ʶ������Ĵ� 
	private:
		int counttimes(string str);	//ͳ�Ʊ�ʶ����tokenlist���ֵĴ��� 
		TokenType gettokentype(string str);//��ȡ�ַ����ı�ʶ�� 
		int state;				//�ʷ�����״̬ 
		int DFAflag;			//DFA�����Զ������������־ 
		bool analysis_flag;		//�ʷ������Ƿ�ɹ��ı�־���� 
		char filename[30];		//�����ļ��� 
		int  linecount;			//�кż�����
		vector<token> tokenlist;//����Զ���ʶ������Ĵ�
		int vectorindex;		//tokenlist��ȡ������
		vector<int>  Id;//����tokenlist�еĴʷ������������ڶ�Ӧ������ 
};

#endif
