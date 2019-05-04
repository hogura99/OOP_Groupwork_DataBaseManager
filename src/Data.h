#pragma once

#ifndef DATA_H
#define DATA_H
#include "AttributeValue.h"

#include <string>
#include <map>

/*
 ********************************************************
 *						definition						*
 ********************************************************
*/

template<class Value = Value>
class Data{
	std::map<std::string, Value* > varMap;
public:
	Data() {}
	~Data() {}
	
	// Get the value of an attribute.
	// @param attrName: the name of the chosen attribute.
	virtual Value* getValue(std::string attrName) const;

	// Set the value of an attribute.
	// @param attrName: the name of the chosen attribute.
	// @param src: the value to set.
	virtual Value* setValue(std::string attrName, Value* src);

	// Get the data type of an attribute.
	// @param attrName: the name of the chosen attribute.
	virtual int getTypename(std::string attrName) const;
};


/*
 ********************************************************
 *					implementation						*
 ********************************************************
*/

template<class Value>
Value* Data<Value>::getValue(std::string attrName) const {
	auto fd = varMap.find(attrName);
	if(fd != varMap.end()) {
		return fd->second;
	}
	else{
		return NULL;
	}
}

template<class Value>
Value* Data<Value>::setValue(std::string attrName, Value* src) {
	
	auto fd = varMap.find(attrName);

	Value* copy_src = src->getCopy();

	if(fd != varMap.end()) {//update
		delete fd->second;
		fd->second = copy_src;
	}
	else {//insert
		varMap.insert(std::make_pair(attrName, copy_src));
	}

	return varMap[attrName];
}

template<class Value>
int Data<Value>::getTypename(std::string attrName) const {
	auto fd = varMap.find(attrName);
	
	if(fd != varMap.end()) {
		return fd->second->getTypename();
	}
	else {
		return -1;
	}
}

#endif