#pragma once
#include <iostream>
#include <string>
#include<stdlib.h>
#include<sstream>
#include<iomanip>
#include<stack>
#include<stdio.h>

using namespace std;

typedef struct
{
	int flag=1;
	string word;
}operatorN;

typedef struct
{
	string operator0;
	string operator1;
	string operator2;
	string operatorex;
	void showASS() {
		cout << setw(8) <<operatorex<<setw(4)<<operator0 << setw(4) << operator1 << "," << operator2 << endl;
	}
}assembly;


typedef operatorN q_operator[4];

class  FinalCode
{
public:
	 FinalCode();
	~FinalCode();
	int i = 0;//目标代码数组索引
	void Code(q_operator q);//汇编代码生成
	void nToq(string* n);//四元式数组向含活跃状态的数组转换
	string expToass(string s);//运算符号与汇编指令的对应
	assembly assum[100];//目标代码数组
private:
	int Jt = 1;//跳转指令的编号
	stringstream ss;//跳转编号的前缀
	stack<int> sw;//有关跳转的栈
	operatorN RDL = { 0,"0" };
};