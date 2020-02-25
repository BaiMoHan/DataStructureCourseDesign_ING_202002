#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H
#include<vector>	//借助vector实现tokenlist 
#include<string>	//借助String类 
#define MAXINDEX 100	//数组最大容量 
//class Parser;

using namespace std;
typedef enum{		//定义词法需要的识别码 
	ERROR_TOKEN=1,LP,RP,PML,PMR,SEMI,COMMA,INT_CONST,FLOAT_CONST,DOUBLE_CONST,
	LONG_CONST,CHAR_CONST,VOID,RETURN,IF,ELSE,WHILE,FOR,BREAK,CONTINUE,ADD,REDUCE,
	MUTIPLY,DIVIDE,ADDSELF,REDUCESELF,ASSIGN,EQ,ADD_EQ,REDUCE_EQ,MUTIPLY_EQ,DIVIDE_EQ,NUM,STRING_CONST,
	SQUTOE_L,SQUTOE_R,DQUTOE_L,DQUTOE_R,INT,DOUBLE,CHAR,LONG,LESS,GREATER,MOD,NOTEQ,
	BRACKETL,BRACKETR,AND,OR,INCLUDE,OTHERS,ENDFILE,COMMENT,ID,FLOAT,CONST,MAIN
}TokenType; 	//定义各类单词的类别码 
typedef struct token{
	TokenType tokentype;	//词的识别码
	string tokenstring;		//词本身的值
	int times;				//出现的次数 
	int linenum;			//所在的行号 
}token;

class Lexer	//词法分析类 
{
	public:
		Lexer();		//构造函数 
		friend class Parser;	//友元类 
		void analysis(char filename[]);
		void Disp();
		void PrintWords();//词法分析成功后输出识别出来的词 
	private:
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
