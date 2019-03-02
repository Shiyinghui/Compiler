#include"SyntaxParsing.h"
#include"LexiAnalysis.h"
#include"SemanticAnalyser.h"
#include"SymbolTable.h"
#include<cstdlib>

extern int Flag;
extern  vector<token>::iterator word;
extern stack <token> SEM;

int countVa = 0; // �Ը�ֵ�������������ʱ��������������ֵ����¼��ʶ������
int tip = 0; //++,-- ��Ϊ��׺������Ԫʽʱ�ĸ����ж�
int ifLoop = 0; //  �ж�Դ�������Ƿ���ѭ��

 ///////////////////////////zyh
SymbolTable Stb;
token* tk_newFun = nullptr;
bool flag_newProcess = true;
int newSpecifier = -1;
int dim[] = { 0 };
///////////////////////////zyh


void Program()
{
	cout << endl;
	cout << "-------------������Ԫʽ����-------------"<<endl;
	cout << endl;
	ExtDefList();
	cout << endl;
	cout << "-------------���ű�-------------"<<endl;
	Stb.show();
}

void ExtDefList()
{
	DefList();
	FunDef();
	CompSt();
}

void FunDef()
{
	if ((*word).tokenValue == 5) // int 
	{
		newSpecifier = (*word).tokenValue;///////////////////////////zyh

		word++;
		if ((*word).tokenValue == 31) // main
		{
			///////////////////////////zyh
			tk_newFun = &(*word);
			flag_newProcess = false;
			if (!Stb.AddToken(*word))
			{
				cout << "�ظ����壺" << word->tokenWord << endl;
				Flag = 0;
			}
			Stb.setCAT(*word, FUN_);
			Stb.setTYPEL(*word, emTVAL(newSpecifier));
			///////////////////////////zyh

			word++;
			if ((*word).tokenValue == 60) // (
			{
				Stb.beginDefFunction(*tk_newFun);///////////////////////////zyh
				flag_newProcess = false;///////////////////////////zyh
				word++;
				if ((*word).tokenValue == 61) // )
					word++;
				else
				{
					cout<< "Syntax Error in Program !" << endl;
					Flag = 0;
				}
			}
			else
			{
				cout<< "Syntax Error in Program !" << endl;
				Flag = 0;
			}

		}
		else
		{
			cout << "Syntax Error in Program !" << endl;
			Flag = 0;
		}
	}
	else
	{
		cout << "Syntax Error in Program !" << endl;
		Flag = 0;
	}
}


// ������ �������
void CompSt()  // CompSt -> { DefList Statement }
{
	if ((*word).tokenValue == 56) // " { "
	{
		///////////////////////////zyh
		if (tk_newFun == nullptr || flag_newProcess)
		{
			Stb.beginNewProcess();
		}
		else
		{
			flag_newProcess = true;
			//int fun /*Stb.beginDefFunction(*tk_newFun);*/ 
			//         (int x){...}    
		}
		///////////////////////////zyh

		word++;
		DefList();     // ������������б�
		Statement();   // ��������б�
		if ((*word).tokenValue == 57) //  " } "
		{
			///////////////////////////zyh
			if (tk_newFun == nullptr)
			{
				Stb.endLastProcess();
			}
			else
			{
				Stb.endDefFunction(*tk_newFun);
				tk_newFun = nullptr;
			}
			///////////////////////////zyh
			word++;
		}
		else
		{
			cout << "Syntax Error in Program !"<<endl;
			Flag = 0;
		}
	}
	else
	{
		cout << "Syntax Error in Program��"<<endl;
		Flag = 0;
	}
}

void Statement()
{
	if ((*word).tokenValue == 5 || (*word).tokenValue == 6 || (*word).tokenValue == 7 || (*word).tokenValue == 8 ||
		(*word).tokenValue == 9 || (*word).tokenValue == 10 || (*word).tokenValue == 11 || (*word).tokenValue == 12)
		// int char float double short long signed unsigned
	{       
		DefList();
		Statement();
	}
	if ((*word).tokenValue == 0 || (*word).tokenValue == 13 || (*word).tokenValue == 15 || (*word).tokenValue == 56
		|| (*word).tokenValue == 30 || (*word).tokenValue == 21 || (*word).tokenValue == 22)
	{
		Stmt();              // identifier, if , while, {, return , break , continue
		Statement();
	}
}



// �����������ӳ���
void StmtList()      // StmtList -> Stmt StmtList | ��
{
	if ((*word).tokenValue == 0 || (*word).tokenValue == 13 || (*word).tokenValue == 15 || (*word).tokenValue == 56
		|| (*word).tokenValue == 30 || (*word).tokenValue == 21 || (*word).tokenValue == 22)
	{
		Stmt();              // identifier, if , while, {, return , break , continue
		StmtList();
	}
}

