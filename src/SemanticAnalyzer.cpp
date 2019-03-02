#include "SyntaxParsing.h"//////////////change
#include "LexiAnalysis.h"
#include"FinalCodeCreate.h"
#include"SemanticAnalyser.h"
#include"SymbolTable.h"
#include <iomanip>
extern int Flag;  // 判断源程序语法是否正确标志
extern  vector<token>::iterator word;  // 迭代器
extern string QUAT[4];        // 四元式
extern token  curToken;
extern stack<token>SEM;  // 语义栈

/*  进行语义分析时一些辅助变量 */
extern int num;
extern int tip;               // 自增、自减 后缀时的判断
extern int countVa;     //  连续赋值操作时记录被赋值变量的个数
extern SymbolTable Stb;   // 符号表
string tempVa;  // 临时变量
token temp_token;  // 临时变量
extern FinalCode FCtemp;
string q_temp[4] = {};

void GEQ(string Operator)
{

	int error = 0;
	/*if (end_program == 1)
	return;
	*/
	token vaAddSub;        // 变量后缀自增或自减运算符，暂存


	if (Operator == "+" || Operator == "-" || Operator == "*" || Operator == "/")
	{
		QUAT[0] = Operator;  //      四元式的第一项： 运算符号

		curToken = SEM.top();  // 取出栈顶元素
		temp_token = curToken;

		//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    删除 [ const ]
			curToken = SEM.top();  // 取 identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
			if (!SEM.empty())
				SEM.pop(); //  删除 identifier of array
		}
		else
		{
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) &&curToken.tokenType != ArrayElem_TYPE)
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord;  // 四元式第二元
			if (!SEM.empty())
				SEM.pop(); //  删除第二元
			if (tip == 1 || tip == 2)          // 变量有后缀的自增或自减操作，取出暂存
			{
				vaAddSub = curToken;  //  暂存
			}
		}

		curToken = SEM.top(); // 取出四元式第三元
		temp_token = curToken;

		//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    删除 [ const ]
			curToken = SEM.top();  // 取 identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
			if (!SEM.empty())
				SEM.pop(); //  删除 identifier of array
		}
		else
		{
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[2] = curToken.tokenWord;  // 四元式第三元
			if (!SEM.empty())
				SEM.pop(); //  删除第三元
		}

		if (error == 1)
			return;

		tempVa = "t" + to_string(num++); // 临时变量

		QUAT[3] = tempVa;    //   四元式的第四项： 临时变量
		temp_token.tokenWord = tempVa;
		temp_token.tokenValue = 0;
		temp_token.tokenType = TEMP_TYPE;

		SEM.push(temp_token);  //  临时变量入栈
		cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;// 输出四则运算的四元式
		q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
		FCtemp.nToq(q_temp);
        if (tip == 1 || tip == 2)
			{
				vaAddSub.tokenValue = 0;
				temp_token.tokenType = TEMP_TYPE;
				tempVa = "t" + to_string(num++);   //运算后 输出自增自减操作的四元式
				if (tip == 1)
				{
					cout << "( " << setw(7) << "++" << setw(7) << vaAddSub.tokenWord << setw(7) << "_" << setw(3) << tempVa << " )" << endl;
					q_temp[0] = "++"; q_temp[1] = vaAddSub.tokenWord; q_temp[2] = "_"; q_temp[3] = tempVa;
					FCtemp.nToq(q_temp);
				}
				{
					cout << "( " << setw(7) << "--" << setw(7) << vaAddSub.tokenWord << setw(7) << "_" << setw(3) << tempVa << " )" << endl;
					q_temp[0] = "--"; q_temp[1] = vaAddSub.tokenWord; q_temp[2] = "_"; q_temp[3] = tempVa;
					FCtemp.nToq(q_temp);
				}
				tip = 0;
			}
		}
	

	if (Operator == "before++")
	{
		QUAT[0] = "++";   //四元式的第一项： 操作符号

		curToken = SEM.top();
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}

		QUAT[1] = curToken.tokenWord;
		temp_token = curToken;
		if (!SEM.empty())
			SEM.pop();

		QUAT[2] = "_";

		if (error == 1)
			return;

		tempVa = "t" + to_string(num++);

		temp_token.tokenWord = tempVa;
		temp_token.tokenType = TEMP_TYPE;
		QUAT[3] = tempVa;

		SEM.push(temp_token);

		cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << QUAT[2] << setw(3) << QUAT[3] << " )" << endl;
		FCtemp.nToq(QUAT);
	}


	if (Operator == "before--")
	{
		QUAT[0] = "--";
		curToken = SEM.top();

		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}

		QUAT[1] = curToken.tokenWord;
		temp_token = curToken;
		if (!SEM.empty())
			SEM.pop();

		QUAT[2] = "_";

		if (error == 1)
			return;

		tempVa = "t" + to_string(num++);

		QUAT[3] = tempVa;
		temp_token.tokenWord = tempVa;
		temp_token.tokenType = TEMP_TYPE;

		SEM.push(temp_token);
		cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << QUAT[2] << setw(3) << QUAT[3] << " )" << endl;
		FCtemp.nToq(QUAT);
	}
}

