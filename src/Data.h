#pragma once

#ifndef DATA_H
#define DATA_H
#include "Derived.h"

#include <string>
#include <map>

class Data{
	std::map<std::string, Base* > varMap;
public:
	Data();
	~Data();
	
	Base* getData(std::string attrName) const;
	Base* setData(std::string attrName, Base* src);
	int getTypename(std::string attrName);
};

#endif