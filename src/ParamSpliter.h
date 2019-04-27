#pragma once

#include <string>
#include <vector>

enum {
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

enum {
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

	static void initMapping();
	
	static int  Split(const std::string &Command, std::vector<std::string> &param/*, int CmdType*/);
	static void Split(const std::string &Command, std::vector<std::string> &param,
												  std::vector<std::string> &not_null, std::string pri_key);
	static int split_use(std::stringstream &ss, std::vector<std::string> &param);
	static int split_show(std::stringstream &ss, std::vector<std::string> &param);
	static int split_drop(std::stringstream &ss, std::vector<std::string> &param);
	static int split_delete(std::stringstream &ss, std::vector<std::string> &param);
	static int split_create(std::stringstream &ss, std::vector<std::string> &param);
	static int split_select(std::stringstream &ss, std::vector<std::string> &param);
	static int split_update(std::stringstream &ss, std::vector<std::string> &param);
	static int split_insert(std::stringstream &ss, std::vector<std::string> &param);
	static void split_createTable(const std::string &Command, std::vector<std::string> &param,
										std::vector<bool> &not_null, std::string pri_key);
};
