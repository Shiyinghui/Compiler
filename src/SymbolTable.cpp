#include <iostream>
#include "SymbolTable.h"

using std::cout;
using std::endl;



SymbolTable::SymbolTable()
{
	this->synbl_all.reserve(20);

	this->typel.reserve(20);
	this->ainfl.reserve(20);
	this->rinfl.reserve(20);

	this->consl.reserve(20);
	this->lenl.reserve(20);
	this->vall.reserve(20);

	this->synbl_all.push_back(map <string, SYNBL>());//初始化全局符号表
	this->vall.push_back(0);
	// int_/*5*/, char_/*6*/, float_/*7*/, double_/*8*/,short_/*9*/, long_/*10*/, signed_/*11*/, unsigned_/*12*/,
	//顺序固定
	this->typel.push_back(TYPEL(int_, sizeof(int)));
	this->typel.push_back(TYPEL(char_, sizeof(char)));
	this->typel.push_back(TYPEL(float_, sizeof(float)));
	this->typel.push_back(TYPEL(double_, sizeof(double)));
	this->typel.push_back(TYPEL(short_, sizeof(short)));
	this->typel.push_back(TYPEL(long_, sizeof(long)));
	this->typel.push_back(TYPEL(signed_, sizeof(signed)));
	this->typel.push_back(TYPEL(unsigned_, sizeof(unsigned)));
}


SymbolTable::~SymbolTable()
{
}

bool SymbolTable::beginDefFunction(token& tk)
{
	if (this->vall.back() == 0)
	{
		this->vall.push_back(
			this->synbl_all.at(0).at(tk.tokenWord).index_ADDR = this->synbl_all.size()
		);
		this->synbl_all.push_back(map<string, SYNBL>());
		return true;
	}
	else
		return false;
}

bool SymbolTable::endDefFunction(token & tk)
{
	if (this->vall.back() == this->synbl_all.at(0).at(tk.tokenWord).index_ADDR)
	{
		this->synbl_all.at(0).at(tk.tokenWord).cat = FUN_;
		this->vall.pop_back();
		return true;
	}
	return false;
}

bool SymbolTable::beginNewProcess()
{
	this->vall.push_back(
		this->synbl_all.size()
	);
	this->synbl_all.push_back(map<string, SYNBL>());
	return true;
}

bool SymbolTable::endLastProcess()
{
	vall.pop_back();
	return false;
}

bool SymbolTable::AddToken(token & tk)
{
	if (tk.tokenType == IDentifier || tk.tokenWord == "main")////////////////////////////////////////main关键字？？？？周三23点05分
	{
		int index_synbl = this->vall.back();
		std::pair< std::map< string, SYNBL >::iterator, bool > ret;
		ret = this->synbl_all[index_synbl].insert(std::pair<string, SYNBL>(tk.tokenWord, SYNBL(tk.tokenWord)));
		tk.tokenIndex = index_synbl;
		return ret.second;
	}
	else
	{
		return false;
	}
}

bool SymbolTable::locateToken(token & tk)
{
	if (tk.tokenType == TEMP_TYPE)
	{
		return true;
	}

	vector <int>::iterator it_vall_vector = this->vall.end();
	do
	{
		it_vall_vector--;
		if (this->synbl_all.at(*it_vall_vector).end() != this->synbl_all.at(*it_vall_vector).find(tk.tokenWord))
		{
			tk.tokenIndex = *it_vall_vector;
			return true;
		}
	} while (it_vall_vector > this->vall.begin());
	tk.tokenIndex = -1;
	return false;
}

bool SymbolTable::setCAT(token & tk, emCAT cat)
{
	this->synbl_all.at(this->vall.back()).at(tk.tokenWord).setCAT(cat);
	return true;
}

bool SymbolTable::setTYPEL(token & tk, emTVAL tval)
{
	if (tk.tokenType == IDentifier||tk.tokenWord=="main")
	{
		if (tval >= int_&&tval <= unsigned_)
		{
			this->synbl_all.at(this->vall.back()).at(tk.tokenWord).index_TYPEL = tval - int_;
		}
		else
		{
			this->synbl_all.at(this->vall.back()).at(tk.tokenWord).index_TYPEL = this->typel.size();
			this->typel.push_back(TYPEL(tval, 0));
		}
		if (tk.tokenWord == "main")return true;
		this->synbl_all.at(this->vall.back()).at(tk.tokenWord).index_ADDR = this->var_addr;
		this->var_addr += getTvalSize(tval);
		return true;
	}
	else
	{
		return false;
	}
}

int SymbolTable::getTYPEL(token & tk)
{
	return this->typel.at(this->synbl_all.at(tk.tokenIndex).at(tk.tokenWord).index_TYPEL).tval;
}

int SymbolTable::getTvalSize(emTVAL tval)
{
	if (tval <= unsigned_)
	{
		return this->typel.at(tval - int_).index_TPOINT;
	}
	else if (tval == ARRAY_)
	{
		return this->ainfl.at(this->typel.at(tval).index_TPOINT).CLEN;
	}
	return 0;
}

int SymbolTable::getTvalSize(int index_TYPEL)
{
	if (index_TYPEL <= unsigned_ - int_)
	{
		return this->typel.at(index_TYPEL).index_TPOINT;
	}
	else if (this->typel.at(index_TYPEL).tval == ARRAY_)
	{
		return this->ainfl.at(this->typel.at(index_TYPEL).index_TPOINT).CLEN;
	}
	return 0;
}

