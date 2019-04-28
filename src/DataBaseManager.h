#pragma once

#include <string>
#include <map>

#include "DataBase.h"
#include "ParamSpliter.h"

class DataBaseManager {

private:

	std::map<std::string, DataBase*> mBase;
	DataBase* mWorkBase;

public:
	DataBaseManager();
	~DataBaseManager();
	void handleCommand(const std::string &command);
	void createBase(const std::string &DBname);
	void dropBase(const std::string &DBname);
	void useBase(const std::string &DBname);
	void showBase();
};