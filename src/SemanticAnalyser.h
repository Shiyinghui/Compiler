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

// 赋值语句四元式函数
void ASSI(string sign1);

// if 语句四元式相关函数
void IF();
void EL();
void IE();

// while 语句四元式相关函数
void WH();
void DO(); 
void WE();
void BREAK();
void CONTINUE();

void RETURN(); // return 四元式函数

// 表达式四元式相关函数
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

