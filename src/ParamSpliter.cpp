#include "ParamSpliter.h"

#include <stringstream>

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
};

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

int ParamSpliter::split_select(std::stringstream &ss, std::vector<std::string> &param)
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

int ParamSpliter::split_delete(std::stringstream &ss, std::vector<std::string> &param)
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

#define CHECK_STR(pt) {		\
	if (pt == string::npos) \
		return FORM_ERROR;	\
}

void ParamSpliter::split_createTable(const std::string &Command, std::vector<std::string> &param,
											std::vector<std::string> &not_null, std::string pri_key)
{
	std::stringstream ss(Command);
	std::string str;
	ss >> str;
	assert(str == "CREATE");
	ss >> str;
	assert(str == "TABLE");
	size_t pt_l = Command.find("TABLE") + 5;
	size_t pt_r = Command.find_first_of('(', pt_l);
	CHECK_STR(pt_r);
	param.clear();

	str = Command.substr(pt_l, pt_r - pt_l); // table name
	param.push_back(str);
	// 以下放弃处理非法读入情况

	size_t brack_r = Command.find_first_of(')', pt_r);
	CHECK_STR(brack_r);
	pt_l = pt_r + 1;
	pt_r = pt_l;
	while (pt_l < brack_r)
	{
		pt_r = Command.find_first_of(',', pt_l);
		if (pt_r == string::npos || pt_r > brack_r)
			pt_r = brack_r;
		size_t pt_not_null = Command.find("NOT NULL", pt_l);
		if (pt_not_null != string::npos && pt_not_null < pt_r)
		{
			str = Command.substr(pt_l, pt_not_null - pt_l);
			eraseSpace(str);
			param.push_back(str);
			not_null.push_back(str);
		}
		else
		{
			str = Command.substr(pt_l, pt_r - pt_l);
			eraseSpace(str);
			param.push_back(str);
		}
		pt_l = pt_r + 1;
	}

	pt_r = Command.find("PRIMARY KEY", pt_l);
	CHECK_STR(pt_r);
	pt_l = Command.find_first_of('(', pt_r);
	CHECK_STR(pt_l);
	pt_l ++;
	pt_r = Command.find_first_of(')', pt_l);
	pri_key = Command.substr(pt_l, pt_r - pt_l);
}

int ParamSpliter::split_insert(std::stringstream &ss, std::vector<std::string> &param)
{
	std::string str, cmd;
	ss >> str;
	if (ss.eof() || str != "INTO")
		return FORM_ERROR;
	cmd = ss.str();

	size_t pt_l = ss.find("INTO") + 5;
	size_t pt_r = ss.find_first_of('(', pt_l);
	CHECK_STR(pt_r);

	str = cmd.substr(pt_l, pt_r - pt_l);
	param.clear();
	eraseSpace(str);
	param.push_back(str); // the name of data table

	int count_attr = 0;

	size_t brack_r = cmd.find_first_of(')', pt_r);
	CHECK_STR(brack_r);

	pt_l = pt_r + 1;
	while (pt_l < brack_r && cmd[pt_l] == ' ')
		pt_l ++;
	while (pt_r < brack_r)
	{
		pt_r = cmd.find_first_of(',', pt_l);
		if (pt_r == string::npos)
			pt_r = brack_r;
		str = cmd.substr(pt_l, pt_r - pt_l);
		eraseSpace(str);
		param.push_back(str);
		pt_l = pt_r + 1;
		count_attr ++;
	}

	pt_l = cmd.find("VALUES");
	CHECK_STR(pt_l);
	pt_l = cmd.find_first_of('(', pt_l);
	CHECK_STR(pt_l);
	brack_r = cmd.find_first_of(')', pt_l);
	CHECK_STR(brack_r);

	int count_value = 0;
	pt_l = pt_l + 1;
	pt_r = pt_l;
	while (pt_r < brack_r)
	{
		pt_r = cmd.find_first_of(',', pt_l);
		if (pt_r == string::npos)
			pt_r = brack_r;
		str = cmd.substr(pt_l, pt_r - pt_l);
		eraseSpace(str);
		param.push_back(str);
		pt_l = pt_r + 1;
		count_value ++;
	}

	if (count_value != count_attr)
		return FORM_ERROR;
	else
		return DATA_INSERT;
}

int ParamSpliter::split_update(std::stringstream &ss, std::vector<std::string> &param)
{
	if (ss.eof())
		return FORM_ERROR;
	param.clear();
	std::string str;
	ss >> str >> str;
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
		if (str.find('=') == string::npos || *str.back() == '=')
		{
			if (*str.back() == '=')
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
			str1 = str.sub(0, p - 1);
			str2 = str.sub(p + 1, str.length() - p);
			param.push_back(str1);
			param.push_back(str2);
		}
	}
	if (str == "WHERE")
		GET_WEHRE_CLAUSE(ss, param);
	return TABLE_UPDATE;
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

#undef GET_WEHRE_CLAUSE
#undef CHECK_STR