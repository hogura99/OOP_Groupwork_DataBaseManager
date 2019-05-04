#pragma once

#include <vector>
#include <string>
#include <map>

#include "errorstream.h"
#include "DataBase.h"
#include "DataTable.h"
#include "ParamSpliter.h"

/*
 ********************************************************
 *						definition						*
 ********************************************************
*/

template<class Value = Value, class DataBase = DataBase<Value, DataTable<Value>, ParamSpliter>, class ParamSpliter = ParamSpliter>
class DataBaseManager {

private:

	std::map<std::string, DataBase* > mBase;
	DataBase* mWorkBase;

public:
	DataBaseManager();
	~DataBaseManager();

	// Query each command.
	// @param command: SQL command.
	void Query(const std::string &command);

	// Create a database.
	// @param DBname: the name of database.
	void CreateBase(const std::string &DBname);

	// Delete a database.
	// @param DBname: the name of database.
	void DropBase(const std::string &DBname);

	// Use a database.
	// @param DBname: the name of database which you want to use.
	void UseBase(const std::string &DBname);

	// Print the names of all tables in all databases.
	void ShowBase();
};


/*
 ********************************************************
 *					implementation						*
 ********************************************************
*/


template<class Value, class DataBase, class ParamSpliter>
DataBaseManager<Value, DataBase, ParamSpliter>::DataBaseManager() {
	mWorkBase = NULL;
}

template<class Value, class DataBase, class ParamSpliter>
DataBaseManager<Value, DataBase, ParamSpliter>::~DataBaseManager() {
	mWorkBase = NULL;
	for (auto it: mBase)
		delete it.second;
}

template<class Value, class DataBase, class ParamSpliter>
void DataBaseManager<Value, DataBase, ParamSpliter>::Query(const std::string &Command) {
	using namespace std;
	vector<string> param;
	string command;
	stralgo::CompressSpace(Command, command);
	int cmd = ParamSpliter().Split(command, param);
	switch (cmd) {

		case BASE_CREATE: {
			CreateBase(param[0]);
			break;
		}
		case BASE_SHOW: {
			ShowBase();
			break;
		}
		case BASE_DROP: {
			DropBase(param[0]);
			break;
		}
		case BASE_USE: {
			UseBase(param[0]);
			break;
		}

		case TABLE_CREATE: {
			mWorkBase->CreateTable(command);
			break;
		}
		case TABLE_DROP: {
			mWorkBase->DropTable(param[0]);
			break;
		}
		case TABLE_SHOW_ALL: {
			mWorkBase->ShowTableAll();
			break;
		}
		case TABLE_SHOW_COL: {
			mWorkBase->ShowTableCol(param[0]);
			break;
		}

		case DATA_INSERT: {
			mWorkBase->InsertData(param);
			break;
		}
		case DATA_DELETE: {
			mWorkBase->DeleteData(param);
			break;
		}
		case DATA_UPDATE: {
			mWorkBase->UpdateData(param);
			break;
		}
		case DATA_SELECT: {
			mWorkBase->SelectData(param);
			break;
		}

		case FORM_ERROR: {
			throw (kERROR_COMMAND_FORM);
			break;
		}
		default: {
			throw (kERROR_UNKNOWN);
		}
	}
}

template<class Value, class DataBase, class ParamSpliter>
void DataBaseManager<Value, DataBase, ParamSpliter>::CreateBase(const std::string &DBName) {
	if (!mBase.count(DBName))
		mBase[DBName] = new DataBase(DBName);
	else
		throw (kERROR_BASE_EXIST);
}

template<class Value, class DataBase, class ParamSpliter>
void DataBaseManager<Value, DataBase, ParamSpliter>::ShowBase() {
	using namespace std;
	cout << "Database" << endl;
	for (auto it: mBase) {
		cout << it.first << endl;
		it.second->ShowTableAll(false);
	}
}

template<class Value, class DataBase, class ParamSpliter>
void DataBaseManager<Value, DataBase, ParamSpliter>::UseBase(const std::string &DBName) {
	mWorkBase = mBase[DBName];
}

template<class Value, class DataBase, class ParamSpliter>
void DataBaseManager<Value, DataBase, ParamSpliter>::DropBase(const std::string &DBName) {
	auto it = mBase.find(DBName);
	if (it != mBase.end()) {
		delete it->second;
		mBase.erase(it);
	} else {
		throw (kERROR_BASE_NOT_EXIST);
	}
}