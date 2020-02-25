#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//����vectorʵ��tokenlist 
#include<string>	//����String�� 
#define MAXINDEX 100	//����������� 
//class Parser;

using namespace std;
typedef enum{		//����ʷ���Ҫ��ʶ���� 
	ERROR_TOKEN=1,LP,RP,PML,PMR,SEMI,COMMA,INT_CONST,FLOAT_CONST,DOUBLE_CONST,
	LONG_CONST,CHAR_CONST,VOID,RETURN,IF,ELSE,WHILE,FOR,BREAK,CONTINUE,ADD,REDUCE,
	MUTIPLY,DIVIDE,ADDSELF,REDUCESELF,ASSIGN,EQ,ADD_EQ,REDUCE_EQ,MUTIPLY_EQ,DIVIDE_EQ,NUM,STRING_CONST,
	SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,INT,DOUBLE,CHAR,LONG,LESS,GREATER,MOD,NOTEQ,
	BRACKETL,BRACKETR,AND,OR,INCLUDE,OTHERS,ENDFILE,COMMENT,ID,FLOAT,CONST,MAIN
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
		friend class Parser;	//��Ԫ�� 
		void analysis(char filename[]);
		void Disp();
		void PrintWords();//�ʷ������ɹ������ʶ������Ĵ� 
	private:
		int counttimes(string str);	//ͳ�Ʊ�ʶ����tokenlist���ֵĴ��� 
		TokenType gettokentype(string str);//��ȡ�ַ����ı�ʶ�� 
		int state;				//�ʷ�����״̬ 
		int DFAflag;			//DFA�����Զ������������־ 
		char filename[30];		//�����ļ��� 
		int  linecount;			//�кż�����
		vector<token> tokenlist;//����Զ���ʶ������Ĵ�
		int vectorindex;		//tokenlist��ȡ������
		//����tokenlist�еĴʷ������������ڶ�Ӧ������
		vector<int> Id;			//�ű�ʶ�������� 
		vector<int> Key; 		//�Źؼ��ֵ�����
		vector<int> Const;		//�ų���������
		vector<int> Operator;	//�Ų����������� 
		vector<int> Comment;	//��ע�͵����� 
		vector<int> Delimiter;	//�Ŷ���������� 

};

#endif
