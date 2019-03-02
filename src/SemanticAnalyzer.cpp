#include "SyntaxParsing.h"//////////////change
#include "LexiAnalysis.h"
#include"FinalCodeCreate.h"
#include"SemanticAnalyser.h"
#include"SymbolTable.h"
#include <iomanip>
extern int Flag;  // �ж�Դ�����﷨�Ƿ���ȷ��־
extern  vector<token>::iterator word;  // ������
extern string QUAT[4];        // ��Ԫʽ
extern token  curToken;
extern stack<token>SEM;  // ����ջ

/*  �����������ʱһЩ�������� */
extern int num;
extern int tip;               // �������Լ� ��׺ʱ���ж�
extern int countVa;     //  ������ֵ����ʱ��¼����ֵ�����ĸ���
extern SymbolTable Stb;   // ���ű�
string tempVa;  // ��ʱ����
token temp_token;  // ��ʱ����
extern FinalCode FCtemp;
string q_temp[4] = {};

void GEQ(string Operator)
{

	int error = 0;
	/*if (end_program == 1)
	return;
	*/
	token vaAddSub;        // ������׺�������Լ���������ݴ�


	if (Operator == "+" || Operator == "-" || Operator == "*" || Operator == "/")
	{
		QUAT[0] = Operator;  //      ��Ԫʽ�ĵ�һ� �������

		curToken = SEM.top();  // ȡ��ջ��Ԫ��
		temp_token = curToken;

		//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    ɾ�� [ const ]
			curToken = SEM.top();  // ȡ identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
			if (!SEM.empty())
				SEM.pop(); //  ɾ�� identifier of array
		}
		else
		{
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) &&curToken.tokenType != ArrayElem_TYPE)
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
			if (!SEM.empty())
				SEM.pop(); //  ɾ���ڶ�Ԫ
			if (tip == 1 || tip == 2)          // �����к�׺���������Լ�������ȡ���ݴ�
			{
				vaAddSub = curToken;  //  �ݴ�
			}
		}

		curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
		temp_token = curToken;

		//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    ɾ�� [ const ]
			curToken = SEM.top();  // ȡ identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
			if (!SEM.empty())
				SEM.pop(); //  ɾ�� identifier of array
		}
		else
		{
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
			if (!SEM.empty())
				SEM.pop(); //  ɾ������Ԫ
		}

		if (error == 1)
			return;

		tempVa = "t" + to_string(num++); // ��ʱ����

		QUAT[3] = tempVa;    //   ��Ԫʽ�ĵ���� ��ʱ����
		temp_token.tokenWord = tempVa;
		temp_token.tokenValue = 0;
		temp_token.tokenType = TEMP_TYPE;

		SEM.push(temp_token);  //  ��ʱ������ջ
		cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[2] << setw(7) << QUAT[1] << setw(3) << QUAT[3] << " )" << endl;// ��������������Ԫʽ
		q_temp[0] = QUAT[0]; q_temp[1] = QUAT[2]; q_temp[2] = QUAT[1]; q_temp[3] = QUAT[3];
		FCtemp.nToq(q_temp);
        if (tip == 1 || tip == 2)
			{
				vaAddSub.tokenValue = 0;
				temp_token.tokenType = TEMP_TYPE;
				tempVa = "t" + to_string(num++);   //����� ��������Լ���������Ԫʽ
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
		QUAT[0] = "++";   //��Ԫʽ�ĵ�һ� ��������

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

	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	curToken = SEM.top(); // ȡ����Ԫʽ����Ԫ
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ����Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + temp_token.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[2] = curToken.tokenWord;  // ��Ԫʽ����Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ������Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
	}
	else
	{
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
		}
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
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
		curToken = SEM.top();  // ȡ��ջ��Ԫ��
		temp_token = curToken;

		//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    ɾ�� [ const ]
			curToken = SEM.top();  // ȡ identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
			if (!SEM.empty())
				SEM.pop(); //  ɾ�� identifier of array
		}
		else
		{
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken) && curToken.tokenType != ArrayElem_TYPE)
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
			if (!SEM.empty())
				SEM.pop(); //  ɾ���ڶ�Ԫ
		}


		curToken = SEM.top();  // ��ջ������
		temp_token = curToken;
		if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
		{
			if (!SEM.empty())
				SEM.pop();   //    ɾ�� [ const ]
			curToken = SEM.top();  // ȡ identifier of array
			if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
			{
				cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
				error = 1;
				Flag = 0;
			}
			QUAT[2] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
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
			QUAT[2] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ

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

void RETURN() // return ��Ԫʽ����
{
	int error = 0;
	QUAT[0] = "return";
	curToken = SEM.top();

	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
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
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] ="_";
	FCtemp.nToq(q_temp);
	//end_program = 1;
}

void IF()  // ������� if ����Ԫʽ
{
	int error = 0;
	QUAT[0] = "if";
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
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
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void EL() // �������else ����Ԫʽ
{
	QUAT[0] = "el";
	cout << "( " << setw(7) << QUAT[0] << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] ="_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void IE() // �������else ����Ԫʽ
{
	// �������������
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
	curToken = SEM.top();  // ȡ��ջ��Ԫ��
	temp_token = curToken;

	//------------------- �������������Ԫ�ص������ ��Ԫʽ�ڶ�Ԫ -------------------
	if (temp_token.tokenValue == 0 && temp_token.tokenType == ArrayElem_TYPE)  //  temp_token �ɸ�Ϊ curToken
	{
		if (!SEM.empty())
			SEM.pop();   //    ɾ�� [ const ]
		curToken = SEM.top();  // ȡ identifier of array
		if (curToken.tokenValue == 0 && !Stb.locateToken(curToken))
		{
			cout << "error:  " + curToken.tokenWord + "  was not declared in this scope" << endl;
			error = 1;
			Flag = 0;
			return;
		}
		QUAT[1] = curToken.tokenWord + temp_token.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ�� identifier of array
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
		QUAT[1] = curToken.tokenWord;  // ��Ԫʽ�ڶ�Ԫ
		if (!SEM.empty())
			SEM.pop(); //  ɾ���ڶ�Ԫ
	}

	cout << "( " << setw(7) << QUAT[0] << setw(7) << QUAT[1] << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = QUAT[0]; q_temp[1] = QUAT[1]; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void WE()     //  whileβ����ѭ��ת�� E ����Ԫʽ
{
	cout << "( " << setw(7) << "we" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "we"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void BREAK()  // while ����� break �����Ԫʽ
{
	cout << "( " << setw(7) << "break" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "break"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

void CONTINUE() //  while ����� continue �����Ԫʽ
{
	cout << "( " << setw(7) << "continue" << setw(7) << "_" << setw(7) << "_" << setw(3) << "_" << " )" << endl;
	q_temp[0] = "continue"; q_temp[1] = "_"; q_temp[2] = "_"; q_temp[3] = "_";
	FCtemp.nToq(q_temp);
}

