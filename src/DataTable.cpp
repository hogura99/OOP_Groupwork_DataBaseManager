#include "DataTable.h"

#include <sstream>
#include <assert.h>
#define NAME first
#define VALUE second

/*
	未完成任务：
		checkLegality的封装问题
		notNull的检查
		update的合法性检查
		若干处红标未解决
*/

DataTable::DataTable(const std::string& table_name, std::vector< std::pair<std::string, int> >& attribute_table,
  const std::string& primary_key, const std::vector<std::string>& not_null_key)
  : table_name_(table_name), primary_key_(primary_key)
{
	for (auto iter = attribute_table.begin(); iter != attribute_table.end(); iter++)
	{
		not_null_key_[iter->first] = false;
		attribute_table_[iter->first] = iter->second;

	}
	for (auto iter = not_null_key.begin(); iter != not_null_key.end(); iter++)
	{
		not_null_key_[*iter] = true;
	}
}

DataTable::~DataTable()
{
	for (auto v: mData_)
		delete v;
}

//Incompleted
bool checkLegality(DataTable _DataTable, const std::vector<ATTRIBUTE>& attributes) // This place will cause a segmentation fault, why ??? Guess that destruction function cause it.
{
	// not check temporarily

	bool flag = true;   //函数返回值

	//检查主键
	const Value* primaryKeyValue;
	std::map<std::string, bool> checkNotNull;
	for (auto iter = attributes.begin(); iter < attributes.end(); iter++)
	{
		if (iter->NAME == _DataTable.primary_key_)
			primaryKeyValue = iter->VALUE;
	}
	bool _exist = false;
	for (auto iter = _DataTable.mData_.begin(); iter != _DataTable.mData_.end(); iter++)
	{
		if ((*iter)->getData(_DataTable.primary_key_) == primaryKeyValue)
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
		switch (_DataTable.attribute_table_[iter->NAME])
		{
			case _DataTable.INT:
				if (typeid(iter->VALUE) != typeid(dataInt))  //基类不等于派生类
				{
					std::cerr << "The type of \"" << iter->NAME << "\" should be \"int\"." << std::endl;
					flag = false;
				}
				break;
			case _DataTable.DOUBLE:
				if (typeid(iter->VALUE) != typeid(dataDouble))
				{
					std::cerr << "The type of \"" << iter->NAME << "\" should be \"double\"." << std::endl;
					flag = false;
				}
				break;
			case _DataTable.STRING:
				if (typeid(iter->VALUE) != typeid(dataString))
				{
					std::cerr << "The type of \"" << iter->NAME << "\" should be \"string\"." << std::endl;
					flag = false;
				}
				break;
		}
		*/
	   checkNotNull[iter->NAME] = true;
	}
	for (auto iter = _DataTable.not_null_key_.begin(); iter != _DataTable.not_null_key_.end(); iter++)
	{
		if ( iter->second && !checkNotNull[iter->first])
		{
			flag = false;
			std::cerr << iter->first << "can not be Null." << std::endl;
		}
	}
	return flag;
}

void DataTable::Insert(const std::vector< ATTRIBUTE > &attributes)
{
	Data* data = new Data;
	if (0 /* this place will cuase a segmentation fault, why ??? */ && checkLegality(*this, attributes))
	{
		std::cerr << "Failed to insert. Please check your input. "<< std::endl;
	}
	else
	{
		for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
		{
			//Value* pt = &const_cast<Value>(iter->VALUE); // 可能有bug！
			data->setData(iter->NAME, iter->VALUE);
		}
		mData_.insert(mData_.end(), data);
	}
}

void DataTable::Remove(std::vector<Data*> &data_list)
{
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)	//似乎有问题
	{
		delete *iter;
		mData_.remove(*iter);
	}
}

void DataTable::Update(const ATTRIBUTE &attribute, std::vector<Data*> &data_list)
{
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		(*iter)->setData(attribute.NAME, attribute.VALUE);
	}
}

void DataTable::Select(const std::string &attribute_name, const std::vector<Data*> &data_list, std::vector<Value*> &attribute_value)
{
	attribute_value.clear();
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		Value* temp = (*iter)->getData(attribute_name);
		attribute_value.insert(attribute_value.end(), temp);
	}
}

void DataTable::PrintAttributeTable()
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
	for (auto _attr: attribute_table_)
	{
		string _attrName = _attr.first;
		int _attrType = _attr.second;
		cout << _attrName;
		cout << "\t" << attrTypeInvMap.at(_attrType) << "(" << _attrType << ")";
		if (not_null_key_[_attrName])
			cout << "\t" << "NO";
		else
			cout << "\t" << "YES";
		if (primary_key_ == _attrName)
			cout << "\t" << "PRI" << "\t" << "NULL";
		else
			cout << "\t" << "NULL";
		cout << endl;
	}
}

Value* DataTable::transValue(const Data*_attr, std::string val, int _dataType)
{
	Value *res = NULL;
	if (attribute_table_.count(val))
		return _attr->getData(val);
	else
		switch (_dataType)
		{
			case INT: res = new dataInt(stralgo::str2int(val)); break;
			case DOUBLE: res = new dataInt(stralgo::str2double(val)); break;
			case STRING: res = new dataString(val); break;
			default: res = NULL; break;
		}
	return res;
}

bool DataTable::checkSingleClause(const Data* attr, const std::vector<std::string> &param)
{
	// 目前只解决param.size()==3的情况,即Attrbute?=value，并且未对参数进行检查
	Value *pt_l = NULL, *pt_r = NULL;
	int dataType = -1;
	if (attribute_table_.count(param[0]))
		dataType = attribute_table_[param[0]];
	if (attribute_table_.count(param[2]))
		dataType = attribute_table_[param[2]];
	pt_l = transValue(attr, param[0], dataType);
	pt_r = transValue(attr, param[2], dataType);

	Value &val_l = *pt_l;
	Value &val_r = *pt_r;

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

void DataTable::GetDataWhere(const std::string &clause, std::vector<Data*> &data_list)
{
	data_list.clear();
	for (auto it : mData_)
	{
		if (calcExpr(it, clause))
			data_list.push_back(it);
	}
}

int DataTable::GetTypeof(const std::string &attrName)
{
	return attribute_table_[attrName];
}

std::map<std::string, int>& DataTable::GetAttributeTable()
{
	return attribute_table_;
}