// �������
void Stmt()       // Stmt -> AssignList | if (Exp) Stmt Other | CompSt | return Exp ; | while (Exp) Stmt | break; | continue;
{
	if ((*word).tokenValue == 0)  // identifier
	{
		AssignList();     // �Ǹ�ֵ���
	}
	else if ((*word).tokenValue == 13)  // keyword : if
	{
		word++;           // �� if �������
		if ((*word).tokenValue == 60) // ' ( '
		{
			word++;
			Exp();
			
			if ((*word).tokenValue == 61) // ' ) '
			{
				IF();   //   ���嶯�� if 
				word++;
				Stmt();
				Other();
				IE();     //  ���嶯��  ie
			}
			else
			{
				cout << "Syntax Error in Program !(probably lack ')' at if () )" << endl;
				Flag = 0;
			}
		}
		else
		{
			cout << "Syntax Error in Program !(probably lack ')' at if () )" << endl;
			Flag = 0;
		}
	}
	else if ((*word).tokenValue == 56) // ' { '
		CompSt();            // �Ǹ������
	else if ((*word).tokenValue == 30)  // keyword : return
	{
		word++;
		Exp();       
		if ((*word).tokenValue == 63) // ' ; '
		{
		    RETURN();           //���嶯��  return ����Ԫʽ
			word++;
		}
		else
		{
			cout << "Syntax Error in Program ! " << endl;
			Flag = 0;
		}
	}
	else if ((*word).tokenValue == 15) // keyword:  while
	{                                                   // while ���
	    WH();	             // while ���������Ԫʽ
		word++;
		if ((*word).tokenValue == 60) // ' ( '
		{
			word++;
		     Exp();
			if ((*word).tokenValue == 61) // ' ) '
			{
				DO();                    //���嶯��  while ѭ����� DO ����Ԫʽ
				ifLoop = 1;           // �����г���ѭ����ѭ����־�� 1
 				word++; 
				Stmt();
				ifLoop = 0;          //  whileѭ��������ѭ����־����Ϊ 0
				//WhStmt();
			    WE();                     //���嶯��  while ��������Ԫʽ
			}
			else
			{
				cout << "Syntax Error in Program !(probably lack ')' at while () )" << endl;
				Flag = 0;
			}
		}
		else
		{
			cout << "Syntax Error in Program ! (probably lack '(' at while () )" << endl;
			Flag = 0;
		}
	}
	else if ((*word).tokenValue == 21) // break ; 
	{
		if (ifLoop == 1)
		{
			word++;
			if ((*word).tokenValue == 63)  //  ;
			{

				BREAK();  // ���嶯��  break ������Ԫʽ
				word++;
			}
		}
		else
		{
			word = word + 2; //////////////
			Flag = 0;
			cout << "Syntax Error ! (break statement not within loop !)" << endl;
			return; //////////////////////////////////////
		}
	}
	else if ((*word).tokenValue == 22)    // continue;
	{
		if (ifLoop == 1)
		{
			word++;
			if ((*word).tokenValue == 63)  //  ;
			{

				CONTINUE();  // ���嶯��  continue ������Ԫʽ
				word++;
			}
		}
		else
		{
			word = word + 2; //////////////
			Flag = 0;
			cout << "Syntax Error ! (continue statement not within loop !)" << endl;
			return; ////////////////////////////////
		}
	}
			
	else
	{
		cout << "Syntax Error in Program !" << endl;
		Flag = 0;
		
	}
}

void Other()
{
	if ((*word).tokenValue == 14) // else
	{
        EL();              // ���嶯�� else ����Ԫʽ
		word++;
		Stmt();
	}
}
/*
void WhStmt()
{
	KwStmt();
	Stmt();
	KwStmt();
}

void KwStmt()
{
	if ((*word).tokenValue == 21 || (*word).tokenValue == 22) //  keyword:  break , continue
	{
		if ((*word).tokenValue == 21)
		{
			word++;
			if ((*word).tokenValue == 63)  //  ;
			{
				word++;
			    BREAK();
			}
			else
			{
				Flag = 1;
				cout << "Error: lacking ' ; ' after ' break ' !" << endl;
			}
		}
		else
		{
			word++;
			if ((*word).tokenValue == 63)  //  ;
			{
				word++;
			    CONTINUE();
			}
			else
			{
				Flag = 1;
				cout << "Error: lacking ' ; ' after ' continue ' !" << endl;
			}
		}
	}

}*/

