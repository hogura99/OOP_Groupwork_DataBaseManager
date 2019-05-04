#pragma once

#include <assert.h>
#include <iostream>
#include <string>
#include <list>
#include <map>

#include "DataTable.h"
#include "errorstream.h"
#include "errorstream.h"

template<class DataTable = DataTable, class ParamSpliter = ParamSpliter>
class DataBase {

private:
	std::map<std::string, DataTable*> mTable;
	std::string __name;

protected:
	virtual Value* transValue(const std::string &VStr, int type) const;

	virtual void PrintSelectData(std::vector< std::pair<std::string, std::vector<Value*> > >& _attrList);

public:
	DataBase(const std::string &DBName);
	~DataBase();

	void CreateTable(const std::string &command);
	void DropTable(const std::string &tableName);
	void ShowTableCol(const std::string &tableName);
	void ShowTableAll(bool PrintTableName = true);

	void InsertData(const std::vector<std::string> &param);
	void DeleteData(const std::vector<std::string> &param);
	void UpdateData(const std::vector<std::string> &param);
	void SelectData(const std::vector<std::string> &param);
};

template<class DataTable, class ParamSpliter>
DataBase<DataTable, ParamSpliter>::DataBase(const std::string &DBName): __name(DBName)
{

}

template<class DataTable, class ParamSpliter>
DataBase<DataTable, ParamSpliter>::~DataBase()
{
	for (auto it: mTable)
		delete it.second;
}

