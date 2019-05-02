#pragma once

#include <iostream>
#include <string>
#include <list>
#include <map>

#include "DataTable.h"

class DataBase {

private:
	std::map<std::string, DataTable*> mTable;
	std::string __name;

public:
	DataBase(const std::string &DBName);
	~DataBase();

	void CreateTable(const std::string &command);
	void DropTable(const std::string &tableName);
	void ShowTableCol(const std::string &tableName);
	void ShowTableAll();

	void InsertData(const std::vector<std::string> &param);
	void DeleteData(const std::vector<std::string> &param);
	void UpdateData(const std::vector<std::string> &param);
	void SelectData(const std::vector<std::string> &param);
};