/*void S()
{
	if ((*word).tokenValue == 0 || (*word).tokenValue == 60 || (*word).tokenValue == 3 || (*word).tokenValue == 51
		|| (*word).tokenValue == 47 || (*word).tokenValue == 48)
		Exp();
}*/

void Exp()
{
	C();
}

void C()
{
	D();
	C1();
}

void C1()
{
	if ((*word).tokenValue == 50)  // ||
	{
		word++;
		D();
	   OR();            //���嶯��   �߼���
		C1();
    }
}

void D()
{
	E();
	D1();
}

void D1()
{
	if ((*word).tokenValue == 49) // &&
	{
		word++;
		E();
		AND();            // ���嶯��  �߼���
		D1();
	}
}

void E()
{
	if ((*word).tokenValue == 51) // !
	{
		word++;
		E();
		NOT();           //  ���嶯��  �߼���
	}

	else	if ((*word).tokenValue == 38) //  -  �˴����� ȡ��
	{
		word++;
		E();
		MINUS();           //   ���嶯��  ȡ�� //////////////
	}
		
	else if ((*word).tokenValue == 60 || (*word).tokenValue == 0 ||	(*word).tokenValue == 47 || (*word).tokenValue == 48 || (*word).tokenValue == 3 || (*word).tokenValue == 1)
		F();                             // (,  identifier,  ++, --, const character
	else
	{
		cout << "Syntax Error in Program !" << endl;
		Flag = 0;
	}
}

void F()
{
	G();
	F1();
}

void F1()
{
	if (((*word).tokenValue >= 32 && (*word).tokenValue <= 36) || (*word).tokenValue == 46) //  sign2:  >, <, >=, <=, ==, != 
	{
		if ((*word).tokenValue == 32)
		{
			word++;
			G();
			GT();     // >   ���嶯��   great than 
			F1();
		}

		if ((*word).tokenValue == 33)
		{
			word++;
			G();
			LT();       //   ���嶯��  <    less than 
			F1();
		}

		if ((*word).tokenValue == 34)
		{
			word++;
			G();
			GE();       //  ���嶯��   >=    great equal 
			F1();
		}
		if ((*word).tokenValue == 35)
		{
			word++;
			G();
			LE();       // ���嶯��   <=    less equal 
			F1();
		}
		if ((*word).tokenValue == 36)
		{
			word++;
			G();
			EQ();       // ���嶯��   ==     equal 
			F1();
		}
		if ((*word).tokenValue == 46)
		{
			word++;
			G();
			NE();       // ���嶯��   !=    not equal 
			F1();
		}
	}
}

void G()
{
	H();
	G1();
}

void G1()
{
	if ((*word).tokenValue == 37)   // sign3 : + 
	{
		word++;
		H();
		GEQ("+");  // ���嶯��  G1 -> + H { GEQ(+) } G1 | - H { GEQ(-) } G1 | ��  
		G1();
	}
	if ((*word).tokenValue == 38)  // sign3: -
	{
		word++;
		H();
		GEQ("-");  //  ���嶯�� ����
		G1();
    }
}

void H()
{
	I();
	H1();
}

void H1()
{
	if ((*word).tokenValue == 39) // sign4 : *  
	{
		word++;
		I();
		GEQ("*");    //  ���嶯�� �˷�
		H1();
	}

	if ((*word).tokenValue == 40) // sign4: /
	{
		word++;
		I();
		GEQ("/");      //  ���嶯�� ����
		H1();
	}
}

void I()
{
	if ((*word).tokenValue == 60) // (
	{
		word++;
		C();   // or  Exp();
		if ((*word).tokenValue == 61) // )
			word++;
	}
	else if ((*word).tokenValue == 0) // identifier
	{
		Va();
         J();
	}
	else if ((*word).tokenValue == 47) // ++
	{
		word++;
		Va();
		GEQ("before++");        //  ���嶯�� ǰ׺����
	}
	else if ((*word).tokenValue == 48)  // --
	{
		word++;
		Va();
		GEQ("before--");	   //  ���嶯�� ǰ׺�Լ�
	}
	else if ((*word).tokenValue == 3) // const
	{
		SEM.push(*word);     //  ���嶯�� ������ջ
		word++;
	}
	else if ((*word).tokenValue == 1) // char ����
	{
		SEM.push(*word);    //  ���嶯�� �ַ��ͳ�����ջ
		word++;
	}
	else
	{
		cout << "error in I" << endl;
		Flag = 0;
	}
}

