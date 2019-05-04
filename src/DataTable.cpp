#include "DataTable.h"

#include <sstream>
#include <assert.h>
//#define NAME first
//#define VALUE second

DataTable::DataTable(const std::string& table_name, std::vector< std::pair<std::string, int> >& attribute_table,
  const std::string& primary_key, const std::vector<std::string>& not_null_key)
  : table_name_(table_name), primary_key_(primary_key), sequential_attribute_table_(attribute_table)
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

bool DataTable::CheckPrimaryKey(const std::vector<ATTRIBUTE>& attributes)
{
	Value* primary_key_value;
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		if (iter->NAME == primary_key_)
		{
			primary_key_value = iter->VALUE;
		}
	}
	for (auto iter = mData_.begin(); iter != mData_.end(); iter++)
	{
		if (*(*iter)->getValue(primary_key_) == *primary_key_value)
			return false;
	}
	return true;
}

bool DataTable::CheckPrimaryKey(const ATTRIBUTE& attribute)
{
	for (auto iter = mData_.begin(); iter != mData_.end(); iter++)
	{
		if (*(*iter)->getValue(primary_key_) == *attribute.VALUE)
			return false;
	}
	return true;
}

bool DataTable::CheckNotNullKey(const std::vector<ATTRIBUTE>& attributes)
{
	std::map<std::string, bool> check_not_null;
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		check_not_null[iter->NAME] = true;
	}
	for (auto iter = not_null_key_.begin(); iter != not_null_key_.end(); iter++)
	{
		if (iter->second && !check_not_null[iter->first])
		{
			return false;
		}
	}
	return true;
}

bool DataTable::CheckAttributeName(const std::vector<ATTRIBUTE>& attributes)
{
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		if (!CheckAttributeName(iter->NAME))
		{
			return false;
		}
	}
	return true;
} 

bool DataTable::CheckAttributeName(const ATTRIBUTE& attribute)
{
	if (!CheckAttributeName(attribute.NAME))
		return false;
	return true;
}

bool DataTable::CheckAttributeName(const std::string& attribute_name)
{
	if (attribute_name != "*" && attribute_table_.find(attribute_name) == attribute_table_.end())
		return false;
	return true;
}

void DataTable::SortData()
{
	using namespace std;
	static vector<Data*> DataList;
	vector<Data*>().swap(DataList);
	for (auto v: mData_)
		DataList.push_back(v);
	mData_.clear();

	sort(DataList.begin(), DataList.end(), DataCompare(primary_key_));

	for (auto v: DataList)
		mData_.push_back(v);
}

void DataTable::Insert(const std::vector< ATTRIBUTE > &attributes)
{
	Data* data = new Data;
	/*if (!CheckAttributeName(attributes) || !CheckPrimaryKey(attributes) || !CheckNotNullKey(attributes))
	{
		std::cerr << "Failed to insert. Please check your input. "<< std::endl;
	}*/
	if (!CheckAttributeName(attributes))
	{
		throw (kERROR_ATTRIBUTE_NOT_EXIST);
	}
	else if (!CheckPrimaryKey(attributes))
	{
		throw (kERROR_PRIMARY_KEY_REPEATED);
	}
	else if (!CheckNotNullKey(attributes))
	{
		throw (kERROR_NOT_NULL_KEY_NULL);
	}
	else
	{
		for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
		{
			//Value* pt = &const_cast<Value>(iter->VALUE); // 可能有bug！
			data->setValue(iter->NAME, iter->VALUE);
		}
		auto insert_position = mData_.end();
		for (auto iter = mData_.begin(); iter != mData_.end(); iter ++)
		{
			Data* current_data = *iter;
			if (*GetPrimaryKey(data) < *GetPrimaryKey(current_data))
			{
				insert_position = iter;
				break;
			}
		}
		mData_.insert(insert_position, data);
	}
}

void DataTable::Remove(std::vector<Data*> &data_list)
{
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		delete *iter;
		mData_.remove(*iter);
	}
}

void DataTable::Update(const ATTRIBUTE &attribute, std::vector<Data*> &data_list)
{
	if (!CheckAttributeName(attribute))
	{
		throw (kERROR_ATTRIBUTE_NOT_EXIST);
		return;
	}

	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		(*iter)->setValue(attribute.NAME, attribute.VALUE);
	}
	this->SortData();

	if ((attribute.NAME == primary_key_ && !CheckPrimaryKey(attribute))) //if update the primary key
	{
		throw (kERROR_PRIMARY_KEY_REPEATED);
		return;
	}
}

void DataTable::Select(const std::string &attribute_name, const std::vector<Data*> &data_list, std::vector<Value*> &attribute_value)
{
	attribute_value.clear();
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		Value* temp = (*iter)->getValue(attribute_name);
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
	for (auto _attr: sequential_attribute_table_)
	{
		string _attrName = _attr.first;
		int _attrType = _attr.second;
		cout << _attrName;
		if (attrTypeWidth.at(_attrType) != 0)
			cout << "\t" << attrTypeInvMap.at(_attrType) << "(" << attrTypeWidth.at(_attrType) << ")";
		else
			cout << "\t" << attrTypeInvMap.at(_attrType);
		if (not_null_key_[_attrName])
			cout << "\t" << "NO";
		else
			cout << "\t" << "YES";
		if (primary_key_ == _attrName)
			cout << "\t" << "PRI" << "\t" << "NULL";
		else
			cout << "\t" << "\t" << "NULL";
		cout << endl;
	}
}

Value* DataTable::transValue(const Data*_attr, std::string val, int _dataType)
{
	Value *res = NULL;
	if (attribute_table_.count(val))
		return _attr->getValue(val);
	else
		switch (_dataType)
		{
			case INT: res = new AttributeValue<int>(stralgo::str2int(val)); break;
			case DOUBLE: res = new AttributeValue<double>(stralgo::str2double(val)); break;
			case STRING: res = new AttributeValue<std::string>(val); break;
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

	if (pt_l == NULL || pt_r == NULL)
		return false;

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

	while (!ss.eof())
	{
		vector<string> param;
		string tmp;
		int _opr = -1;
		while (!ss.eof())
		{
			ss >> tmp;
			if (is_logic_oprt(tmp))
			{
				tmp = upperized(tmp);
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

Value* DataTable::GetPrimaryKey(const Data* data)
{
	return data->getValue(primary_key_);
}

std::vector< std::pair<std::string, int> > DataTable::GetAttributeTable()
{
	return sequential_attribute_table_;
}