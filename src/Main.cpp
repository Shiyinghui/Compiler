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
int Flag = 1;   //�﷨�����У��Ƿ�����־������ʱ���丳ֵΪ0
int num = 1; // ��������У���Ԫʽ��ʱ���� t ���±�
int t;
vector<token>::iterator word;
stack <token> SEM;
string QUAT[4] = { "" }; // ��Ԫʽ����				
token  curToken;  
FinalCode FCtemp;

int main()
{
	Tokenizer tokenizer("src.c");
	cout << "-------------�ʷ�����-------------"<<endl;

	cout << "1.  Դ�������е��ʵ� token ���У�" << endl;
    tokenizer.findAllToken();

	cout << "2.  �ؼ��ֱ�" << endl;
	tokenizer.outputList(tokenizer.keywdSet);

	cout << "3.  �����" << endl;
	tokenizer.outputList(tokenizer.delimitSet);

	cout << "4.  ������" << endl;
    tokenizer.outputList(tokenizer.constSet);

	cout << "5.  ���б�ʶ����" << endl;
	tokenizer.outputList(tokenizer.identifierSet);

	

	word = tokenizer.tokenSet.begin();
	Program();
	if ((*word).tokenValue == 100 && Flag == 1)   // #
		cout << "�﷨��ȷ��" << endl;
	else cout << "�����﷨����" << endl;
	cout << endl;
	cout << "Ŀ��������£�" << endl;
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

