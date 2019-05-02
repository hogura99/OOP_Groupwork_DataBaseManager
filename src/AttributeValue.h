#pragma once

#ifndef ATTRIBUTEVALUE_H
#define ATTRIBUTEVALUE_H

#include "Value.h"
#include <string>

template<class T>
class AttributeValue : public Value{
    T _attrValue;
    public:
        AttributeValue(T attrValue): _attrValue(attrValue){
            if(typeid(int).name() == typeid(T).name()){
                _typeName = INT;
            }
            else if(typeid(std::string).name() == typeid(T).name()){
                _typeName = STRING;
            }
            else if(typeid(double).name() == typeid(T).name()){
                _typeName = DOUBLE;
            }
        }
        ~AttributeValue(){}

        AttributeValue<T>* setValue(T attrValue);
        T getValue() const;
        int getTypename() const override;
        bool operator==(const AttributeValue<T>& b) const;
        bool operator!=(const AttributeValue<T>& b) const;
        bool operator>(const AttributeValue<T>& b) const;
        bool operator<(const AttributeValue<T>& b) const;
        bool operator>=(const AttributeValue<T>& b) const;
        bool operator<=(const AttributeValue<T>& b) const;
};
/*
class ValueDouble : public Value{
    double Value;
    public:
        ValueDouble(double v): Value(v){}
        ~ValueDouble();

        ValueDouble* setValue(double v);
        double getValue() const;
        int getTypename() const override;
        bool operator==(const ValueDouble& b) const;
        bool operator!=(const ValueDouble& b) const;
        bool operator>(const ValueDouble& b) const;
        bool operator<(const ValueDouble& b) const;
        bool operator>=(const ValueDouble& b) const;
        bool operator<=(const ValueDouble& b) const;
};

class ValueString : public Value{
    std::string Value;
    public:
        ValueString(std::string v): Value(v) {}
        ~ValueString();
        
        ValueString* setValue(std::string v);
        std::string getValue() const;
        int getTypename() const override;
        bool operator==(const ValueString& b) const;
        bool operator!=(const ValueString& b) const;
        
};
*/
//following funcs need to be called with dynamic_cast;
//getValue
//operator maybe

#endif