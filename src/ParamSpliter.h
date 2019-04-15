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

class ParamSpliter {
private:
	map<string, int> cmdType;

public:

	enum {
		bILLEAL,
		bCREATE,
		bDROP,
		bSHOW,
		bUSE,
	};

	static int  Split(const std::string &Command, std::vector<std::string> &param, int CmdType);
	static void Split(const std::string &Command, std::vector<std::string> &param,
												  std::vector<std::string> &not_null, std::string pri_key);
};
