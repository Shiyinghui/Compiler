#include <iostream>
#include"LexiAnalysis.h"
#include"SyntaxParsing.h"
#include"SemanticAnalyser.h"
#include"FinalCodeCreate.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<stack>
using namespace std;
int Flag = 1;   //语法分析中，是否出错标志，出错时将其赋值为0
int num = 1; // 语义分析中，四元式临时变量 t 的下标
int t;
vector<token>::iterator word;
stack <token> SEM;
string QUAT[4] = { "" }; // 四元式数组				
token  curToken;  
FinalCode FCtemp;

int main()
{
	Tokenizer tokenizer("src.c");
	cout << "-------------词法分析-------------"<<endl;

	cout << "1.  源程序所有单词的 token 序列：" << endl;
    tokenizer.findAllToken();

	cout << "2.  关键字表：" << endl;
	tokenizer.outputList(tokenizer.keywdSet);

	cout << "3.  界符表：" << endl;
	tokenizer.outputList(tokenizer.delimitSet);

	cout << "4.  常数表：" << endl;
    tokenizer.outputList(tokenizer.constSet);

	cout << "5.  所有标识符：" << endl;
	tokenizer.outputList(tokenizer.identifierSet);

	

	word = tokenizer.tokenSet.begin();
	Program();
	if ((*word).tokenValue == 100 && Flag == 1)   // #
		cout << "语法正确！" << endl;
	else cout << "存在语法错误！" << endl;
	cout << endl;
	cout << "目标代码如下：" << endl;
	for (t = 0; t < FCtemp.i; t++)
	{
		if ((FCtemp.assum[t].operator0 == FCtemp.assum[t + 1].operator0) && (FCtemp.assum[t].operator1 == FCtemp.assum[t + 1].operator1) && (FCtemp.assum[t].operator2 == FCtemp.assum[t + 1].operator2)) {
			if (FCtemp.assum[t].operatorex != "") { FCtemp.assum[t].showASS(); t++; }
			else {}
			continue;
		}
		FCtemp.assum[t].showASS();
	}
	int a = 0;
	cin >> a;
	return 0;
}

