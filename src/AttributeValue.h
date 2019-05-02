#pragma once

#ifndef ATTRIBUTEVALUE_H
#define ATTRIBUTEVALUE_H

#include "Value.h"
#include <string>
#include <typeinfo>

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

        const AttributeValue<T>*& setValue(T attrValue) ;
        T getValue() const;
        int getTypename() const override;
        bool operator==(const AttributeValue<T>& b) const;
        bool operator!=(const AttributeValue<T>& b) const;
        bool operator>(const AttributeValue<T>& b) const;
        bool operator<(const AttributeValue<T>& b) const;
        bool operator>=(const AttributeValue<T>& b) const;
        bool operator<=(const AttributeValue<T>& b) const;
};

#endif