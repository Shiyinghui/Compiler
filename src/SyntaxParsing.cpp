#include"SyntaxParsing.h"
#include"LexiAnalysis.h"
#include"SemanticAnalyser.h"
#include"SymbolTable.h"
#include<cstdlib>

extern int Flag;
extern  vector<token>::iterator word;
extern stack <token> SEM;

int countVa = 0; // 对赋值语句进行语义分析时，若出现连续赋值，记录标识符个数
int tip = 0; //++,-- 作为后缀生成四元式时的辅助判断
int ifLoop = 0; //  判断源程序中是否有循环

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
	cout << "-------------生成四元式如下-------------"<<endl;
	cout << endl;
	ExtDefList();
	cout << endl;
	cout << "-------------符号表-------------"<<endl;
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
				cout << "重复定义：" << word->tokenWord << endl;
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


// 函数体 复合语句
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
		DefList();     // 变量定义语句列表
		Statement();   // 陈述语句列表
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
		cout << "Syntax Error in Program！"<<endl;
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



// 陈述语句相关子程序
void StmtList()      // StmtList -> Stmt StmtList | 空
{
	if ((*word).tokenValue == 0 || (*word).tokenValue == 13 || (*word).tokenValue == 15 || (*word).tokenValue == 56
		|| (*word).tokenValue == 30 || (*word).tokenValue == 21 || (*word).tokenValue == 22)
	{
		Stmt();              // identifier, if , while, {, return , break , continue
		StmtList();
	}
}

