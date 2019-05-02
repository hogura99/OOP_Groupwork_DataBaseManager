#include "ParamSpliter.h"

#include <assert.h>

#include <sstream>

#define IS_FROM(str) (str == "FROM" || str == "from")

std::map<std::string, int> ParamSpliter::cmdType = 
{
	{"INSERT", cINSERT},
	{"CREATE", cCREATE},
	{"UPDATE", cUPDATE},
	{"DELETE", cDELETE},
	{"SELECT", cSELECT},
	{"DROP", cDROP},
	{"SHOW", cSHOW},
	{"USE", cUSE},

	{"insert", cINSERT},
	{"create", cCREATE},
	{"update", cUPDATE},
	{"delete", cDELETE},
	{"select", cSELECT},
	{"drop", cDROP},
	{"show", cSHOW},
	{"use", cUSE},
};

int ParamSpliter::Split(const std::string &Command, std::vector<std::string> &param/*, int CmdType*/)
{
	std::stringstream ss(Command);
	std::string str;
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
			res = split_insert(ss, param);
			break;
		}
		case cUPDATE:
		{
			res = split_update(ss, param);
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

int ParamSpliter::split_create(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string str;
	ss >> str;
	if (str == "TABLE")
		return TABLE_CREATE;
	if (str != "DATABASE")
		return FORM_ERROR;
	/*ss >> str;
	if (str != "DATABASE")
		return FORM_ERROR;*/
	ss >> str;
	if (str.length() == 0)
		return FORM_ERROR;
	param.clear();
	param.push_back(str);
	return BASE_CREATE;
}

void ParamSpliter::split_where(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string expr, expr2;
	std::string str;
	while (!ss.eof())
	{
		ss >> str;
		for (int i = 0; i < str.length(); i ++)
		{
			bool flag = false;
			for (int j = 3; j >= 1 && i + j <= str.length(); j --)
			{
				if (Exprs::oprTYPE.count(str.substr(i, j)))
				{
					expr += " " + str.substr(i, j) + " ";
					i += j - 1;
					flag = true;
					break;
				}
			}
			if (!flag)
				expr += str[i];
		}
		if (!ss.eof())
			expr += " ";
	}
	Params::cmprSpace(expr, expr2);
	param.push_back(expr2);
}

int ParamSpliter::split_select(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string str;
	param.clear();
	while (ss >> str) {
		if (IS_FROM(str))
			break;
		param.push_back(str);
	}

	if (!IS_FROM(str))
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

	ParamSpliter::split_where(ss, param);
	return DATA_SELECT;
}

int ParamSpliter::split_delete(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string str;
	ss >> str;
	if (!IS_FROM(str) || ss.eof())
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
	if (str != "WHERE" || ss.eof())
		return FORM_ERROR;
	ParamSpliter::split_where(ss, param);
	return DATA_DELETE;
}

#define CHECK_STR(pt) {		\
	if (pt == std::string::npos) \
		return FORM_ERROR;	\
}

int ParamSpliter::split_createTable(const std::string &Command, std::vector<std::string> &param,
											std::vector<std::string> &not_null, std::string &pri_key)
{
	using namespace std;
	string _command, str;
	Params::replaceMark(Command, _command);
	stringstream ss(_command);
	ss >> str;
	assert(str == "CREATE");
	ss >> str;
	assert(str == "TABLE");
	param.clear();

	ss >> str;
	param.push_back(str);

	str = "";
	string _attrName = "", _attrType = "";
	while (!ss.eof())
	{
		ss >> _attrName >> _attrType;

		if (_attrName == "PRIMARY" && _attrType == "KEY")
		{
			ss >> str;
			pri_key = str;
			continue;
		}

		if (_attrName == "NOT" && _attrType == "NULL")
		{
			if (str == "")
				return FORM_ERROR;
			not_null.push_back(str);
			str = "";
			continue;
		}

		param.push_back(_attrName);
		param.push_back(_attrType);
		str = _attrName;
	}
	return TABLE_CREATE;
}

int ParamSpliter::split_insert(std::stringstream &ss, std::vector<std::string> &param)
{
	using namespace std;
	string Command = ss.str(), _command, str;
	Params::replaceMark(Command, _command);
	stringstream cmd(_command);

	cmd >> str >> str; // INSERT INTO
	if (cmd.eof() || str != "INTO")
		return FORM_ERROR;

	param.clear();
	cmd >> str;
	param.push_back(str); // table name

	while (!cmd.eof())
	{
		cmd >> str;
		if (str != "VALUES")
			param.push_back(str);
	}

	return DATA_INSERT;
}

int ParamSpliter::split_update(std::stringstream &ss, std::vector<std::string> &param)
{
	if (ss.eof())
		return FORM_ERROR;
	param.clear();
	std::string str;
	ss >> str;
	param.push_back(str);
	ss >> str;
	if (str != "SET" || ss.eof())
		return FORM_ERROR;

	while (!ss.eof())
	{
		std::string expr, str1, str2;
		ss >> str;
		if (str == "WHERE")
			break;
		if (str.find('=') == std::string::npos || str.back() == '=')
		{
			if (str.back() == '=')
				str.pop_back();
			param.push_back(str);
			ss >> str;
			if (str == "=")
			{
				ss >> str;
				param.push_back(str);
			}
			else
			{
				str = str.substr(1, (int) str.length() - 1);
				param.push_back(str);
			}
		}
		else
		{
			size_t p = str.find('=');
			if (p == 0 || p + 1 == str.length())
				return FORM_ERROR;
			str1 = str.substr(0, p);
			str2 = str.substr(p + 1, str.length() - p);
			param.push_back(str1);
			param.push_back(str2);
		}
	}
	if (str == "WHERE")
		ParamSpliter::split_where(ss, param);
	return DATA_UPDATE;
}

int ParamSpliter::split_use(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string str;
	ss >> str;
	if (str.empty() || !ss.eof())
		return FORM_ERROR;
	param.clear();
	param.push_back(str);
	return BASE_USE;
}

int ParamSpliter::split_show(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string str;
	if (ss.eof())
		return FORM_ERROR;
	ss >> str;
	if (str == "DATABASES")
		return BASE_SHOW;
	else if (str == "TABLES")
		return TABLE_SHOW_ALL;
	else if (str != "columns")
		return FORM_ERROR;

	param.clear();
	do {
		ss >> str;
	} while (!IS_FROM(str) && !ss.eof());
	if (ss.eof())
		return FORM_ERROR;
	ss >> str;
	if (str.back() == ';')
		str.pop_back();
	param.push_back(str);

	return TABLE_SHOW_COL;
}

int ParamSpliter::split_drop(std::stringstream &ss, std::vector<std::string> &param)
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

void eraseSpace(std::string &str)
{
	while (!str.empty() && str.back() == ' ')
		str.pop_back();
	reverse(str.begin(), str.end());
	while (!str.empty() && str.back() == ' ')
		str.pop_back();
	reverse(str.begin(), str.end());
}

#undef CHECK_STR