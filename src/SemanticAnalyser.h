#ifndef SEMANTIC_ANALYSER_H
#define SEMANTIC_ANALYSER_H

#include<iostream>
#include <stdlib.h>
#include<stdio.h>
#include<string>
#include <iomanip>
#include<vector>
#include<stack>
using namespace std;

// ��ֵ�����Ԫʽ����
void ASSI(string sign1);

// if �����Ԫʽ��غ���
void IF();
void EL();
void IE();

// while �����Ԫʽ��غ���
void WH();
void DO(); 
void WE();
void BREAK();
void CONTINUE();

void RETURN(); // return ��Ԫʽ����

// ���ʽ��Ԫʽ��غ���
void GEQ(string Operator); // +, -, *, /, ++, --
void OR();
void AND();
void NOT();
void MINUS();
void GT();
void LT();
void GE();
void LE();
void EQ();
void NE();



#endif 

