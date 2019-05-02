#include "str_algorithm.h"

namespace stralgo {

void EraseSpace(std::string &str)
{
	while (!str.empty() && str.back() == ' ')
		str.pop_back();
	reverse(str.begin(), str.end());
	while (!str.empty() && str.back() == ' ')
		str.pop_back();
	reverse(str.begin(), str.end());
}

void CompressSpace(const std::string &str1, std::string &str2)
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

bool str2int(const std::string &str, int &val)
{
	val = 0;
	for (char c: str)
		if (isdigit(c))
			val = val * 10 + c - '0';
		else
			return false;
	return true;
}

void ReplaceMark(const std::string &src, std::string &dst)
{
	dst.clear();
	bool _in_string = 0;
	for (char c: src)
	{
		if (c == '\"')
			_in_string ^= 1;
		if (_in_string)
		{
			dst.push_back(c);
			continue;
		}
		if (c != '(' && c != ')' && c != ',' && c != ';') // this place can be replace with a self-definition function
			dst.push_back(c);
		else
			dst.push_back(' ');
	}
	EraseSpace(dst);
}

bool str2double(const std::string &str, double &val)
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

int str2int(const std::string &str)
{
	int val = 0;
	str2int(str, val);
	return val;
}

double str2double(const std::string &str)
{
	double val = 0;
	str2double(str, val);
	return val;
}

}