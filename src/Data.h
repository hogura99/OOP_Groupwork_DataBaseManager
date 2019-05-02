#pragma once

#ifndef DATA_H
#define DATA_H
#include "AttributeValue.h"

#include <string>
#include <map>

class Data{
	std::map<std::string, Value* > varMap;
public:
	Data() {}
	~Data() {}
	
	Value* getValue(std::string attrName) const;
	Value* setValue(std::string attrName, Value* src);
	int getTypename(std::string attrName) const;
};

#endif