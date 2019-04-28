#pragma once

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <stack>
#include <map>

enum OPERATORS
{
	opEQ,
	opGI,
	opLE,
	opGIQ,
	opLEQ,
	opNEQ,
	opNOT,
	opOR,
	opAND,
	opCOUNT,
};

const int oprORDER[] =
{
	1,
	1,
	1,
	1,
	1,
	1,
	4,
	2,
	3,
};

namespace Exprs
{

	static std::map<std::string, int> oprTYPE =
	{
		{ "==", opEQ },
		{ ">" , opGI },
		{ "<" , opLE },
		{ ">=", opGIQ },
		{ "<=", opLEQ },
		{ "!=", opNEQ },
		{ "OR", opOR },
		{ "AND", opAND },
		{ "NOT", opNOT },
	};

}

#define IS_LOGIC_OPRT(opr) ((opr) == "OR" || (opr) == "AND" || (opr) == "NOT")

#endif