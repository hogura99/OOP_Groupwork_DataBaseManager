#pragma once

#ifndef PARAM_SPLITER_H
#define PARAM_SPLITER_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <cctype>

#include "str_algorithm.h"
#include "expression.h"
#include "errorstream.h"

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
	
	virtual int Split(const std::string &Command, std::vector<std::string> &param);
	virtual int split_use(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_show(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_drop(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_delete(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_create(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_select(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_update(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_insert(std::stringstream &ss, std::vector<std::string> &param);
	virtual int split_create_table(const std::string &Command, std::vector<std::string> &param,
										std::vector<std::string> &not_null, std::string &pri_key);

	virtual void split_where(std::stringstream &ss, std::vector<std::string> &param);
};

#endif