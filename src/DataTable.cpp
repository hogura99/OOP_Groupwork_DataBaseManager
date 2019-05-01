#include "DataTable.h"

#include <sstream>
#include <assert.h>
#define NAME first
#define BASE second

/*
	未完成任务：
		checkLegality的封装问题
		notNull的检查
		update的合法性检查
		若干处红标未解决
*/

DataTable::DataTable(const std::string &_tableName, std::vector< std::pair<std::string, int> >& _attrTable, const std::string& _primaryKey, const std::vector<std::string>& _notNullKey)
: tableName(_tableName), primaryKey(_primaryKey)
{
	for (auto iter = _attrTable.begin(); iter != _attrTable.end(); iter++)
	{
		notNullKey[iter->first] = false;
		attrTable[iter->first] = iter->second;

	}
	for (auto iter = _notNullKey.begin(); iter != _notNullKey.end(); iter++)
	{
		notNullKey[*iter] = true;
	}
}

DataTable::~DataTable()
{
	for (auto v: mData)
		delete v;
}

bool checkLegality(DataTable _DataTable, const std::vector<ATTRIBUTE>& attributes) // This place will cause a segmentation fault, why ??? Guess that destruction function cause it.
{
	// not check temporarily

	bool flag = true;   //函数返回值

	//检查主键
	const Base* primaryKeyValue;
	std::map<std::string, bool> checkNotNull;
	for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
	{
		if (iter->NAME == _DataTable.primaryKey)
			primaryKeyValue = iter->BASE;
	}
	bool _exist = false;
	for (auto iter = _DataTable.mData.begin(); iter != _DataTable.mData.end(); iter++)   //iter不能大小比较
	{
		if ((*iter)->getData(_DataTable.primaryKey) == primaryKeyValue)
			_exist = true;
	}
	if (_exist)
	{
		std::cerr << "This Primary Key already exists." << std::endl;   //输出用cerr
		flag = false;
	}

	//检查类型&非空(这里应该还要检查类型名输入是否正确的。。。好烦啊这个version不写了，用户难道自己心里没点B数🐎)
	for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
	{
		/*
		switch (_DataTable.attrTable[iter->NAME])
		{
			case _DataTable.INT:
				if (typeid(iter->BASE) != typeid(dataInt))  //基类不等于派生类
				{
					std::cerr << "The type of \"" << iter->NAME << "\" should be \"int\"." << std::endl;
					flag = false;
				}
				break;
			case _DataTable.DOUBLE:
				if (typeid(iter->BASE) != typeid(dataDouble))
				{
					std::cerr << "The type of \"" << iter->NAME << "\" should be \"double\"." << std::endl;
					flag = false;
				}
				break;
			case _DataTable.STRING:
				if (typeid(iter->BASE) != typeid(dataString))
				{
					std::cerr << "The type of \"" << iter->NAME << "\" should be \"string\"." << std::endl;
					flag = false;
				}
				break;
		}
		*/
	   checkNotNull[iter->NAME] = true;
	}
	for (auto iter = _DataTable.notNullKey.begin(); iter != _DataTable.notNullKey.end(); iter++)
	{
		if ( iter->second && !checkNotNull[iter->first])
		{
			flag = false;
			std::cerr << iter->first << "can not be Null." << std::endl;
		}
	}
	return flag;
}

void DataTable::insert(const std::vector< ATTRIBUTE > &attributes)
{
	Data* _data = new Data;
	if (0 /* this place will cuase a segmentation fault, why ??? */ && checkLegality(*this, attributes))
	{
		std::cerr << "Failed to insert. Please check your input. "<< std::endl;
	}
	else
	{
		for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
		{
			//Base* pt = &const_cast<Base>(iter->BASE); // 可能有bug！
			_data->setData(iter->NAME, iter->BASE);
		}
		mData.insert(mData.end(), _data);
		std::cerr << "Successfully inserted." << std::endl; //这里应该再输出更详细的信息的，这版就不写了
	}
}

void DataTable::remove(std::list<Data*> &dataList)
{
	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		delete *iter;
		mData.remove(*iter);
	}
}

void DataTable::update(const ATTRIBUTE &attributes, std::list<Data*> &dataList)
{
	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		(*iter)->setData(attributes.NAME, attributes.BASE);
	}
}

void DataTable::select(const std::string &attrName, std::vector<Base*> &attrList, const std::list<Data*> &dataList)
{
	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		Base* temp = (*iter)->getData(attrName);
		attrList.insert(attrList.end(), temp);
	}
}

