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
	int i = 0;//Ŀ�������������
	void Code(q_operator q);//����������
	void nToq(string* n);//��Ԫʽ�����򺬻�Ծ״̬������ת��
	string expToass(string s);//�����������ָ��Ķ�Ӧ
	assembly assum[100];//Ŀ���������
private:
	int Jt = 1;//��תָ��ı��
	stringstream ss;//��ת��ŵ�ǰ׺
	stack<int> sw;//�й���ת��ջ
	operatorN RDL = { 0,"0" };
};