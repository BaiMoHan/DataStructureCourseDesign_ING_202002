#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//借助vector实现tokenlist 
#include<string>	//借助String类 
using namespace std;
typedef enum{		//定义词法需要的识别码 
	ERROR_TOKEN=1,LP,RP,PML,PMR,SEMI,COMMA,INT_CONST,FLOAT_CONST,DOUBLE_CONST,
	LONG_CONST,CHAR_CONST,VOID,RETURN,IF,ELSE,WHILE,FOR,BREAK,CONTINUE,ADD,REDUCE,
	MUTIPLEY,DIVIDE,ADDSELF,DIVIDESELF,ASSIGN,EQ,ADD_EQ,DIVIDE_EQ,NUM,STRING_CONST,
	SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,INT,DOUBLE,CHAR,LONG,LESS,GRETER,MOD,NOTEQ,
	BRACKETL,BRACKETR,AND,OR,INCLUDE,OTHERS,ENDFILE,COMMENT 
}TokenType; 	//定义各类单词的类别码 
typedef struct token{
	TokenType tokentype;	//词的识别码
	string tokenstring;		//词本身的值
	int linenum;			//所在的行号 
}token;

class Lexer	//词法分析类 
{
	public:
		Lexer();		//构造函数 
		void analysis(char filename[]);
		void Disp();
	private:
		bool analysis_flag;		//词法分析是否成功的标志变量 
		char filename[30];		//测试文件名 
		int  linecount;			//行号计数器
		vector<token> tokenlist;//存放自动机识别出来的词
		int vectorindex;		//tokenlist的取词索引 
};

#endif