void DataTable::printAttrTable()
{
	using namespace std;
	/*{
		cout << PRINT_TABLE_TYPE[0];
		for (int i = 1; i < PRINT_TABLE_TYPE_COUNT; i ++)
		{
			cout << PRINT_TABLE_TYPE[i] << "\t";
		}
		cout << endl;
	}*/
	cout << "Field\tType\tNull\tKey\tDefault\tExtra" << endl;
	for (auto _attr: attrTable)
	{
		string _attrName = _attr.first;
		int _attrType = _attr.second;
		cout << _attrName;
		cout << "\t" << attrTypeInvMap.at(_attrType) << "(" << _attrType << ")";
		if (notNullKey[_attrName])
			cout << "\t" << "NO";
		else
			cout << "\t" << "YES";
		if (primaryKey == _attrName)
			cout << "\t" << "PRI" << "\t" << "NULL";
		else
			cout << "\t" << "NULL";
		cout << endl;
	}
}

Base* DataTable::transValue(const Data*_attr, std::string val, int _dataType)
{
	Base *res = NULL;
	if (attrTable.count(val))
		return _attr->getData(val);
	else
		switch (_dataType)
		{
			case INT: res = new dataInt(Params::str2int(val)); break;
			case DOUBLE: res = new dataInt(Params::str2double(val)); break;
			case STRING: res = new dataString(val); break;
			default: res = NULL; break;
		}
	return res;
}

bool DataTable::checkSingleClause(const Data* attr, const std::vector<std::string> &param)
{
	// 目前只解决param.size()==3的情况,即Attrbute?=value，并且未对参数进行检查
	Base *pt_l = NULL, *pt_r = NULL;
	int dataType = -1;
	if (attrTable.count(param[0]))
		dataType = attrTable[param[0]];
	if (attrTable.count(param[2]))
		dataType = attrTable[param[2]];
	pt_l = transValue(attr, param[0], dataType);
	pt_r = transValue(attr, param[2], dataType);

	Base &val_l = *pt_l;
	Base &val_r = *pt_r;

	bool res = false;
	switch (Exprs::oprTYPE[param[1]])
	{
		case opLE:  res = val_l <  val_r; break;
		case opGI:  res = val_l >  val_r; break;
		case opEQ:  res = val_l == val_r; break;
		case opLEQ: res = val_l <= val_r; break;
		case opGIQ: res = val_l >= val_r; break;
		case opNEQ: res = val_l != val_r; break;
	}

	std::cerr << val_l << " " << param[1] << " " << val_r << " = " << res << std::endl;
	return res;
}

void DataTable::__PopStack(std::stack<bool> &val, std::stack<int> &opr)
{
	bool se = val.top(); val.pop();
	assert(!val.empty());
	bool fi = val.top(); val.pop();
	int _opr = opr.top(); opr.pop();
	switch (_opr)
	{
		case opAND: fi = fi && se; break;
		case opOR: fi = fi || se; break;
		default: break;
	}
	val.push(fi);
}

bool DataTable::calcExpr(const Data* it, const std::string &clause)
{
	using namespace std;
	if (clause.empty() || clause == "*")
		return true;

	stringstream ss(clause);
	stack<bool> val;
	stack<int> opr;

	while (!ss.eof()) // 这里可以考虑用AC自动机
	{
		vector<string> param;
		string tmp;
		int _opr = -1;
		while (!ss.eof())
		{
			ss >> tmp;
			if (IS_LOGIC_OPRT(tmp))
			{
				_opr = Exprs::oprTYPE[tmp];
				break;
			}
			else
				param.push_back(tmp);
		}
		val.push(checkSingleClause(it, param));
		while (!opr.empty() && oprORDER[opr.top()] > oprORDER[_opr])
			__PopStack(val, opr);
		if (~_opr)
			opr.push(_opr);
	}

	while (!opr.empty()) // 这里可以考虑用一个函数指针来扩展逻辑运算符
		__PopStack(val, opr);
	assert(!val.empty());
	return val.top();
}

void DataTable::getDataWhere(const std::string &clause, std::list<Data*> &dataList)
{
	dataList.clear();
	for (auto it : mData)
	{
		if (calcExpr(it, clause))
			dataList.push_back(it);
	}
}

int DataTable::getTypeof(const std::string &attrName)
{
	return attrTable[attrName];
}