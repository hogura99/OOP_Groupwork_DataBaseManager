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
	
	virtual Value* getValue(std::string attrName) const;
	virtual Value* setValue(std::string attrName, Value* src);
	virtual int getTypename(std::string attrName) const;
};

#endif