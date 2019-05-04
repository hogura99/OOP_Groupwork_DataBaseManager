#pragma once
#include <iostream>
#include <assert.h>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "ParamSpliter.h"
#include "errorstream.h"
#include "expression.h"
#include "Value.h"
#include "Data.h"


/*
 ********************************************************
 *                      definition                      *
 ********************************************************
*/


template<class Value = Value>
struct Attribute
{
	std::string name;
	Value* value;
	Attribute() { name = ""; value = NULL; }
	Attribute(std::string name, Value* value): name(name), value(value) {}
};

template<class Value = Value>
class DataTable
{
	private:
		
		//the Data in the DataTable
		std::list<Data<Value>* > mData_;

		const std::string table_name_;
		
		//the name of the primary key
		const std::string primary_key_;

		//tells whether an attribute is a not null key.
		std::map<std::string, bool> not_null_key_;

		//records the names and types of the attributes.
		std::map<std::string, int> attribute_table_;

		//record the names and types of the attributes in the same sequence as initialized.
		std::vector< std::pair<std::string, int> > sequential_attribute_table_;

		// Offer a comparison based on primary_key_.
		class DataCompare
		{
			std::string key_;
		public:
			DataCompare(std::string key): key_(key) {}
			bool operator()(const Data<Value>* a, const Data<Value>* b)
			{
				return *a->getValue(key_) < *b->getValue(key_);
			}
		};

	protected:

		// Pop the calculation stack and restore results. Basically it makes logic calculation.
		// @param val: the stack of operation values.
		// @param opr: the stack of operators.
		virtual void PopStack(std::stack<bool> &val, std::stack<int> &opr);

		// Calculate the expression. Basically it makes logic calculation.
		// @param it: the data you need to calculate with, which may exist in clause as an attribute name.
		// @param clause: the expression you need to calculate.
		virtual bool CalcExpr(const Data<Value>* it, const std::string &clause);

		// Get the result of a single logic expression.
		// @param it: the data you need to calculate with, which may exist in _param as an attribute name.
		// @param _param: the single expression you need to calculate.
		//      * _param[0]: left operation value
		//      * _param[1]: operator
		//      * _param[2]: right operator value
		virtual bool CheckSingleClause(const Data<Value>* it, const std::vector<std::string> &_param);
		
		// Transform the string value to storage Value.
		// @param _attr: you may need it to get an attribute name, if not, set it to NULL.
		// @param val: the string value to transform.
		// @param dataType: the type you need to transform value into
		virtual Value* TransValue(const Data<Value>* _attr, std::string val, int dataType);
		
		// Sort mData_.
		virtual void SortData();

	public:
		//@param table_name: the name of the table.
		//@param attribute_table: a vector containing the names and the types of the attributes. i.e. < <stu_name, STRING>, <stu_id, INT> >
		//@param primary_key: the name of the primary key.
		//@param not_null_key: the names of the not null key.
		DataTable(const std::string& table_name, std::vector< std::pair<std::string, int> >& attribute_table,
		  const std::string& primary_key, const std::vector<std::string>& not_null_key);

		//delete the DataTable and the Data inside it.
		~DataTable();

		//insert a data into the table.
		//@param attributes: names and the corresponding value of the attributes
		void Insert(const std::vector< Attribute<Value> >& attributes);

		//remove the chosen data.
		//@param data_list: the position of the data to be removed.
		void Remove(std::vector< Data<Value>* > &data_list);

		//update the chosen data's single attribute
		void Update(const Attribute<Value> &attribute, std::vector< Data<Value>* > &data_list);

		//get the value (corresponding to the name) of the chosen data.
		//@param attribute_value: the returned value.
		void Select(const std::string &attribute_name, const std::vector<Data<Value>*> &data_list, std::vector<Value*> &attribute_value);

		//get the position of the data satisfying the clause.
		//@param clause: the string after the "where".
		//@param data_list: the position of the chosen data (pointer)
		virtual void GetDataWhere(const std::string &clause, std::vector<Data<Value>*> &data_list);

		//get the type of an attribute.
		//@param attrName: the attribute you want to enquire.
		virtual int GetTypeof(const std::string &attrName);

		//Get the primary key value data.
		//@param data: the data you want to enquire.
		virtual Value* GetPrimaryKey(const Data<Value>* data);

		//Get the attribute names and their correspondent type.
		std::vector< std::pair<std::string, int> > GetAttributeTable();
		
		//Print the attribute table.
		virtual void PrintAttributeTable();

		//check whether there are more than one data with the same primary key.
		//use the copy constructor of Value. May cause bugs.
		virtual bool CheckPrimaryKey(const std::vector< Attribute<Value> >& attributes);

		virtual bool CheckPrimaryKey(const Attribute<Value> & attribute);

		//check whether there are uninitialized attribute which should be not null.
		virtual bool CheckNotNullKey(const std::vector< Attribute<Value> >& attributes);

		//check whether there are unexist attribute name in the input.
		virtual bool CheckAttributeName(const std::vector< Attribute<Value> >& attributes);

		virtual bool CheckAttributeName(const Attribute<Value>& attribute);

		virtual bool CheckAttributeName(const std::string& attribute_name);
};

/*
 ********************************************************
 *                  implementation                      *
 ********************************************************
*/

template<class Value>
DataTable<Value>::DataTable(const std::string& table_name, std::vector< std::pair<std::string, int> >& attribute_table,
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

template<class Value>
DataTable<Value>::~DataTable()
{
	for (auto v: mData_)
		delete v;
}

template<class Value>
bool DataTable<Value>::CheckPrimaryKey(const std::vector< Attribute<Value> >& attributes)
{
	Value* primary_key_value;
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		if (iter->name == primary_key_)
		{
			primary_key_value = iter->value;
		}
	}
	for (auto iter = mData_.begin(); iter != mData_.end(); iter++)
	{
		if (*(*iter)->getValue(primary_key_) == *primary_key_value)
			return false;
	}
	return true;
}

