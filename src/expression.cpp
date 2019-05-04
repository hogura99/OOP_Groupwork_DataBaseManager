#include "expression.h"

std::string upperized(std::string str)
{
	for (char &c: str)
		if ('a' <= c && c <= 'z')
			c += 'A' - 'a';
	return str;
}

bool is_logic_oprt(std::string opr)
{
	opr = upperized(opr);
	return ((opr) == "OR" || (opr) == "AND" || (opr) == "NOT");
}