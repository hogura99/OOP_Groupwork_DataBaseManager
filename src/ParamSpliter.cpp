#include "ParamSpliter.h"

#include <stringstream>

int Split(const std::string &Command, std::vector<std::string> &param, int CmdType)
{
	std::stringstream ss(Command);
	string tmp;
	ss >> tmp;
	
}