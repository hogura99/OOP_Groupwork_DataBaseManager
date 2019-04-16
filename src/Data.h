#pragma once

#include "Derived.h"

#include <string>

class Data{
	std::vector<std::pair<std::string, Base> > vec;
public:
	Data();
	~Data();
	
	Base* getData(std::string attrName);

	Base* setData(std::string attrName, int value);
	Base* setData(std::string attrName, double value);
	Base* setData(std::string attrName, std::string value);
	
	std::string getTypename(std::string attrName); 
}