void J()        // ��׺ ++ --
{
	if ((*word).tokenValue == 47)
	{
		tip = 1;
		//GEQ("++After");
		word++;
	}
	if ((*word).tokenValue == 48)
	{
		tip = 2;
		//GEQ("--After");
		word++;
	}
}

void Va()
{
	if ((*word).tokenValue == 0) // identifier
	{
		SEM.push(*word);   //  ���嶯�� ��ʶ����ջ
		word++;
		Va1();
	}
}

/*void Va1()
{
	if ((*word).tokenValue == 58) // [
	{
		word++;
		if ((*word).tokenValue == 3) // [ const ]
		{
			word++;
			if ((*word).tokenValue == 59) // ]
				word++;
			else
			{
				cout << "ȱ��' ] '" << endl;
				Flag = 0;
			}
		}
		else
		{
			cout << "������ʽ�﷨����" << endl;
			Flag = 0;
		}
	}
}*/



// ��ֵ�������ӳ���
void AssignList()   //AssignList -> Assign AssignList | ��    ����һ����丳ֵ���ʽ
{
	if ((*word).tokenValue == 0)         //identifier 
	{
		Assign();
		AssignList();
	}
}

void Assign()    //Assign -> Ass Assign1
{
	Ass();
	Assign1();
}


void Assign1() // Assign1 -> , Ass Assign1 | ;             // , ����һ����ֵ�������ֶ����ֵ���ʽ
{
	if ((*word).tokenValue == 62) // ,
	{
		word++;
		Ass();
		Assign1();
	}
	else if ((*word).tokenValue == 63) // 63
		word++;
	else
	{
		cout << "Syntax Error in Program !" << endl;
		Flag = 0;
	}
}

void Ass()  // Ass -> Va sign1 Ass | Exp // sign1 : =, +=, -=, *=, / =, %= 
{
	
	int tempValue = (*(++word)).tokenValue; // tempValue ���������ж������ĸ�����ʽ
    word--;
	int temp;
	word = word + 4;
	temp = (*word).tokenValue;
	word = word - 4;
   if ((*word).tokenValue == 0 && tempValue >= 41 && tempValue <= 45)    //identifier ��ʶ��
	{

		Va();
		if ((*word).tokenValue >= 41 && (*word).tokenValue <= 45)  // sign1 : +=, -=, *=, /=, =
		{
			countVa++;

			string sign1 = (*word).tokenWord; ///// ��¼����ֵ�������β�
			word++;
			Ass();
			ASSI(sign1);    //  ���嶯�� ��ֵ
			countVa = 0;
		}
		else
		{
			cout << "Syntax Error in Program !" << endl;
			Flag = 0;
		}
	}
   else if ((*word).tokenValue == 0 && temp >= 41 && temp <= 45)
   {
	   Va();
	   if ((*word).tokenValue >= 41 && (*word).tokenValue <= 45)  // sign1 : +=, -=, *=, /=, =
	   {
		   countVa++;

		   string sign1 = (*word).tokenWord; ///// ��¼����ֵ�������β�
		   word++;
		   Ass();
		   ASSI(sign1);    //  ���嶯�� ��ֵ
		   countVa = 0;
	   }
	   else
	   {
		   cout << "Syntax Error in Program !" << endl;
		   Flag = 0;
	   }
   }
	else if ((*word).tokenValue == 0 || (*word).tokenValue == 60 || (*word).tokenValue == 3 || (*word).tokenValue == 51
		|| (*word).tokenValue == 47 || (*word).tokenValue == 48 || (*word).tokenValue == 38 || (*word).tokenValue == 1)
		//����Exp()���ж����ַ��� ����identifier or "(" or constant or  ++ or -- or ! or - or character
	{
		Exp();
	}
	else
	{
		cout << "Syntax Error in Program !" << endl;
		Flag = 0;
	}

}
 

// ���������������ӳ���
void DefList()  //DefList -> Def  DefList | ��
{

	int tempValue = (*(++word)).tokenValue;////////////////////////////
	word--;
	if (tempValue != 31)  //  �������������� ��main�� 
	{
		if ((*word).tokenValue == 5 || (*word).tokenValue == 6 || (*word).tokenValue == 7 || (*word).tokenValue == 8 ||
			(*word).tokenValue == 9 || (*word).tokenValue == 10 || (*word).tokenValue == 11 || (*word).tokenValue == 12)
		{      // int char float double short long signed unsigned

			Def();
			DefList();
		}
	}
}

