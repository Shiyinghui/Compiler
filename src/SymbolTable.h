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
	bool beginDefFunction(token& tk);//开始定义一个名为tk的函数
	bool endDefFunction(token& tk);//结束定义函数tk
	bool beginNewProcess();//开始一个新的{过程}
	bool endLastProcess();//结束上一个{过程}
	bool AddToken(token& tk);//新增标识符tk
	bool locateToken(token& tk);//查找tk是否曾定义
	bool setCAT(token& tk, emCAT cat);//设置tk的cat值  enum emCAT { UNKNOW = -1,FUN_, VAR_, CONST_ };

	bool setTYPEL(token& tk, emTVAL tval);//设置tk的tval值/*不可为ARRAY*/ enum emTVAL { STRUCT_=23, void_=4/*4*/, int_/*5*/, char_/*6*/, float_/*7*/, double_/*8*/,short_/*9*/, long_/*10*/};

	int getTYPEL(token& tk);//获得tk的tval值 enum emTVAL {/*ARRAY_*/ STRUCT_=23, void_=4/*4*/, int_/*5*/, char_/*6*/, float_/*7*/, double_/*8*/,short_/*9*/, long_/*10*/};

	int getTvalSize(emTVAL tval);

	int getTvalSize(int index_TYPEL);

	bool addAINFL_d(token& tk, emTVAL baseTVAL, int num_dim, int *up_dim);//设置一个已存在于SYNABL中的tk为 “bsdrTVAL[up_dim[0]][up_dim[1]]..[up_dim[num_dim-1]]”

	void show();

private:
	//map<string, PFINFL> pfinfl;	//函数表       

	vector<map <string, SYNBL>> synbl_all;//所有符号表 ，synbl_all[0]全局表
										  //map <string, SYNBL> synbl;//符号表总表
	vector<TYPEL> typel;	//类型表
	vector<AINFL> ainfl;	//数组表
	vector<RINFL> rinfl;	//结构表

	vector<CONSL> consl;	//常量表
	vector<LENL>  lenl;		//长度表
							//stack <map <string, SYNBL>> process;  //过程栈
	vector <int> vall;		//活动记录,存储 synbl_all 的index
	int var_addr = 0;
};