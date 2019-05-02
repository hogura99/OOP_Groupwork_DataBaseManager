#pragma once

#ifndef PARAM_SPLITER_H
#define PARAM_SPLITER_H

#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include <cctype>

#include "str_algorithm.h"
#include "expression.h"

enum DATA_BASE_OPEARTION {
	FORM_ERROR,
	BASE_DEFAULT,
	BASE_CREATE,
	BASE_SHOW,
	BASE_DROP,
	BASE_USE,
	TABLE_CREATE,
	TABLE_DROP,
	TABLE_SHOW_ALL,
	TABLE_SHOW_COL,
	DATA_INSERT,
	DATA_DELETE,
	DATA_UPDATE,
	DATA_SELECT,
};

enum DATA_BASE_COMMAND {
	cILLEAL,
	cCREATE,
	cINSERT,
	cUPDATE,
	cDELETE,
	cSELECT,
	cDROP,
	cSHOW,
	cUSE,
};

class ParamSpliter {
private:
	static std::map<std::string, int> cmdType;

public:
	
	static int Split(const std::string &Command, std::vector<std::string> &param);
	static int split_use(std::stringstream &ss, std::vector<std::string> &param);
	static int split_show(std::stringstream &ss, std::vector<std::string> &param);
	static int split_drop(std::stringstream &ss, std::vector<std::string> &param);
	static int split_delete(std::stringstream &ss, std::vector<std::string> &param);
	static int split_create(std::stringstream &ss, std::vector<std::string> &param);
	static int split_select(std::stringstream &ss, std::vector<std::string> &param);
	static int split_update(std::stringstream &ss, std::vector<std::string> &param);
	static int split_insert(std::stringstream &ss, std::vector<std::string> &param);
	static int split_create_table(const std::string &Command, std::vector<std::string> &param,
										std::vector<std::string> &not_null, std::string &pri_key);

	static void split_where(std::stringstream &ss, std::vector<std::string> &param);
};

#endif