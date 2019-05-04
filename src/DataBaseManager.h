#pragma once

#include <string>
#include <map>

#include "errorstream.h"
#include "DataBase.h"
#include "ParamSpliter.h"

class DataBaseManager {

private:

	std::map<std::string, DataBase*> mBase;
	DataBase* mWorkBase;

public:
	DataBaseManager();
	~DataBaseManager();

	void Query(const std::string &command);

	void CreateBase(const std::string &DBname);
	void DropBase(const std::string &DBname);
	void UseBase(const std::string &DBname);
	void ShowBase();
};