#pragma once
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <iostream>
#include "LexiAnalysis.h"
#include < stdarg.h > 
#include <iomanip>
using std::iterator;
using std::vector;
using std::stack;
using std::string;
using std::cout;
using std::endl;
using std::map;
const int IDentifier = IDENT_TYPE;
enum emTVAL {
	ARRAY_, STRUCT_ = 23, void_ = 4/*4*/, int_/*5*/, char_/*6*/, float_/*7*/, double_/*8*/,
	short_/*9*/, long_/*10*/, signed_/*11*/, unsigned_/*12*/,
};
enum emCAT { UNKNOW = -1, FUN_, VAR_, vARRAY_,CONST_ };

struct CONSL
{
	vector<int> data;
};
struct LENL
{
	vector<int> data;
};

struct TYPEL
{
	TYPEL(emTVAL tval, int index_TPOINT) { this->tval = tval; this->index_TPOINT = index_TPOINT; }
	emTVAL tval;
	int index_TPOINT;
};

struct AINFL
{
	AINFL(int up, int ctp, int clen)
	{
		this->up = up;
		this->CTP = ctp;
		this->CLEN = clen;
	}
	int up;
	int CTP;
	int CLEN;
};
struct RINFL
{
};

struct SYNBL
{
	SYNBL(string str) { this->name = str; }
	bool setCAT(emCAT cat) { this->cat = cat; return true; }

	string name = "name";
	int index_TYPEL = -1;
	emCAT cat = UNKNOW;
	int index_ADDR = -1;
};

class SymbolTable
{
public:
	SymbolTable();
	virtual ~SymbolTable();
	bool beginDefFunction(token& tk);//��ʼ����һ����Ϊtk�ĺ���
	bool endDefFunction(token& tk);//�������庯��tk
	bool beginNewProcess();//��ʼһ���µ�{����}
	bool endLastProcess();//������һ��{����}
	bool AddToken(token& tk);//������ʶ��tk
	bool locateToken(token& tk);//����tk�Ƿ�������
	bool setCAT(token& tk, emCAT cat);//����tk��catֵ  enum emCAT { UNKNOW = -1,FUN_, VAR_, CONST_ };

	bool setTYPEL(token& tk, emTVAL tval);//����tk��tvalֵ/*����ΪARRAY*/ enum emTVAL { STRUCT_=23, void_=4/*4*/, int_/*5*/, char_/*6*/, float_/*7*/, double_/*8*/,short_/*9*/, long_/*10*/};

	int getTYPEL(token& tk);//���tk��tvalֵ enum emTVAL {/*ARRAY_*/ STRUCT_=23, void_=4/*4*/, int_/*5*/, char_/*6*/, float_/*7*/, double_/*8*/,short_/*9*/, long_/*10*/};

	int getTvalSize(emTVAL tval);

	int getTvalSize(int index_TYPEL);

	bool addAINFL_d(token& tk, emTVAL baseTVAL, int num_dim, int *up_dim);//����һ���Ѵ�����SYNABL�е�tkΪ ��bsdrTVAL[up_dim[0]][up_dim[1]]..[up_dim[num_dim-1]]��

	void show();

private:
	//map<string, PFINFL> pfinfl;	//������       

	vector<map <string, SYNBL>> synbl_all;//���з��ű� ��synbl_all[0]ȫ�ֱ�
										  //map <string, SYNBL> synbl;//���ű��ܱ�
	vector<TYPEL> typel;	//���ͱ�
	vector<AINFL> ainfl;	//�����
	vector<RINFL> rinfl;	//�ṹ��

	vector<CONSL> consl;	//������
	vector<LENL>  lenl;		//���ȱ�
							//stack <map <string, SYNBL>> process;  //����ջ
	vector <int> vall;		//���¼,�洢 synbl_all ��index
	int var_addr = 0;
};