// 陈述语句
void Stmt()       // Stmt -> AssignList | if (Exp) Stmt Other | CompSt | return Exp ; | while (Exp) Stmt | break; | continue;
{
	if ((*word).tokenValue == 0)  // identifier
	{
		AssignList();     // 是赋值语句
	}
	else if ((*word).tokenValue == 13)  // keyword : if
	{
		word++;           // 是 if 条件语句
		if ((*word).tokenValue == 60) // ' ( '
		{
			word++;
			Exp();
			
			if ((*word).tokenValue == 61) // ' ) '
			{
				IF();   //   语义动作 if 
				word++;
				Stmt();
				Other();
				IE();     //  语义动作  ie
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
		CompSt();            // 是复合语句
	else if ((*word).tokenValue == 30)  // keyword : return
	{
		word++;
		Exp();       
		if ((*word).tokenValue == 63) // ' ; '
		{
		    RETURN();           //语义动作  return 的四元式
			word++;
		}
		else
		{
			cout << "Syntax Error in Program ! " << endl;
			Flag = 0;
		}
	}
	else if ((*word).tokenValue == 15) // keyword:  while
	{                                                   // while 语句
	    WH();	             // while 语句的入口四元式
		word++;
		if ((*word).tokenValue == 60) // ' ( '
		{
			word++;
		     Exp();
			if ((*word).tokenValue == 61) // ' ) '
			{
				DO();                    //语义动作  while 循环语句 DO 的四元式
				ifLoop = 1;           // 程序中出现循环，循环标志置 1
 				word++; 
				Stmt();
				ifLoop = 0;          //  while循环结束，循环标志复置为 0
				//WhStmt();
			    WE();                     //语义动作  while 语句出口四元式
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

				BREAK();  // 语义动作  break 语句的四元式
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

				CONTINUE();  // 语义动作  continue 语句的四元式
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
        EL();              // 语义动作 else 的四元式
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
	   OR();            //语义动作   逻辑或
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
		AND();            // 语义动作  逻辑与
		D1();
	}
}

void E()
{
	if ((*word).tokenValue == 51) // !
	{
		word++;
		E();
		NOT();           //  语义动作  逻辑非
	}

	else	if ((*word).tokenValue == 38) //  -  此处用作 取负
	{
		word++;
		E();
		MINUS();           //   语义动作  取负 //////////////
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
			GT();     // >   语义动作   great than 
			F1();
		}

		if ((*word).tokenValue == 33)
		{
			word++;
			G();
			LT();       //   语义动作  <    less than 
			F1();
		}

		if ((*word).tokenValue == 34)
		{
			word++;
			G();
			GE();       //  语义动作   >=    great equal 
			F1();
		}
		if ((*word).tokenValue == 35)
		{
			word++;
			G();
			LE();       // 语义动作   <=    less equal 
			F1();
		}
		if ((*word).tokenValue == 36)
		{
			word++;
			G();
			EQ();       // 语义动作   ==     equal 
			F1();
		}
		if ((*word).tokenValue == 46)
		{
			word++;
			G();
			NE();       // 语义动作   !=    not equal 
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
		GEQ("+");  // 语义动作  G1 -> + H { GEQ(+) } G1 | - H { GEQ(-) } G1 | ε  
		G1();
	}
	if ((*word).tokenValue == 38)  // sign3: -
	{
		word++;
		H();
		GEQ("-");  //  语义动作 减法
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
		GEQ("*");    //  语义动作 乘法
		H1();
	}

	if ((*word).tokenValue == 40) // sign4: /
	{
		word++;
		I();
		GEQ("/");      //  语义动作 除法
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
		GEQ("before++");        //  语义动作 前缀自增
	}
	else if ((*word).tokenValue == 48)  // --
	{
		word++;
		Va();
		GEQ("before--");	   //  语义动作 前缀自减
	}
	else if ((*word).tokenValue == 3) // const
	{
		SEM.push(*word);     //  语义动作 常量入栈
		word++;
	}
	else if ((*word).tokenValue == 1) // char 类型
	{
		SEM.push(*word);    //  语义动作 字符型常量入栈
		word++;
	}
	else
	{
		cout << "error in I" << endl;
		Flag = 0;
	}
}

void J()        // 后缀 ++ --
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
		SEM.push(*word);   //  语义动作 标识符入栈
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
				cout << "缺少' ] '" << endl;
				Flag = 0;
			}
		}
		else
		{
			cout << "数组表达式语法错误！" << endl;
			Flag = 0;
		}
	}
}*/



// 赋值语句相关子程序
void AssignList()   //AssignList -> Assign AssignList | 空    产生一句或多句赋值表达式
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


void Assign1() // Assign1 -> , Ass Assign1 | ;             // , 允许一条赋值语句里出现多个赋值表达式
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
	
	int tempValue = (*(++word)).tokenValue; // tempValue 用来辅助判断是用哪个产生式
    word--;
	int temp;
	word = word + 4;
	temp = (*word).tokenValue;
	word = word - 4;
   if ((*word).tokenValue == 0 && tempValue >= 41 && tempValue <= 45)    //identifier 标识符
	{

		Va();
		if ((*word).tokenValue >= 41 && (*word).tokenValue <= 45)  // sign1 : +=, -=, *=, /=, =
		{
			countVa++;

			string sign1 = (*word).tokenWord; ///// 记录符号值，用作形参
			word++;
			Ass();
			ASSI(sign1);    //  语义动作 赋值
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

		   string sign1 = (*word).tokenWord; ///// 记录符号值，用作形参
		   word++;
		   Ass();
		   ASSI(sign1);    //  语义动作 赋值
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
		//进入Exp()，判断首字符是 否是identifier or "(" or constant or  ++ or -- or ! or - or character
	{
		Exp();
	}
	else
	{
		cout << "Syntax Error in Program !" << endl;
		Flag = 0;
	}

}
 

// 变量定义语句相关子程序
void DefList()  //DefList -> Def  DefList | 空
{

	int tempValue = (*(++word)).tokenValue;////////////////////////////
	word--;
	if (tempValue != 31)  //  不是主函数定义 （main） 
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

void Dec1()        //Dec1 -> , Dec Dec1 | 空
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
			cout << "重复定义：" << (word - 1)->tokenWord << endl;
			Flag = 0;
		}
		Stb.setCAT(*(word - 1), VAR_);
		Stb.setTYPEL(*(word - 1), emTVAL(newSpecifier));
	}
	else
	{
		if (!Stb.AddToken(*(word - 4)))
		{
			cout << "重复定义：" << (word - 4)->tokenWord << endl;
			Flag = 0;
		}
		Stb.setCAT(*(word - 4), vARRAY_);
		
		Stb.addAINFL_d(*(word - 4), emTVAL(newSpecifier), 1, dim);
		dim[0] = 0;
	}
	
	///////////////////////////zyh
	Init();
}

void Init()     //  Init -> = Init1 | ε
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

void ElemList()     // ElemList -> elem Elem |  ε   // elem:  const 常量，character 字符
{
	if ((*word).tokenValue == 3 || (*word).tokenValue == 1)
		// 元素为常数或字符型常量 ， 字符型常量 和 常数可互相转化，但可能会有warning
	{
		word++;
		Elem();
	}
}

void Elem()   //  Elem ->, elem Elem | ε            // 连续初始化   
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

void Va1()  //  Va1 -> [ 整型常量 ] |  ε //  数组
{
	token elemToken; /////////// 未初始化
	if ((*word).tokenValue == 58)  // [
	{
		elemToken.tokenWord += (*word).tokenWord;
		//SEM.push(*word);
		word++;
		if ((*word).tokenValue == 3 && (*word).tokenType == 2) // 整型常量
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
			    SEM.push(elemToken);   //     将整体入栈
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



