#pragma once

#ifndef DATA_H
#define DATA_H
#include "Derived.h"

#include <string>
#include <map>

class Data{
	std::map<std::string, Value* > varMap;
public:
	Data() {}
	~Data() {}
	
	Value* getData(std::string attrName) const;
	Value* setData(std::string attrName, Value* src);
	int getTypename(std::string attrName);
};

#endif