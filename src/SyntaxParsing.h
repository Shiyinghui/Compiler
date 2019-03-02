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
void Statement();  // Statement -> StmtList Statement | DefList Statement | ��

//������
void CompSt();  //compound statement  ������� CompSt -> { DefList Statement }
void StmtList();  // statement list  ��������б�    StmtList -> Stmt StmtList | ��
void DefList();   // define list   ��������б�         DefList -> Def DefList | ��
void Stmt();      // statement  �������      Stmt -> AssignList | if (Exp) { IF () } Stmt Other { IE () } | CompSt  | break ; { BREAK() } | continue ;{ CONTINUE() }
                                                                //  | return Exp(or return S) ; { RETURN() } | while { WH() }  (Exp) { DO() } Stmt 
void S();
void Other();   // Other -> else { EL() }  Stmt | ��

void Def();      // ��������                  Def -> Specifier DecList;       
void DecList();  // ����ı����б�     DecList -> Dec Dec1
void Dec();       //����ı���             Dec -> Va Init             // Init ���г�ʼ��
void Dec1();     //�ķ��任�м����  Dec1 -> , Dec Dec1| ��            // , ��������������
void Init();      //��������ʱ��ʼ��    Init -> = Init1 | ��    
void Init1();    // �ķ��任�м����   Init1 -> Exp | { ElemList }  
void Va();     // Va -> identifier  { PUSH(identifier) }             // identifier ��ʶ��
void Va1();
void ElemList();
void Elem();
void AssignList();  // ��ֵ����б�    AssignList -> Assign AssignList | �� 
void Assign();    //��ֵ���                Assign -> Ass Assign1
void Assign1();  //   Assign1 -> , Ass Assign1 | ;     , ����һ����ֵ�������ֶ����ֵ���ʽ
void Ass();        // �ķ��任�м���� Ass -> Va sign1 Ass { ASSI() }| Exp  // sign1 : =, +=, -=, *=, / =, %=   


// ���ʽ�ķ���ز���ʽ
void Exp();    // Exp -> C;   // ���ʽ
void C();      //   C -> D C1
void C1();     //  C1 -> || D { OR() } C1 | ��
void D();      //   D -> E D1
void D1();    //  D1 -> && { AND () } E D1 |��
void E();     //   E -> ! E { NOT () } | - E { MINUS () }| F 
void F();      //  F -> G F1
void F1();   //   F1 -> sign2 G { sign2()} F1 | ��     
void G();    //    G -> H G1
void G1();  //    G1 -> sign3 H { GEQ(sign3) } G1 | ��        
void H();    // H->I H1
void H1();   // H1->sign4 I{ GEQ(sign4) }  H1 | ��             // sign4: *, /
void I();     //  I -> (Exp) | Va J | ++ Va  { GEQ(++) }   | -- Va   { GEQ(--) } | const  { PUSH(const) }  | char    // const: ʵ�ͻ����ͳ��� 
void J();    // J -> ++ | -- | ��

#endif
