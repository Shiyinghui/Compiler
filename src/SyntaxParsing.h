#ifndef _LOGICEXP_H
#define _LOGICEXP_H
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include<vector>
#include"LexiAnalysis.h"

void Program();       //  Program -> ExtDefList
void ExtDefList();    // ExtDefList->FunDef CompSt
void FunDef();         // FunDef -> int main ()
void Statement();  // Statement -> StmtList Statement | DefList Statement | ε

//函数体
void CompSt();  //compound statement  复合语句 CompSt -> { DefList Statement }
void StmtList();  // statement list  陈述语句列表    StmtList -> Stmt StmtList | ε
void DefList();   // define list   定义语句列表         DefList -> Def DefList | ε
void Stmt();      // statement  陈述语句      Stmt -> AssignList | if (Exp) { IF () } Stmt Other { IE () } | CompSt  | break ; { BREAK() } | continue ;{ CONTINUE() }
                                                                //  | return Exp(or return S) ; { RETURN() } | while { WH() }  (Exp) { DO() } Stmt 
void S();
void Other();   // Other -> else { EL() }  Stmt | ε

void Def();      // 变量定义                  Def -> Specifier DecList;       
void DecList();  // 定义的变量列表     DecList -> Dec Dec1
void Dec();       //定义的变量             Dec -> Va Init             // Init 进行初始化
void Dec1();     //文法变换中间符号  Dec1 -> , Dec Dec1| ε            // , 连续定义多个变量
void Init();      //变量定义时初始化    Init -> = Init1 | ε    
void Init1();    // 文法变换中间符号   Init1 -> Exp | { ElemList }  
void Va();     // Va -> identifier  { PUSH(identifier) }             // identifier 标识符
void Va1();
void ElemList();
void Elem();
void AssignList();  // 赋值语句列表    AssignList -> Assign AssignList | ε 
void Assign();    //赋值语句                Assign -> Ass Assign1
void Assign1();  //   Assign1 -> , Ass Assign1 | ;     , 允许一条赋值语句里出现多个赋值表达式
void Ass();        // 文法变换中间符号 Ass -> Va sign1 Ass { ASSI() }| Exp  // sign1 : =, +=, -=, *=, / =, %=   


// 表达式文法相关产生式
void Exp();    // Exp -> C;   // 表达式
void C();      //   C -> D C1
void C1();     //  C1 -> || D { OR() } C1 | ε
void D();      //   D -> E D1
void D1();    //  D1 -> && { AND () } E D1 |ε
void E();     //   E -> ! E { NOT () } | - E { MINUS () }| F 
void F();      //  F -> G F1
void F1();   //   F1 -> sign2 G { sign2()} F1 | ε     
void G();    //    G -> H G1
void G1();  //    G1 -> sign3 H { GEQ(sign3) } G1 | ε        
void H();    // H->I H1
void H1();   // H1->sign4 I{ GEQ(sign4) }  H1 | ε             // sign4: *, /
void I();     //  I -> (Exp) | Va J | ++ Va  { GEQ(++) }   | -- Va   { GEQ(--) } | const  { PUSH(const) }  | char    // const: 实型或整型常量 
void J();    // J -> ++ | -- | ε

#endif
