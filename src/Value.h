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

class Value{
protected:
    int _typeName;

	virtual bool __compare(const Value *val_l, const Value *val_r, int type, int opr) const;
    
public:
    static std::map<std::string, int> attrTypeMap;
    static std::map<int, std::string> attrTypeInvMap;
    static std::map<int, int> attrTypeWidth;

	Value() {_typeName = NONE_TYPE;}
    Value(const Value& src) {_typeName = src.getTypename();}
    
    virtual ~Value(){/* std::cerr << "delete Value -->   " << this << std::endl; */}
    virtual Value* setValue(){return NULL;}
    virtual Value* getCopy();
    virtual Value* transValue(std::string StrVal, int dataType);
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