template<class Value>
bool DataTable<Value>::CheckPrimaryKey(const Attribute<Value>& attribute)
{
	for (auto iter = mData_.begin(); iter != mData_.end(); iter++)
	{
		if (*(*iter)->getValue(primary_key_) == *attribute.value)
			return false;
	}
	return true;
}

template<class Value>
bool DataTable<Value>::CheckNotNullKey(const std::vector< Attribute<Value> >& attributes)
{
	std::map<std::string, bool> check_not_null;
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		check_not_null[iter->name] = true;
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

template<class Value>
bool DataTable<Value>::CheckAttributeName(const std::vector< Attribute<Value> >& attributes)
{
	for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
	{
		if (!CheckAttributeName(iter->name))
		{
			return false;
		}
	}
	return true;
} 

template<class Value>
bool DataTable<Value>::CheckAttributeName(const Attribute<Value>& attribute)
{
	if (!CheckAttributeName(attribute.name))
		return false;
	return true;
}

template<class Value>
bool DataTable<Value>::CheckAttributeName(const std::string& attribute_name)
{
	if (attribute_name != "*" && attribute_table_.find(attribute_name) == attribute_table_.end())
		return false;
	return true;
}

template<class Value>
void DataTable<Value>::SortData()
{
	using namespace std;
	static vector<Data<Value>*> DataList;
	vector<Data<Value>*>().swap(DataList);
	for (auto v: mData_)
		DataList.push_back(v);
	mData_.clear();

	sort(DataList.begin(), DataList.end(), DataCompare(primary_key_));

	for (auto v: DataList)
		mData_.push_back(v);
}

template<class Value>
void DataTable<Value>::Insert(const std::vector< Attribute<Value> > &attributes)
{
	Data<Value>* data = new Data<Value>;
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
			//Value* pt = &const_cast<Value>(iter->value); // 可能有bug！
			data->setValue(iter->name, iter->value);
		}
		auto insert_position = mData_.end();
		for (auto iter = mData_.begin(); iter != mData_.end(); iter ++)
		{
			Data<Value>* current_data = *iter;
			if (*GetPrimaryKey(data) < *GetPrimaryKey(current_data))
			{
				insert_position = iter;
				break;
			}
		}
		mData_.insert(insert_position, data);
	}
}

template<class Value>
void DataTable<Value>::Remove(std::vector<Data<Value>*> &data_list)
{
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		delete *iter;
		mData_.remove(*iter);
	}
}

template<class Value>
void DataTable<Value>::Update(const Attribute<Value> &attribute, std::vector<Data<Value>*> &data_list)
{
	if (!CheckAttributeName(attribute))
	{
		throw (kERROR_ATTRIBUTE_NOT_EXIST);
		return;
	}

	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		(*iter)->setValue(attribute.name, attribute.value);
	}
	this->SortData();

	if ((attribute.name == primary_key_ && !CheckPrimaryKey(attribute))) //if update the primary key
	{
		throw (kERROR_PRIMARY_KEY_REPEATED);
		return;
	}
}

template<class Value>
void DataTable<Value>::Select(const std::string &attribute_name, const std::vector<Data<Value>*> &data_list, std::vector<Value*> &attribute_value)
{
	attribute_value.clear();
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++)
	{
		Value* temp = (*iter)->getValue(attribute_name);
		attribute_value.insert(attribute_value.end(), temp);
	}
}

template<class Value>
void DataTable<Value>::PrintAttributeTable()
{
	using namespace std;
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

template<class Value>
Value* DataTable<Value>::TransValue(const Data<Value>*_attr, std::string val, int _dataType)
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

template<class Value>
bool DataTable<Value>::CheckSingleClause(const Data<Value>* attr, const std::vector<std::string> &param)
{
	// 目前只解决param.size()==3的情况,即Attrbute?=value，并且未对参数进行检查
	Value *pt_l = NULL, *pt_r = NULL;
	int dataType = -1;
	if (attribute_table_.count(param[0]))
		dataType = attribute_table_[param[0]];
	if (attribute_table_.count(param[2]))
		dataType = attribute_table_[param[2]];
	pt_l = TransValue(attr, param[0], dataType);
	pt_r = TransValue(attr, param[2], dataType);

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

template<class Value>
void DataTable<Value>::PopStack(std::stack<bool> &val, std::stack<int> &opr)
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

template<class Value>
bool DataTable<Value>::CalcExpr(const Data<Value>* it, const std::string &clause)
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
		val.push(CheckSingleClause(it, param));
		while (!opr.empty() && oprORDER[opr.top()] > oprORDER[_opr])
			PopStack(val, opr);
		if (~_opr)
			opr.push(_opr);
	}

	while (!opr.empty()) // 这里可以考虑用一个函数指针来扩展逻辑运算符
		PopStack(val, opr);
	assert(!val.empty());
	return val.top();
}

template<class Value>
void DataTable<Value>::GetDataWhere(const std::string &clause, std::vector<Data<Value>*> &data_list)
{
	data_list.clear();
	for (auto it : mData_)
	{
		if (CalcExpr(it, clause))
			data_list.push_back(it);
	}
}

template<class Value>
int DataTable<Value>::GetTypeof(const std::string &attrName)
{
	return attribute_table_[attrName];
}

template<class Value>
Value* DataTable<Value>::GetPrimaryKey(const Data<Value>* data)
{
	return data->getValue(primary_key_);
}

template<class Value>
std::vector< std::pair<std::string, int> > DataTable<Value>::GetAttributeTable()
{
	return sequential_attribute_table_;
}