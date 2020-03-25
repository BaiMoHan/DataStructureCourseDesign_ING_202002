#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//����vectorʵ��tokenlist 
#include<string>	//����String�� 
#define MAXINDEX 100	//����������� 
#define status int		//���庯������ֵ 
#define ERROR -1		//�������ֵ 
#define OK 1			//������ȷֵ 


using namespace std;	//ʹ�ñ�׼�����ռ� 

typedef enum{		//����ʷ���Ҫ��ʶ����
	 INT=1,CHAR,FLOAT,DOUBLE,LONG,VOID,INTCONST,CHARCONST,LONGCONST,FLOATCONST,DOUBLECONST,STRINGCONST,
	 CONST,INCLUDE,IF,ELSE,WHILE,FOR,BREAK,RETURN,CONTINUE,MAIN,SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,COMMENT,ADDEQ,
	 ADDSELF,PML,REDUCEEQ,REDUCESELF,PMR,MUTIPLYEQ,MODEQ,DIVIDEEQ,MOD,DIVIDE,EQ,NOTEQ,LESS,GREATER,AND,
	 OR,ASSIGN,ADD,BRACKETL,BRACKETR,REDUCE,MUTIPLY,LP,RP,COMMA,SEMI,ENDFILE,ID
	
}TokenType; 

typedef struct token{
	TokenType tokentype;	//�ʵ�ʶ����
	string tokenstring;		//�ʱ����ֵ
	int times;				//���ֵĴ��� 
	int linenum;			//���ڵ��к� 
}token;

typedef enum{	//�����﷨���Ľ�����
	//���ɽڵ�,���ڵ�,�ļ������ڵ�,�����ļ����ڵ�,�ⲿ��������,��������ֵ����,������,�պ����β�����,�����β����У�
	//��������,��ʶ��������,����,�ⲿ��������,��������,�������,�������,�ֲ���������,�������,break��� 
	rt=1,inclu,inclufile,exfuncdecla,functype,funcname,nonfuncparam,funcparam,type,id,array,exvardecla,funcdef,
	compstmd,statelist,locvardecla,returnnode,breaknode
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
		status Program();		//�����﷨����
		status DeclarationList();	//���������﷨���� 
		status VarDeclaration(syntaxtree &T);	//�ⲿ�������������� 
		status FunctionDeclaration();	//�������������� 
		status FunctionDefine();		//�������崦���� 
		status ParameterList(syntaxtree& T);//�β����д����� 
		syntaxtree Parameter();	//�βδ����� 
		syntaxtree TypeSpecifier();			//��������������
		syntaxtree Identifier(int state);			//��ʶ�������� 
		 
		
		
		syntaxtree CompoundStmd();		//������䴦����
		syntaxtree StatementList();		//������д�����
		syntaxtree Statement();			//��䴦���� 
		syntaxtree Expression();		//���ʽ������
		syntaxtree LocalVar();			//�ֲ����������� 
		
		
		syntaxtree ReturnState();	//����return��� 
		syntaxtree BreakState();	//����break��� 
		 
		status ExVarDeclaration();			//�ⲿ�������������� 
		status FunctionDefine(syntaxtree &T);	//�������庯��
		status InsertSibling(syntaxtree& p,syntaxtree& q);//����p���ֵܽڵ�q 
		status InsertChild(syntaxtree&p,syntaxtree &q);	//����p�ĺ��ӽڵ�q 
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
		int errorflag;			//�﷨�������־ 
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
