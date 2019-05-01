#pragma once

#ifndef BASE_H
#define BASE_H

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

class Base{
private:
	virtual bool __compare(const Base *val_l, const Base *val_r, int type, int opr) const;
protected:
	int _typeName;
public:
	Base() {_typeName = NONE_TYPE;}
    virtual ~Base(){}
    virtual Base* setData(){return NULL;}
    virtual int getTypename() const;

    bool operator==(const Base& b) const;
    bool operator!=(const Base& b) const;
    bool operator>(const Base& b) const;
    bool operator<(const Base& b) const;
    bool operator>=(const Base& b) const;
    bool operator<=(const Base& b) const;   
    friend std::ostream& operator<<(std::ostream& out, Base& b);
};

#endif