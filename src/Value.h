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

class Value{
private:
	virtual bool __compare(const Value *val_l, const Value *val_r, int type, int opr) const;
protected:
	int _typeName;
public:
	Value() {_typeName = NONE_TYPE;}
    virtual ~Value(){}
    virtual Value* setValue(){return NULL;}
    virtual int getTypename() const;

    bool operator==(const Value& b) const;
    bool operator!=(const Value& b) const;
    bool operator>(const Value& b) const;
    bool operator<(const Value& b) const;
    bool operator>=(const Value& b) const;
    bool operator<=(const Value& b) const;
    friend std::ostream& operator<<(std::ostream& out, Value& b);
};

#endif