#pragma once

#include <string>
#include <vector>

class ParamSpliter {
public:
	enum {
		BASE_DEFAULT,
		TABLE_CREATE,
		TABLE_DEFAULT,
		DATA_INSERT,
		DATA_DELETE,
		DATA_UPDATE,
		DATA_SELECT
	};

	static void Split(const std::string &Command, std::vector<std::string> &param, int CmdType);
};