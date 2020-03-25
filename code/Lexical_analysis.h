#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//借助vector实现tokenlist 
#include<string>	//借助String类 
#define MAXINDEX 100	//数组最大容量 
#define status int		//定义函数返回值 
#define ERROR -1		//定义错误值 
#define OK 1			//定义正确值 


using namespace std;	//使用标准命名空间 

typedef enum{		//定义词法需要的识别码
	 INT=1,CHAR,FLOAT,DOUBLE,LONG,VOID,INTCONST,CHARCONST,LONGCONST,FLOATCONST,DOUBLECONST,STRINGCONST,
	 CONST,INCLUDE,IF,ELSE,WHILE,FOR,BREAK,RETURN,CONTINUE,MAIN,SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,COMMENT,ADDEQ,
	 ADDSELF,PML,REDUCEEQ,REDUCESELF,PMR,MUTIPLYEQ,MODEQ,DIVIDEEQ,MOD,DIVIDE,EQ,NOTEQ,LESS,GREATER,AND,
	 OR,ASSIGN,ADD,BRACKETL,BRACKETR,REDUCE,MUTIPLY,LP,RP,COMMA,SEMI,ENDFILE,ID
	
}TokenType; 

typedef struct token{
	TokenType tokentype;	//词的识别码
	string tokenstring;		//词本身的值
	int times;				//出现的次数 
	int linenum;			//所在的行号 
}token;

typedef enum{	//定义语法树的结点类别
	//过渡节点,根节点,文件包含节点,引用文件名节点,外部函数声明,函数返回值类型,函数名,空函数形参序列,函数形参序列，
	//数据类型,标识符变量名,数组,外部变量声明,函数定义,复合语句,语句序列,局部变量声明,返回语句,break语句 
	rt=1,inclu,inclufile,exfuncdecla,functype,funcname,nonfuncparam,funcparam,type,id,array,exvardecla,funcdef,
	compstmd,statelist,locvardecla,returnnode,breaknode
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
		status Program();		//程序语法分析
		status DeclarationList();	//声明序列语法分析 
		status VarDeclaration(syntaxtree &T);	//外部变量声明处理函数 
		status FunctionDeclaration();	//函数声明处理函数 
		status FunctionDefine();		//函数定义处理函数 
		status ParameterList(syntaxtree& T);//形参序列处理函数 
		syntaxtree Parameter();	//形参处理函数 
		syntaxtree TypeSpecifier();			//类型声明处理函数
		syntaxtree Identifier(int state);			//标识符处理函数 
		 
		
		
		syntaxtree CompoundStmd();		//复合语句处理函数
		syntaxtree StatementList();		//语句序列处理函数
		syntaxtree Statement();			//语句处理函数 
		syntaxtree Expression();		//表达式处理函数
		syntaxtree LocalVar();			//局部变量处理函数 
		
		
		syntaxtree ReturnState();	//处理return语句 
		syntaxtree BreakState();	//处理break语句 
		 
		status ExVarDeclaration();			//外部变量声明处理函数 
		status FunctionDefine(syntaxtree &T);	//函数定义函数
		status InsertSibling(syntaxtree& p,syntaxtree& q);//插入p的兄弟节点q 
		status InsertChild(syntaxtree&p,syntaxtree &q);	//插入p的孩子节点q 
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
		int errorflag;			//语法树错误标志 
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
