#include"FinalCodeCreate.h"

FinalCode::FinalCode()
{

}

FinalCode::~FinalCode()
{

}

void FinalCode::Code(q_operator q)
{
	assembly ass;
	//���ʽ��Ԫʽ
	if (q[0].word == "+" || q[0].word == "-" || q[0].word == "*" || q[0].word == "/")
	{
		if (RDL.word == "0")
		{
			ass.operator0 = "MOV";
			ass.operator1 = "AX";
			ass.operator2 = q[1].word;
			assum[i] = ass;
			i++;
			ass.operator0 = expToass(q[0].word);
			ass.operator1 = "AX";
			ass.operator2 = q[2].word;
			assum[i] = ass;
			i++;
		}
		else if (RDL.word == q[1].word)
		{
			if (q[1].flag == 1)
			{
				ass = { "ST","AX",q[1].word };
				assum[i] = ass;
				i++;
				ass = { expToass(q[0].word),"AX",q[2].word };
				assum[i] = ass;
				i++;
			}
			else
			{
				ass = { expToass(q[0].word),"AX",q[2].word };
				assum[i] = ass;
				i++;
			}
		}
		else if (RDL.word == q[2].word)
		{
			if (q[2].flag == 1)
			{
				ass = { "ST","AX",q[2].word };
				assum[i] = ass;
				i++;
				ass = { expToass(q[0].word),"AX",q[1].word };
				assum[i] = ass;
				i++;
			}
			else
			{
				ass = { expToass(q[0].word),"AX",q[1].word };
				assum[i] = ass;
				i++;
			}
		}
		else
		{
			if (RDL.flag == 1)
			{
				ass = { "ST","AX",RDL.word };
				assum[i] = ass;
				i++;
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
				ass = { expToass(q[0].word),"AX",q[2].word };
				assum[i] = ass;
				i++;
			}
			else
			{
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
				ass = { expToass(q[0].word),"AX",q[2].word };
				assum[i] = ass;
				i++;
			}
		}
		RDL = q[3];
	}

	//�Ƚ����
	else if (q[0].word == ">" || q[0].word == "<" || q[0].word == "==" || q[0].word == ">=" || q[0].word == "<=" || q[0].word == "!=")
	{
			ass.operator0 = "MOV";
			ass.operator1 = "AX";
			ass.operator2 = q[1].word;
			assum[i] = ass;
			i++;
			ass.operator0 = expToass(q[0].word);
			ass.operator1 = "AX";
			ass.operator2 = q[2].word;
			assum[i] = ass;
			i++;
	}

	//�����Լ����
	else if (q[0].word == "++" || q[0].word == "--" )
	{
		ass = { "MOV","BX",q[1].word };
		assum[i] = ass;
		i++;
		ass = { expToass(q[0].word),"AX" };
		assum[i] = ass;
		i++;
		ass = { "ST","BX",q[3].word };
		assum[i] = ass;
		i++;
	}

	//�߼��������
	else if (q[0].word=="&&"||q[0].word=="||"||q[0].word=="!")
	{
		ass = { "MOV","AX",q[1].word };
		assum[i] = ass;
		i++;
		ass = { expToass(q[0].word),"AX",q[2].word };
		assum[i] = ass;
		i++;
		ass = { "ST","AX",q[3].word };
		assum[i] = ass;
		i++;
	}


	//��ֵ���
	else if (q[0].word == "=")
	{
		if (RDL.word == "0")
		{
			ass = { "MOV","AX",q[1].word };
			assum[i] = ass;
			i++;
		}
		else if (RDL.word == q[1].word)
		{
			ass = { "ST","AX",q[1].word };
			assum[i] = ass;
			i++;
		}
		else
		{
			if (RDL.flag == 1)
			{
				ass = { "ST","AX",RDL.word };
				assum[i] = ass;
				i++;
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
			}
			else
			{
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
			}
		}
		RDL = q[3];
	}

	//�������
	else if (q[0].word == "if")
	{
		if (RDL.word == "0")
		{
			ass = { "MOV","AX",q[1].word };
			assum[i] = ass;
			i++;
			ss <<"J"<< Jt;
			ass = { "FJ","AX" ,ss.str() };
			ss.str("");
			sw.push(Jt);
			//cout << "��ջ1" <<Jt<< endl;
			Jt++;
			assum[i] = ass;
			i++;
		}
		else if (RDL.word == q[1].word)
		{
			if (q[1].flag == 1) {
				ass = { "ST","AX",q[1].word };
				assum[i] = ass;
				i++;
				ss << "J" << Jt;
				ass = { "FJ","AX" ,ss.str() };
				ss.str("");
				sw.push(Jt);
				//cout << "��ջ2" << Jt << endl;
				Jt++;
				assum[i] = ass;
				i++;
			}
			else
			{
				ss << "J" << Jt;
				ass = { "FJ","AX" ,ss.str() };
				ss.str("");
				sw.push(Jt);
				//cout << "��ջ3" << Jt << endl;
				Jt++;
				assum[i] = ass;
				i++;
			}
			RDL = { 0,"0" };
		}
		else
		{
			if (RDL.flag == 1)
			{
				ass = { "ST","AX",RDL.word };
				assum[i] = ass;
				i++;
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
				ss << "J" << Jt;
				ass = { "FJ","AX" ,ss.str() };
				ss.str("");
				sw.push(Jt);
				//cout << "��ջ4" << Jt << endl;
				Jt++;
				assum[i] = ass;
				i++;
			}
			else
			{
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
				ss << "J" << Jt;
				ass = { "FJ","AX" ,ss.str() };
				ss.str("");
				sw.push(Jt);
				//cout << "��ջ5" << Jt << endl;
				Jt++;
				assum[i] = ass;
				i++;
			}
		}
		RDL = { 0,"0" };
	}

	else if (q[0].word == "el")
	{
			//sw.push(Jt);
			ss << "J" << sw.top();
			sw.pop();
			ass = { "ST","AX",RDL.word,ss.str()+": " };
			ss.str("");
			assum[i] = ass;
			sw.push(Jt);
			i++;
	}

	else if (q[0].word == "ie")
	{
		if (RDL.flag == 1) {
			/*ss << "J" << sw.top();
			cout << sw.top() << "����" << endl;
			sw.pop();*/
			//ass = { "ST","Rie",RDL.word,ss.str() + ": " };
			ass = { "ST","AX",RDL.word };
			ss.str("");
			assum[i] = ass;
			i++;
		}
	}

	//ѭ�����
	else if (q[0].word == "wh") {
		sw.push(Jt);
		ss << "J" << Jt;
		ass = { "","","",ss.str() + ": " };
		assum[i] = ass;
		i++;
		ss.str("");
	}

	else if (q[0].word == "we")
	{
		if (RDL.flag == 1) {
			ass = { "ST","AX",RDL.word };
			assum[i] = ass;
			i++;
		}
		RDL = { 0,"0" };
		sw.pop();
		int temp = sw.top();
		//ss << "J" << sw.top();
		sw.pop();
		//ass = { "JMP","_", ss.str() };
		//ss.str("");
		//assum[i] = ass;
		//i++;
	}

	else if (q[0].word == "do")
	{
		if (RDL.word == "0") {
			ass = { "MOV","AX",q[1].word };
			assum[i] = ass;
			i++;
			ss << "J" << Jt;
			ass = { "FJ","AX",ss.str() };
			sw.push(Jt);
			//cout << "��ջ7" << Jt << endl;
			Jt++;
			ss.str("");
			assum[i] = ass;
			i++;
		}
		else if (q[0].word==q[1].word)
		{
			if (q[1].flag == 1) {
				ass = { "ST","AX",q[1].word };
				assum[i] = ass;
				i++;
				ss << "J" << Jt;
				ass = { "FJ","AX",ss.str() };
				ss.str("");
				assum[i] = ass;
				i++;
			}
			else
			{
				ss << "J" << Jt;
				ass = { "FJ","AX",ss.str() };
				ss.str("");
				assum[i] = ass;
				i++;
			}
			sw.push(Jt);
			//cout << "��ջ8" << Jt << endl;
			Jt++;
			RDL = { 0,"0" };
		}
		else if (RDL.word!=q[1].word)
		{
			if (RDL.flag == 1) {
				ass = { "ST","AX",q[1].word };
				assum[i] = ass;
				i++;
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
				ss << "J" << Jt;
				ass = { "FJ","AX",ss.str() };
				ss.str("");
				assum[i] = ass;
				i++;
			}
			else {
				ass = { "MOV","AX",q[1].word };
				assum[i] = ass;
				i++;
				ss << "J" << Jt;
				ass = { "FJ","AX",ss.str() };
				ss.str("");
				assum[i] = ass;
				i++;
			}
			sw.push(Jt);
			//cout << "��ջ9" << Jt << endl;
			Jt++;
			RDL = { 0,"0" };
		}
	}

	//�������
	else if (q[0].word == "return")
	{
		ass = { "MOV","CH","4CH" };
		assum[i] = ass;
		i++;
		ass = { "INT","21H","" };
		assum[i] = ass;
		i++;
	}

}

void FinalCode::nToq(string* n)
{
	q_operator q;
	for (int i = 0; i<4; i++) { q[i] = { 1,n[i] }; }
	Code(q);
}

string FinalCode::expToass (string s) {
	if (s == "+") { s = "ADD"; return s; }
	else if (s == "-") { s = "SUB"; return s; }
	else if (s == "*") { s = "MUL"; return s; }
	else if (s == "/") { s = "DIV"; return s; }
	else if (s == "==") { s = "ET"; return s; }
	else if (s == ">=") { s = "GET"; return s; }
	else if (s == "<=") { s = "LET"; return s; }
	else if (s == ">") { s = "GT"; return s; }
	else if (s == "<") { s = "LT"; return s; }
	else if (s == "!=") { s = "NT"; return s; }
	else if (s == "++") { s = "INC"; return s; }
	else if (s == "--") { s = "DEC"; return s; }
	else if (s == "&&") { s = "AND"; return s; }
	else if (s == "||") { s = "OR"; return s; }
	else if (s == "!") { s = "NOT"; return s; }
	else return s;
}