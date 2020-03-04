#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//����vectorʵ��tokenlist 
#include<string>	//����String�� 
#define MAXINDEX 100	//����������� 
#define status int		//���庯������ֵ 
#define ERROR -1


using namespace std;
//typedef enum{		//����ʷ���Ҫ��ʶ���� 
//	ERROR_TOKEN=1,LP,RP,PML,PMR,SEMI,COMMA,INT_CONST,FLOAT_CONST,DOUBLE_CONST,
//	LONG_CONST,CHAR_CONST,VOID,RETURN,IF,ELSE,WHILE,FOR,BREAK,CONTINUE,ADD,REDUCE,
//	MUTIPLY,DIVIDE,ADDSELF,REDUCESELF,ASSIGN,EQ,ADD_EQ,REDUCE_EQ,MUTIPLY_EQ,DIVIDE_EQ,NUM,STRING_CONST,
//	SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,INT,DOUBLE,CHAR,LONG,LESS,GREATER,MOD,NOTEQ,
//	BRACKETL,BRACKETR,AND,OR,INCLUDE,OTHERS,ENDFILE,COMMENT,ID,FLOAT,CONST,MAIN
//}TokenType; 	//������൥�ʵ������ 

typedef enum{		//����ʷ���Ҫ��ʶ����
	 INT=1,CHAR,FLOAT,DOUBLE,LONG,VOID,INTCONST,CHARCONST,LONGCONST,FLOATCONST,DOUBLECONST,STRINGCONST,
	 CONST,INCLUDE,IF,ELSE,WHILE,FOR,BREAK,RETURN,CONTINUE,MAIN,SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,ADD,ADDEQ,
	 ADDSELF,REDUCE,REDUCEEQ,REDUCESELF,MUTIPLY,MUTIPLYEQ,DIVIDE,DIVIDEEQ,MOD,MODEQ,EQ,NOTEQ,LESS,GREATER,AND,
	 OR,ASSIGN,COMMENT,BRACKETL,BRACKETR,PML,PMR,LP,RP,COMMA,SEMI,ENDFILE,ID
	
}TokenType; 

typedef struct token{
	TokenType tokentype;	//�ʵ�ʶ����
	string tokenstring;		//�ʱ����ֵ
	int times;				//���ֵĴ��� 
	int linenum;			//���ڵ��к� 
}token;

typedef enum{	//�����﷨���Ľ�����
	//���ڵ�,�ļ������ڵ�,�����ļ����ڵ�,�ⲿ��������,�ⲿ��������,��������, �������� 
	rt=1,inclu,inclufile,exdeflist,exvardef,exfuncdef,datatype,
}nodekind;

	
typedef struct syntaxnode{	//�����ֵܱ�ʾ�� 
	syntaxnode *child;	//Ĭ���������Ǻ��� 
	syntaxnode *sibling;//���������ֵ� 
	nodekind kind;		//����������
	int listindex;			//��tokenlist�е����� 
}* syntaxtree,syntaxnode; 

class Lexer	//�ʷ������� 
{
	public:
		token tokentext;	//��ʱȡ���м����
		int index; 			//��tokenlist�е�ȡ������ 
		Lexer();		//���캯�� 
		void analysis(char filename[]);//�ʷ����� 
		status Program(syntaxtree &root);		//�����﷨����
		status DeclarationList(syntaxtree& T);	//���������﷨���� 
//		status Declaration();		//�����﷨����
//		int VarDeclaration();	//��������
//		int FunctionDeclaration();	//�������� 
		void PrintNode(syntaxtree p,int step);	//��ӡ���ڵ�p 
		void PrintTree(syntaxtree& root);	//ǰ�������ӡ�� 
		void DeleteTree(syntaxtree& root);	//�ͷ����ռ� 
		void PrintWords();//�ʷ������ɹ������ʶ������Ĵ� 
	private:
		syntaxtree root=NULL;	//�﷨�����ڵ� 
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
