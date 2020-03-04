#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//借助vector实现tokenlist 
#include<string>	//借助String类 
#define MAXINDEX 100	//数组最大容量 
#define status int		//定义函数返回值 
#define ERROR -1


using namespace std;
//typedef enum{		//定义词法需要的识别码 
//	ERROR_TOKEN=1,LP,RP,PML,PMR,SEMI,COMMA,INT_CONST,FLOAT_CONST,DOUBLE_CONST,
//	LONG_CONST,CHAR_CONST,VOID,RETURN,IF,ELSE,WHILE,FOR,BREAK,CONTINUE,ADD,REDUCE,
//	MUTIPLY,DIVIDE,ADDSELF,REDUCESELF,ASSIGN,EQ,ADD_EQ,REDUCE_EQ,MUTIPLY_EQ,DIVIDE_EQ,NUM,STRING_CONST,
//	SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,INT,DOUBLE,CHAR,LONG,LESS,GREATER,MOD,NOTEQ,
//	BRACKETL,BRACKETR,AND,OR,INCLUDE,OTHERS,ENDFILE,COMMENT,ID,FLOAT,CONST,MAIN
//}TokenType; 	//定义各类单词的类别码 

typedef enum{		//定义词法需要的识别码
	 INT=1,CHAR,FLOAT,DOUBLE,LONG,VOID,INTCONST,CHARCONST,LONGCONST,FLOATCONST,DOUBLECONST,STRINGCONST,
	 CONST,INCLUDE,IF,ELSE,WHILE,FOR,BREAK,RETURN,CONTINUE,MAIN,SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,ADD,ADDEQ,
	 ADDSELF,REDUCE,REDUCEEQ,REDUCESELF,MUTIPLY,MUTIPLYEQ,DIVIDE,DIVIDEEQ,MOD,MODEQ,EQ,NOTEQ,LESS,GREATER,AND,
	 OR,ASSIGN,COMMENT,BRACKETL,BRACKETR,PML,PMR,LP,RP,COMMA,SEMI,ENDFILE,ID
	
}TokenType; 

typedef struct token{
	TokenType tokentype;	//词的识别码
	string tokenstring;		//词本身的值
	int times;				//出现的次数 
	int linenum;			//所在的行号 
}token;

typedef enum{	//定义语法树的结点类别
	//根节点,文件包含节点,引用文件名节点,外部定义序列,外部变量声明,函数定义, 数据类型 
	rt=1,inclu,inclufile,exdeflist,exvardef,exfuncdef,datatype,
}nodekind;

	
typedef struct syntaxnode{	//孩子兄弟表示法 
	syntaxnode *child;	//默认左子树是孩子 
	syntaxnode *sibling;//右子树是兄弟 
	nodekind kind;		//树结点的类型
	int listindex;			//在tokenlist中的索引 
}* syntaxtree,syntaxnode; 

class Lexer	//词法分析类 
{
	public:
		token tokentext;	//临时取词中间变量
		int index; 			//在tokenlist中的取词索引 
		Lexer();		//构造函数 
		void analysis(char filename[]);//词法分析 
		status Program(syntaxtree &root);		//程序语法分析
		status DeclarationList(syntaxtree& T);	//声明序列语法分析 
//		status Declaration();		//声明语法分析
//		int VarDeclaration();	//变量声明
//		int FunctionDeclaration();	//函数声明 
		void PrintNode(syntaxtree p,int step);	//打印树节点p 
		void PrintTree(syntaxtree& root);	//前序遍历打印树 
		void DeleteTree(syntaxtree& root);	//释放树空间 
		void PrintWords();//词法分析成功后输出识别出来的词 
	private:
		syntaxtree root=NULL;	//语法树根节点 
		int counttimes(string str);	//统计标识符在tokenlist出现的次数 
		TokenType gettokentype(string str);//获取字符串的标识符 
		int state;				//词法分析状态 
		int DFAflag;			//DFA有限自动机分析情况标志 
		char filename[30];		//测试文件名 
		int  linecount;			//行号计数器
		vector<token> tokenlist;//存放自动机识别出来的词
		int vectorindex;		//tokenlist的取词索引
		//对于tokenlist中的词分类后将索引存放在对应数组中
		vector<int> Id;			//放标识符的索引 
		vector<int> Key; 		//放关键字的索引
		vector<int> Const;		//放常量的索引
		vector<int> Operator;	//放操作符的索引 
		vector<int> Comment;	//放注释的索引 
		vector<int> Delimiter;	//放定界符的索引 

};

#endif