void Def()  //Def -> Specifier DecList;  Specifier : int char float double short long signed unsigned
{
	
    if ((*word).tokenValue == 5 || (*word).tokenValue == 6 || (*word).tokenValue == 7 || (*word).tokenValue == 8 ||
		(*word).tokenValue == 9 || (*word).tokenValue == 10 || (*word).tokenValue == 11 || (*word).tokenValue == 12)		
	{        // int char float double short long signed unsigned
		newSpecifier = (*word).tokenValue;///////////////////////////zyh
		word++;
		DecList();
		if ((*word).tokenValue == 63)  // " ; "
		{
			newSpecifier = -1;///////////////////////////zyh
			word++;
		}
		else
		{
			cout << "lacking ' ; ' in the end of definition of Variable " << endl;
			Flag = 0;
		}
	}
}


void DecList()          //DecList -> Dec Dec1
{
	Dec();
	Dec1();
}

void Dec1()        //Dec1 -> , Dec Dec1 | ��
{
	if ((*word).tokenValue == 62)  // ','
	{
		word++;
		Dec();
		Dec1();
	}
}

void Dec()   // Dec -> Va init
{
	
	Va();
	///////////////////////////zyh
	if (dim[0] == 0)
	{
		if (!Stb.AddToken(*(word - 1)))
		{
			cout << "�ظ����壺" << (word - 1)->tokenWord << endl;
			Flag = 0;
		}
		Stb.setCAT(*(word - 1), VAR_);
		Stb.setTYPEL(*(word - 1), emTVAL(newSpecifier));
	}
	else
	{
		if (!Stb.AddToken(*(word - 4)))
		{
			cout << "�ظ����壺" << (word - 4)->tokenWord << endl;
			Flag = 0;
		}
		Stb.setCAT(*(word - 4), vARRAY_);
		
		Stb.addAINFL_d(*(word - 4), emTVAL(newSpecifier), 1, dim);
		dim[0] = 0;
	}
	
	///////////////////////////zyh
	Init();
}

void Init()     //  Init -> = Init1 | ��
{
	if ((*word).tokenValue == 45) // =
	{
		word++;
		Init1();
	}
}

void Init1() // Init1 -> Exp | { ElemList }  
{
	if ((*word).tokenValue == 0 || (*word).tokenValue == 60 || (*word).tokenValue == 3 || (*word).tokenValue == 51
		|| (*word).tokenValue == 47 || (*word).tokenValue == 48 || (*word).tokenValue == 38 || (*word).tokenValue == 1)
		Exp();
	
	else 	if ((*word).tokenValue == 56) //  {
	{
		word++;
		 ElemList();
		if ((*word).tokenValue == 57) //  }
			word++;
		else
		{
			cout << "Syntax Error in program!" << endl;
			Flag = 0;
		}
	}
	else
	{
		cout << "Syntax Error in program!" << endl;
		Flag = 0;
	}
}

void ElemList()     // ElemList -> elem Elem |  ��   // elem:  const ������character �ַ�
{
	if ((*word).tokenValue == 3 || (*word).tokenValue == 1)
		// Ԫ��Ϊ�������ַ��ͳ��� �� �ַ��ͳ��� �� �����ɻ���ת���������ܻ���warning
	{
		word++;
		Elem();
	}
}

void Elem()   //  Elem ->, elem Elem | ��            // ������ʼ��   
{
	if ((*word).tokenValue == 62)  // ,
	{
		word++;
		if ((*word).tokenValue == 3 || (*word).tokenValue == 1) //  elem
		{  
			word++;
		    Elem();
		}
		else
		{
			cout << "Syntax Error in program!" << endl;
			Flag = 0;
		}
	}
}

void Va1()  //  Va1 -> [ ���ͳ��� ] |  �� //  ����
{
	token elemToken; /////////// δ��ʼ��
	if ((*word).tokenValue == 58)  // [
	{
		elemToken.tokenWord += (*word).tokenWord;
		//SEM.push(*word);
		word++;
		if ((*word).tokenValue == 3 && (*word).tokenType == 2) // ���ͳ���
		{
			if(newSpecifier!=-1)  dim[0]=atoi((*word).tokenWord.c_str());
			elemToken.tokenWord += (*word).tokenWord;
			//SEM.push(*word) ;
			word++;
			if ((*word).tokenValue == 59) // ]
			{
				elemToken.tokenWord += (*word).tokenWord;
				elemToken.tokenValue = 0;
				elemToken.tokenType = ArrayElem_TYPE;
			    SEM.push(elemToken);   //     ��������ջ
				word++;
			}
			else
			{
				cout << "Syntax Error in program!" << endl;
				Flag = 0;
			}
		}
		else
		{
			cout << "Syntax Error in program!" << endl;
			Flag = 0;
		}
	}
}