void GT()    // great than
{
	int error = 0;
	/*if (end_program == 1)
	return;
	*/
	QUAT[0] = ">";

	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}

	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}
void LT()    // less than
{
	int error = 0;
	/*if (end_program == 1)
	return;
	*/
	QUAT[0] = "<";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}

	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}

void GE() // great equal
{
	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = ">=";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}



	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}
void LE() // less equal
{
	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = "<=";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}


	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}
void EQ()  // equal
{
	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = "==";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}

	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}
void NE()   // not equal
{
	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = "!=";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}


	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}

void OR() // logic  or
{
	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = "||";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}

	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
		FCtemp.nToq(q_temp);
}
void AND() // logic  and 
{

	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = "&&";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	curToken = SEM.top(); // 取出四元式第三元
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第三元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // 四元式第三元
		if (!SEM.empty())
			SEM.pop(); //  删除第三元
	}


	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}

void NOT()   // logic  not
{
	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = "!";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}

void MINUS()
{
	int error = 0;
	QUAT[0] = "-";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}
	if (error == 1)
		return;

	tempVa = "t" + to_string(num++);

	QUAT[3] = tempVa;
	temp_token.tokenWord = tempVa;
	temp_token.tokenValue = 0;
	temp_token.tokenType = TEMP_TYPE;

	SEM.push(temp_token);
	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << QUAT[3] << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] = QUAT[3];
	FCtemp.nToq(q_temp);
}

void ASSI(string sign1)
{
	/*if (end_program == 1)
	return;
	*/
	int error = 0;
	QUAT[0] = sign1;

	while (countVa >0)
	{
		curToken = SEM.top();  // 取出栈顶元素
		temp_token = curToken;

		//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    删除 [ const ]
			curToken = SEM.top();  // 取 identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
			if (!SEM.empty())
				SEM.pop(); //  删除 identifier of array
		}
		else
		{
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord;  // 四元式第二元
			if (!SEM.empty())
				SEM.pop(); //  删除第二元
		}


		curToken = SEM.top();  // 次栈顶变量
		temp_token = curToken;
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    删除 [ const ]
			curToken = SEM.top();  // 取 identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
			SEM.push(temp_token);
		
		}
		else
		{
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[2] = curToken.tokenWord;  // 四元式第二元

		}
		countVa--;
		if (error == 1)
			return;
		cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << QUAT[2] << " )" << endl;
		q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] ="_"; q_temp[3] = QUAT[2];
		FCtemp.nToq(q_temp);
	}
	countVa = 0;
}

void RETURN() // return 四元式函数
{
	int error = 0;
	QUAT[0] = "return";
	curToken = SEM.top();

	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] ="_";
	FCtemp.nToq(q_temp);
	//end_program = 1;
}

void IF()  // 条件语句 if 的四元式
{
	int error = 0;
	QUAT[0] = "if";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void EL() // 条件语句else 的四元式
{
	QUAT[0] = "el";
	cout << "( " << setw(7) << QUAT[0] << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] ="_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void IE() // 条件语句else 的四元式
{
	// 无条件出口语句
	cout << "( " << setw(7) << "ie" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "ie"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void WH()
{
	cout << "( " << setw(7) << "wh" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "wh"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void DO()
{
	int error = 0;
	QUAT[0] = "do";
	curToken = SEM.top();  // 取出栈顶元素
	temp_token = curToken;

	//------------------- 处理变量是数组元素的情况， 四元式第二元 -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token 可改为 curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    删除 [ const ]
		curToken = SEM.top();  // 取 identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除 identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord;  // 四元式第二元
		if (!SEM.empty())
			SEM.pop(); //  删除第二元
	}

	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void WE()     //  while尾（兼循环转向 E ）四元式
{
	cout << "( " << setw(7) << "we" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "we"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void BREAK()  // while 语句中 break 语句四元式
{
	cout << "( " << setw(7) << "break" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "break"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void CONTINUE() //  while 语句中 continue 语句四元式
{
	cout << "( " << setw(7) << "continue" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "continue"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