template<class DataTable, class ParamSpliter>
Value* DataBase<DataTable, ParamSpliter>::transValue(const std::string &VStr, int type) const
{
	Value *pt = NULL;
	switch (type)
	{
		case INT :
		{
			int val = 0;
			if (stralgo::str2int(VStr, val))
				pt = new AttributeValue<int>(val);
			break;
		}
		case DOUBLE :
		{
			double val = 0;
			if (stralgo::str2double(VStr, val))
				pt = new AttributeValue<double>(val);
			break;
		}
		case STRING :
		{
			pt = new AttributeValue<std::string>(VStr);
			break;
		}
	}
	return pt;
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::CreateTable(const std::string &command) {
	using namespace std;
	vector<string> param, not_null;
	vector<pair<string, int> > _attrType;
	string pri_key = "\0";
	auto res = ParamSpliter().split_create_table(command, param, not_null, pri_key);
	assert(res == TABLE_CREATE); // !!!

	for (int i = 1; i < param.size(); i += 2)
	{
		pair<string, int> _attr;
		_attr.first = param[i];
		param[i + 1] = upperized(param[i + 1]);
		if (attrTypeMap.count(param[i + 1]))
			_attr.second = attrTypeMap.at(param[i + 1]);
		else
			throw kERROR_TYPE_NOT_SUPPORT;
		_attrType.push_back(_attr);
	}
	if (!mTable.count(param[0]))
		mTable[param[0]] = new DataTable(param[0], _attrType, pri_key, not_null);
	else
		throw (kERROR_TABLE_EXIST);
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::DropTable(const std::string &Name) {
	auto it = mTable.find(Name);
	if (it != mTable.end()) {
		delete it->second;
		mTable.erase(it);
	} else {
		throw (kERROR_TABLE_NOT_EXIST);
	}
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::ShowTableCol(const std::string &Name) {
	auto it = mTable.find(Name);
	if (it != mTable.end())
		mTable[Name]->PrintAttributeTable();
	else
		throw (kERROR_TABLE_NOT_EXIST);
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::ShowTableAll(bool PrintTableName) { // temporarily function
	if (PrintTableName)
		std::cout << "Tables_in_" << __name << std::endl;
	for (auto it: mTable) {
		std::cout << it.first << std::endl;
	}
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::InsertData(const std::vector<std::string> &param)
{
	//using namespace stralgo;
	if (mTable.count(param[0]) && param.size() % 2 == 1)
	{
		auto _table = mTable[ param[0] ];
		std::vector<ATTRIBUTE> _attributes;
		int n = param.size() / 2;
		for (int i = 1; i <= n; i ++)
		{
			if (!_table->CheckAttributeName(param[i]))
			{
				throw (kERROR_ATTRIBUTE_NOT_EXIST);
				return;
			}
			auto _attr_type = _table->GetTypeof(param[i]);
			Value *pt = this->transValue(param[i + n], _attr_type);
			if (pt != NULL)
				_attributes.push_back( ATTRIBUTE(param[i], pt) );
			else		
				throw (kERROR_INSERT_DATA_ATTRIBUTE_TYPE_MISMATCH);
		}
		_table->Insert(_attributes);
	}
	else
	{
		if (!mTable.count(param[0]))
			throw (kERROR_TABLE_NOT_EXIST);
		else
			throw (kERROR_COMMAND_FORM);
	}
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::PrintSelectData(std::vector< std::pair<std::string, std::vector<Value*> > >& attrList)
{
	using namespace std;
	int n = attrList[0].second.size();
	if (!n)
		return;

	for (int i = 0; i < attrList.size(); i ++)
		cout << attrList[i].first << "\t";
	cout << endl;

	for (int i = 0; i < n; i ++)
	{
		for (int j = 0; j < attrList.size(); j ++)
			if (attrList[j].second[i] != NULL)
				cout << *(attrList[j].second[i]) << "\t";
			else
				cout << "NULL" << "\t";
		cout << endl;
	}
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::SelectData(const std::vector<std::string> &param)
{
	using namespace std;
	string _attrName = param[0];
	string _tableName = param[1];

	DataTable* _table = mTable[_tableName];

	if (!_table->CheckAttributeName(_attrName))
	{
		throw kERROR_ATTRIBUTE_NOT_EXIST;
		return;
	}

	vector<Data*> _dataList;
	vector< pair<string, vector<Value*> > > _attrList;
	_attrList.clear();

	if (param.size() == 3)
		_table->GetDataWhere(param[2], _dataList);
	else
		_table->GetDataWhere("", _dataList);

	if (_attrName == "*")
	{
		auto _attrTable = _table->GetAttributeTable();
		for (auto _attr: _attrTable)
		{
			_attrName = _attr.first;
			_attrList.resize(_attrList.size() + 1);
			_attrList.back().first = _attrName;
			_table->Select(_attrName, _dataList, _attrList.back().second);
		}
	}
	else
	{
		_attrList.resize(1);
		_attrList[0].first = _attrName;
		_table->Select(_attrName, _dataList, _attrList[0].second);
	}

	// print select data ...

	this->PrintSelectData(_attrList);
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::UpdateData(const std::vector<std::string> &param)
{
	// only set one attribute
	using namespace std;
	string _tableName = param[0];
	string _attrName  = param[1];
	string _attrVStr  = param[2];
	DataTable *_table = mTable[_tableName];
	if (!_table->CheckAttributeName(_attrName))
	{
		throw kERROR_ATTRIBUTE_NOT_EXIST;
		return;
	}

	Value *_attrVal = this->transValue(_attrVStr, _table->GetTypeof(_attrName));
	if (_attrVal == NULL)
	{
		throw kERROR_ATTRIBUTE_NOT_EXIST;
		return;
	}

	ATTRIBUTE _attribute = ATTRIBUTE(_attrName, _attrVal);

	static vector<Data*> _dataList;
	if (param.size() == 4)
		_table->GetDataWhere(param[3], _dataList);
	else
		_table->GetDataWhere("", _dataList);

	_table->Update(_attribute, _dataList);
}

template<class DataTable, class ParamSpliter>
void DataBase<DataTable, ParamSpliter>::DeleteData(const std::vector<std::string> &param)
{
	using namespace std;

	string _tableName = param[0];
	DataTable *_table = mTable[_tableName];

	static vector<Data*> _dataList;
	if (param.size() == 2)
		_table->GetDataWhere(param[1], _dataList);
	else
		_table->GetDataWhere("", _dataList);

	_table->Remove(_dataList);
}