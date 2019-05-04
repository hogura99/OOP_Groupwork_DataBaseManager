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
	
	// Split the command into parameters.
	// @param Command: command.
	// @param param: the splitted parameters.
	virtual int Split(const std::string &Command, std::vector<std::string> &param);

	// Split the "USE" clause.
	// @param ss: the stringstream of command.
	// @param param:
	//		* param[0]: the data base name.
	virtual int split_use(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "SHOW" clause.
	// @param ss: the stringstream of command.
	// @param param: not empty iff show the columns of table
	//		* param[0]: the name of table you want to SHOW columns.
	//		* otherwise param should be empty.
	virtual int split_show(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "DROP" clause.
	// @param ss: the stringstream of command.
	// @param param:
	//		* param[0]: the name of table of database you want to drop.
	virtual int split_drop(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "DELETE" clause.
	// @param ss: the stringstream of command.
	// @param param:
	//		* param[0]: the name of table you will work on.
	//		* param[1]: the where clause of delete.
	virtual int split_delete(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "CREATE" clause.
	// @param ss: the stringstream of command.
	// @param param: not empty iff create a database.
	//		* param[0]: the name of database you will create.
	//		** the split of "CREATE TABLE" should work on another function.
	virtual int split_create(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "SELECT" clause.
	// @param ss: the stringstream of command.
	// @param param:
	//		* param[0]: the name of the table
	//		* param[i]: the attributes to select
	// 		* the last element of param: the where clause of select
	virtual int split_select(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "UPDATE" clause.
	// @param ss: the stringstream of command.
	// @param param
	//		* param[0]: the name of the table
	//		* param[2*i+1,2*i+2]: the attribute name and correspondent setting value
	// 		* the last element of param: the where clause of update
	virtual int split_update(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "INSERT" clause.
	// @param ss: the stringstream of command.
	// @param param:
	//	    * param[0]: the name of the table 
	//	    * (param[i],param[i+param.size()/2]): the attribute name and correspondent value of insert data
	virtual int split_insert(std::stringstream &ss, std::vector<std::string> &param);

	// Split the "CREATE TABLE" clause.
	// @param ss: the stringstream of command.
	// @param param:
	//		* param[0]: the name of the table
	//		* param[2*i+1],param[2*i+2]: the attribute name and correspondent data type.
	// @param not_null: the name of not null keys.
	// @param pri_key: the name of primary key.
	virtual int split_create_table(const std::string &Command, std::vector<std::string> &param,
										std::vector<std::string> &not_null, std::string &pri_key);

	// Split the "WHERE" clause.
	// @param ss: the stringstream of command.
	// @param param: the clauses to split.
	virtual void split_where(std::stringstream &ss, std::vector<std::string> &param);
};

#endif