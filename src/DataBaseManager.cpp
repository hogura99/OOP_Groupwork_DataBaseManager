#include <string>
#include <vector>

#include "DataBaseManager.h"
#include "ParamSpliter.h"

DataBaseManager::DataBaseManager() {
	mWorkBase = NULL;
}

DataBaseManager::~DataBaseManager() {
	mWorkBase = NULL;
	for (auto it: mBase)
		delete it.second;
}

bool DataBaseManager::handleCommand(const string &command) {
	using namespace std;
	vector<string> param;
	int cmd = ParamSpliter::Split(command, param/*, ParamSpliter::BASE_DEFAULT*/);
	switch (cmd) {

		case BASE_CREATE: {
			createBase(param[0]);
			break;
		}
		case BASE_SHOW: {
			showBase();
			break;
		}
		case BASE_DROP: {
			dropBase(param[0]);
			break;
		}
		case BASE_USE: {
			useBase(param[0]);
			break;
		}

		case TABLE_CREATE: {
			mWorkBase->createTable(command);
			break;
		}
		case TABLE_DROP: {
			mWorkBase->dropBase(param[0]);
			break;
		}
		case TABLE_SHOW_ALL: {
			mWorkBase->showTableAll();
			break;
		}
		case TABLE_SHOW_COL: {
			mWorkBase->showTable(param[0]);
			break;
		}

		case DATA_INSERT: {
			mWorkBase->insertData(param);
			break;
		}
		case DATA_DELETE: {
			mWorkBase->deleteData(param);
			break;
		}
		case DATA_UPDATE: {
			mWorkBase->updateData(param);
			break;
		}
		case DATA_SELECT: {
			mWorkBase->selectData(param);
			break;
		}

		default: {
			
			break;
		}
	}
}

void DataBaseManager::createBase(const string &DBName) {
	mBase[DBName] = new DataBase(DBName);
}

void DataBaseManager::showBase() {
	cout << " ++++++++++ \n ---------- " << endl;
	for (auto it: mBase) {
		cout << it.first << endl;
		it.second->printTable();
		cout << " ---------- " << endl;
	}
	cout << " ++++++++++\n " << endl;
}

void DataBaseManager::useBase(const string &DBName) {
	mWorkBase = mBase[DBName];
}

void DataBaseManager::dropBase(const string &DBName) {
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