bool SymbolTable::addAINFL_d(token & tk, emTVAL baseTVAL, int num_dim, int * up_dim)
{

	int ctp = baseTVAL - int_;
	int clen = this->getTvalSize(baseTVAL);
	while (num_dim>0)
	{
		this->typel.push_back(TYPEL(ARRAY_, this->ainfl.size()));
		this->ainfl.push_back(AINFL(up_dim[num_dim - 1], ctp, clen));
		clen = up_dim[num_dim - 1] * clen;
		ctp = this->typel.size() - 1;
		num_dim--;
	}
	this->synbl_all.at(this->vall.back()).at(tk.tokenWord).index_TYPEL = ctp;
	this->synbl_all.at(this->vall.back()).at(tk.tokenWord).index_ADDR = this->var_addr;
	this->var_addr += getTvalSize(this->synbl_all.at(this->vall.back()).at(tk.tokenWord).index_TYPEL);
	return true;
}

void SymbolTable::show()
{
	cout << setw(4) << "TYPEL表" << endl;
	cout << setw(4) << "序号\t类型\t大小\n";
	for (vector<TYPEL>::iterator it_TYPEL = this->typel.begin(); it_TYPEL < this->typel.end(); it_TYPEL++)
	{
		cout << setw(4) << it_TYPEL - this->typel.begin() << "\t";
		switch (it_TYPEL->tval)
		{
			//enum emTVAL {/*ARRAY_*/ STRUCT_=23, void_=4/*4*/, int_/*5*/, char_/*6*/, float_/*7*/, double_/*8*/,short_/*9*/, long_/*10*/, signed_/*11*/, unsigned_/*12*/

		case int_:cout << setw(11) << "int"; break;
		case char_:cout << setw(11) << "char"; break;
		case float_:cout << setw(11) << "float"; break;
		case double_:cout << setw(11) << "double"; break;
		case short_:cout << setw(11) << "short"; break;
		case long_:cout << setw(11) << "long"; break;
		case signed_:cout << setw(11) << "signed"; break;
		case unsigned_:cout << setw(11) << "unsigned"; break;
		case ARRAY_:cout << setw(11) << "array"; break;
		default:
			cout << setw(11) << it_TYPEL->tval;
			break;
		}

		cout << setw(4) << this->getTvalSize(it_TYPEL - this->typel.begin()) << endl;
	}

	int i = 0;
	cout << setw(4) << endl << "全局表([0])：" << endl;
	cout << setw(4) << endl << "name\tindex_TYPEL\tcat\taddr" << endl;
	for (map<string, SYNBL>::iterator it_SYNBL_map = synbl_all.at(i).begin(); it_SYNBL_map != synbl_all.at(i).end(); it_SYNBL_map++)
	{
		cout << setw(4) << it_SYNBL_map->second.name << "\t" << it_SYNBL_map->second.index_TYPEL << "\t\t";
		switch (it_SYNBL_map->second.cat)
		{
			//enum emCAT { UNKNOW = -1, FUN_, VAR_, CONST_ };
		case FUN_:cout << setw(4) << "FUN"; break;
		case VAR_:cout << setw(4) << "VAR"; break;
		case CONST_:cout << setw(4) << "CONST"; break;
		case vARRAY_:cout << setw(4) << "ARRAY_"; break;
		default:
			break;
		}
		cout << setw(4) << "\t" << it_SYNBL_map->second.index_ADDR << endl;
	}
	i++;
	while (i<int(this->synbl_all.size()))
	{
		cout << setw(4) << endl << "函数/过程符号表([" << i << "])：" << endl;
		for (map<string, SYNBL>::iterator it_SYNBL_map = synbl_all.at(i).begin(); it_SYNBL_map != synbl_all.at(i).end(); it_SYNBL_map++)
		{
			cout << setw(4) << it_SYNBL_map->second.name << "\t" << it_SYNBL_map->second.index_TYPEL << "\t\t";
			switch (it_SYNBL_map->second.cat)
			{
				//enum emCAT { UNKNOW = -1, FUN_, VAR_, CONST_ };
			case FUN_:cout << setw(4) << "FUN"; break;
			case VAR_:cout << setw(4) << "VAR"; break;
			case CONST_:cout << setw(4) << "CONST"; break;
			case vARRAY_:cout << setw(4) << "ARRAY_"; break;
			default:
				break;
			}
			cout << setw(4) << "\t" << it_SYNBL_map->second.index_ADDR << endl;
		}
		i++;
	}
	cout << "数组表" << endl;
	cout << "序号\t上界\t单元类型(TYPEL)\t单元长度\n";
	for (vector<AINFL>::iterator it_anifl = this->ainfl.begin(); it_anifl < this->ainfl.end(); ++it_anifl)
	{
		cout << it_anifl - this->ainfl.begin() << "\t" << it_anifl->up << "\t";
		switch (it_anifl->CTP + int_)
		{
		case int_:cout << setw(11) << "int"; break;
		case char_:cout << setw(11) << "char"; break;
		case float_:cout << setw(11) << "float"; break;
		case double_:cout << setw(11) << "double"; break;
		case short_:cout << setw(11) << "short"; break;
		case long_:cout << setw(11) << "long"; break;
		case signed_:cout << setw(11) << "signed"; break;
		case unsigned_:cout << setw(11) << "unsigned"; break;
		default:cout << setw(11) << it_anifl->CTP; break;
		}
		cout << "\t" << it_anifl->CLEN << endl;
	}
}
