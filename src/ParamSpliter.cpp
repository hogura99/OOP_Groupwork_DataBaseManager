#include "ParamSpliter.h"

#include <stringstream>

std::map<std::string, int> ParamSpliter::cmdType = std::map<std::string, int>;

void ParamSpliter::initMapping() {
	cmdType["INSERT"] = cINSERT;
	cmdType["CREATE"] = cCREATE;
	cmdType["UPDATE"] = cUPDATE;
	cmdType["DELETE"] = cDELETE;
	cmdType["SELECT"] = cSELECT;
	cmdType["DROP"] = cDROP;
	cmdType["SHOW"] = cSHOW;
	cmdType["USE"] = cUSE;
}

int ParamSpliter::Split(const std::string &Command, std::vector<std::string> &param/*, int CmdType*/)
{
	std::stringstream ss(Command);
	string str;
	ss >> str;
	if (!cmdType.count(str))
		return FORM_ERROR;
	int res = BASE_DEFAULT;
	switch (cmdType[str])
	{
		case cCREATE:
		{
			res = split_create(ss, param);
			break;
		}
		case cINSERT:
		{
			res = DATA_INSERT;
			break;
		}
		case cUPDATE:
		{
			res = DATA_UPDATE;
			break;
		}
		case cSELECT:
		{
			res = split_select(ss, param);
			break;
		}
		case cDELETE:
		{
			res = split_delete(ss, param);
			break;
		}
		case cUSE:
		{
			res = split_use(ss, param);
			break;
		}
		case cDROP:
		{
			res = split_drop(ss, param);
			break;
		}
		case cSHOW:
		{
			res = split_show(ss, param);
			break;
		}
		default:
			break;
	}
	return res;
}

int ParamSpliter::split_create(std::stringstream &ss, std::vector<std::string> &param);
{
	std::string str;
	ss >> str;
	if (ss == "TABLE")
		return TABLE_CREATE;
	if (ss != "BASE")
		return FORM_ERROR;
	ss >> str;
	if (ss != "DATABASE")
		return FORM_ERROR;
	ss >> str;
	if (ss.length() == 0)
		return FORM_ERROR;
	param.clear();
	param.push_back(ss);
	return BASE_CREATE;
}

int ParamSpliter::split_createTable(std::stringstream &ss, std::vector<std::string> &param);
{
	
}

#define GET_WEHRE_CLAUSE(ss, param) \
{									\
	string expr;					\
	while (ss >> str) {				\
		expr += str;				\
		if (!ss.eof())				\
			expr += " ";			\
	}								\
	param.push_back(expr);			\
}

int ParamSpliter::split_select(std::stringstream &ss, std::vector<std::string> &param);
{
	std::string str;
	param.clear();
	while (ss >> str) {
		if (str == "FROM")
			break;
		param.push_back(str);
	}

	if (str != "FROM")
		return FORM_ERROR;
	if (!(ss >> str))
		return FORM_ERROR;

	param.push_back(str);
	if (!(ss >> str)) // no where clause
	{
		param.push_back("\0");
		return DATA_SELECT;
	}

	if (str != "WHERE")
		return FORM_ERROR;

	GET_WEHRE_CLAUSE(ss, param);
	return DATA_SELECT;
}

int ParamSpliter::split_delete(std::stringstream &ss, std::vector<std::string> &param);
{
	std::string str;
	ss >> str;
	if (str != "FROM" || ss.eof())
		return FORM_ERROR;
	ss >> str;
	param.clear();
	param.push_back(str); // add attribute name
	if (ss.eof()) // no where clause
	{
		param.push_back("\0");
		return DATA_DELETE;
	}

	ss >> str;
	if (ss != "WHERE" || ss.eof())
		return FORM_ERROR;
	GET_WEHRE_CLAUSE(ss, param);
	return DATA_DELETE;
}

int ParamSpliter::split_use(std::stringstream &ss, std::vector<std::string> &param);
{
	std::string str;
	ss >> str;
	if (str.empty() || !ss.eof())
		return FORM_ERROR;
	param.clear();
	param.push_back(str);
	return BASE_USE;
}

int ParamSpliter::split_show(std::stringstream &ss, std::vector<std::string> &param);
{
	std::string str;
	if (ss.eof())
		return FORM_ERROR;
	ss >> str;
	if (str == "DATABASES")
		return BASE_SHOW;
	else if (str == "TABLES")
		return TABLE_SHOW_ALL;
	else if (str == "FROM")
		return FORM_ERROR;

	param.clear();
	do {
		param.push_back(str);
		ss >> str;
	} while (ss != "FROM" && !ss.eof());
	if (ss.eof())
		return FORM_ERROR;
	ss >> str;
	if (ss.eof())
		return FORM_ERROR;
	ss >> str;
	param.push_back(str);

	return TABLE_SHOW_COL;
}

int ParamSpliter::split_drop(std::stringstream &ss, std::vector<std::string> &param);
{
	std::string str, str2;
	if (ss.eof())
		return FORM_ERROR;
	ss >> str;
	if (ss.eof())
		return FORM_ERROR;

	param.clear();
	ss >> str2;
	param.push_back(str2);
	if (str == "DATABASE")
		return BASE_DROP;
	else if (str == "TABLE")
		return TABLE_DROP;
	else
		return FORM_ERROR;
}

#undef GET_WEHRE_CLAUSE