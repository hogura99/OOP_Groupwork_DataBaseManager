#include <string>
#include <vector>

#include "DataBaseManager.h"

DataBaseManager::DataBaseManager() {
	mWorkBase = NULL;
}

DataBaseManager::~DataBaseManager() {
	mWorkBase = NULL;
	for (auto it: mBase)
		delete it.second;
}

void DataBaseManager::Query(const std::string &Command) {
	using namespace std;
	vector<string> param;
	string command;
	stralgo::CompressSpace(Command, command);
	int cmd = ParamSpliter::Split(command, param);
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

		default: {
			
			break;
		}
	}
}

void DataBaseManager::CreateBase(const std::string &DBName) {
	mBase[DBName] = new DataBase(DBName);
}

void DataBaseManager::ShowBase() {
	using namespace std;
	cout << "Database" << endl;
	for (auto it: mBase) {
		cout << it.first << endl;
		it.second->ShowTableAll();
	}
}

void DataBaseManager::UseBase(const std::string &DBName) {
	mWorkBase = mBase[DBName];
}

void DataBaseManager::DropBase(const std::string &DBName) {
	auto it = mBase.find(DBName);
	try {
		if (it != mBase.end()) {
			delete it->second;
			mBase.erase(it);
		} else
			throw false;
	}
	catch (bool) {
		// deletion fail
	}
}