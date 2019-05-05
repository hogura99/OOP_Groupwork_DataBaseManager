#include "ParamSpliter.h"

#include <assert.h>

#include <sstream>

#define IS_FROM(str) (upperized(str) == "FROM")

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

int ParamSpliter::Split(const std::string &Command, std::vector<std::string> &param)
{
	std::stringstream ss(Command);
	std::string str;
	ss >> str;
	if (!cmdType.count(str))
		return FORM_ERROR;
	int res = BASE_DEFAULT;
	//std::cerr << ">>> " << str << std::endl;
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
	if (upperized(str) == "TABLE")
		return TABLE_CREATE;
	if (upperized(str) != "DATABASE")
		return FORM_ERROR;
	ss >> str;
	if (str.length() == 0)
		return FORM_ERROR;
	param.clear();
	param.push_back(str);
	return BASE_CREATE;
}

void ParamSpliter::split_where(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string expr = "", expr2 = "";
	std::string str = "";
	while (!ss.eof())
	{
		ss >> str;
		for (int i = 0; i < str.length(); i ++)
		{
			bool flag = false;
			for (int j = std::min(3, (int)str.length() - i); j >= 1; j --) // 判断是否为长度为3、2、1的运算符
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
	stralgo::CompressSpace(expr, expr2);
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
	if (!(ss >> str)) // 没有 where 语句
	{
		param.push_back("\0");
		return DATA_SELECT;
	}

	if (upperized(str) != "WHERE")
		return FORM_ERROR;

	split_where(ss, param);
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
	param.push_back(str); // 属性名
	if (ss.eof()) // 没有 where 语句
	{
		param.push_back("");
		return DATA_DELETE;
	}

	ss >> str;
	if (upperized(str) != "WHERE" || ss.eof())
		return FORM_ERROR;
	split_where(ss, param);
	return DATA_DELETE;
}

#define CHECK_STR(pt) {		\
	if (pt == std::string::npos) \
		return FORM_ERROR;	\
}

int ParamSpliter::split_create_table(const std::string &Command, std::vector<std::string> &param,
											std::vector<std::string> &not_null, std::string &pri_key)
{
	using namespace std;
	string _command, str;
	stralgo::ReplaceMark(Command, _command);
	stringstream ss(_command);
	ss >> str;
	//assert(str == "CREATE");
	ss >> str;
	//assert(str == "TABLE");
	param.clear();

	ss >> str;
	param.push_back(str);

	str = "";
	string _attrName = "", _attrType = "";
	while (!ss.eof())
	{
		ss >> _attrName >> _attrType;

		if (upperized(_attrName) == "PRIMARY" && upperized(_attrType) == "KEY")
		{
			ss >> str;
			pri_key = str;
			continue;
		}

		if (upperized(_attrName) == "NOT" && upperized(_attrType) == "NULL")
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
	stralgo::ReplaceMark(Command, _command);
	stringstream cmd(_command);

	cmd >> str >> str; // INSERT INTO
	if (cmd.eof() || upperized(str) != "INTO")
		return FORM_ERROR;

	param.clear();
	cmd >> str;
	param.push_back(str); // 表名

	while (!cmd.eof())
	{
		cmd >> str;
		if (upperized(str) != "VALUES")
			param.push_back(str);
	}

	return DATA_INSERT;
}

int ParamSpliter::split_update(std::stringstream &_ss, std::vector<std::string> &param)
{
	if (_ss.eof())
		return FORM_ERROR;
	auto tmp = _ss.str();
	std::string cmd;
	stralgo::ReplaceMark(tmp, cmd);
	std::stringstream ss(cmd);

	std::string str;
	ss >> str;
	// assert(str == "UPDATE");

	param.clear();
	ss >> str;
	param.push_back(str);
	ss >> str;
	if (upperized(str) != "SET" || ss.eof())
		return FORM_ERROR;

	while (!ss.eof())
	{
		std::string expr, str1, str2;
		ss >> str;
		if (upperized(str) == "WHERE")
			break;
		// 对每个“=”前后的空格分情况讨论
		if (str.find('=') == std::string::npos || str.back() == '=') // =前有空格，=前无空格后有空格
		{
			if (str.back() == '=')
				str.pop_back();
			param.push_back(str);
			ss >> str;
			if (str == "=") // =前有空格，后有空格
			{
				ss >> str;
				param.push_back(str);
			}
			else // =前有空格，后无空格
			{
				str = str.substr(1, (int) str.length() - 1); // 去除字符串首尾的等号
				param.push_back(str);
			}
		}
		else
		{ // =前后均无空格
			size_t p = str.find('=');
			if (p == 0 || p + 1 == str.length())
				return FORM_ERROR;
			str1 = str.substr(0, p);
			str2 = str.substr(p + 1, str.length() - p); // 删去等号
			param.push_back(str1);
			param.push_back(str2);
		}
	}
	split_where(ss, param);
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
	if (upperized(str) == "DATABASES")
		return BASE_SHOW;
	else if (upperized(str) == "TABLES")
		return TABLE_SHOW_ALL;
	else if (upperized(str) != "COLUMNS")
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
	if (upperized(str) == "DATABASE")
		return BASE_DROP;
	else if (upperized(str) == "TABLE")
		return TABLE_DROP;
	else
		return FORM_ERROR;
}

#undef CHECK_STR