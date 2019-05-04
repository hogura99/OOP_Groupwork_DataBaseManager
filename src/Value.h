#pragma once

#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <map>

#include "expression.h"

enum attrType
{
    NONE_TYPE,
    INT,
    DOUBLE,
    STRING
};

const std::map<std::string, int> attrTypeMap =
{
	{"INT", INT},
	{"DOUBLE", DOUBLE},
	{"CHAR", STRING},
};

const std::map<int, std::string> attrTypeInvMap = 
{
    {INT, "int"},
    {DOUBLE, "double"},
    {STRING, "char"},
};

const std::map<int, int> attrTypeWidth = 
{
    {INT, 11},
    {DOUBLE, 0},
    {STRING, 1},
};

class Value{
protected:
    int _typeName;

	virtual bool __compare(const Value *val_l, const Value *val_r, int type, int opr) const;
    
public:
	Value() {_typeName = NONE_TYPE;}
    Value(const Value& src) {_typeName = src.getTypename();}
    
    virtual ~Value(){ std::cerr << "delete Value -->   " << this << std::endl; }
    virtual Value* setValue(){return NULL;}
    virtual Value* getCopy();
    int getTypename() const;

    bool operator==(const Value& b) const;
    bool operator!=(const Value& b) const;
    bool operator>(const Value& b) const;
    bool operator<(const Value& b) const;
    bool operator>=(const Value& b) const;
    bool operator<=(const Value& b) const;
    friend std::ostream& operator<<(std::ostream& out, Value& b);
};

#endif