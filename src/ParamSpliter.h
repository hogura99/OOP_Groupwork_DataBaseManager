#pragma once

#ifndef PARAM_SPLITER_H
#define PARAM_SPLITER_H

#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include <cctype>
#include <cmath>

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

	//static void initMapping();
	
	static int  Split(const std::string &Command, std::vector<std::string> &param/*, int CmdType*/);
	//static void Split(const std::string &Command, std::vector<std::string> &param,
	//											  std::vector<std::string> &not_null, std::string pri_key);
	static int split_use(std::stringstream &ss, std::vector<std::string> &param);
	static int split_show(std::stringstream &ss, std::vector<std::string> &param);
	static int split_drop(std::stringstream &ss, std::vector<std::string> &param);
	static int split_delete(std::stringstream &ss, std::vector<std::string> &param);
	static int split_create(std::stringstream &ss, std::vector<std::string> &param);
	static int split_select(std::stringstream &ss, std::vector<std::string> &param);
	static int split_update(std::stringstream &ss, std::vector<std::string> &param);
	static int split_insert(std::stringstream &ss, std::vector<std::string> &param);
	static int split_createTable(const std::string &Command, std::vector<std::string> &param,
										std::vector<std::string> &not_null, std::string pri_key);
};

void eraseSpace(std::string &str);

class Params
{
public:

	static void cmprSpace(const std::string &str1, std::string &str2)
	{
		if (str1.empty())
			return;
		str2 = "";
		str2.push_back(str1[0]);
		for (size_t i = 0; i + 1 < str1.length(); i ++)
			if (str1[i] == str1[i + 1] && str1[i] == ' ')
				continue;
			else
				str2.push_back(str1[i + 1]);
	}

	static bool str2int(const std::string &str, int &val)
	{
		val = 0;
		for (char c: str)
			if (isdigit(c))
				val = val * 10 + c - '0';
			else
				return false;
		return true;
	}

	static bool str2double(const std::string &str, double &val)
	{
		val = 0;
		int p = -1;
		for (size_t i = 0; i < str.length(); i ++)
		{
			if (str[i] == '.')
			{
				if (~p)
					return false;
				p = i;
			}
			else if (isdigit(str[i]))
				val = val * 10 + int(str[i] - '0');
			else
				return false;
		}
		if (~p)
			val = val / pow(10, str.length() - p - 1);
		return true;
	}

	static int str2int(const std::string &str)
	{
		int val = 0;
		str2int(str, val);
		return val;
	}

	static double str2double(const std::string &str)
	{
		double val = 0;
		str2double(str, val);
		return val;
	}

};

#endif