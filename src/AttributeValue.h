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
        const AttributeValue<T>* setValue(T attrValue) ;
        T getValue() const;
        int getTypename() const;
        bool operator==(const AttributeValue<T>& b) const;
        bool operator!=(const AttributeValue<T>& b) const;
        bool operator>(const AttributeValue<T>& b) const;
        bool operator<(const AttributeValue<T>& b) const;
        bool operator>=(const AttributeValue<T>& b) const;
        bool operator<=(const AttributeValue<T>& b) const;
};

template<class T>
const AttributeValue<T>* AttributeValue<T>::setValue(T attrValue) {
    _attrValue = attrValue;
    return this;
}
template<class T>
T AttributeValue<T>::getValue() const{
    return _attrValue;
}
template<class T>
int AttributeValue<T>::getTypename() const {
    return _typeName;
}
template<class T>
bool AttributeValue<T>::operator==(const AttributeValue<T>& b) const{
    return _attrValue == b.getValue();
}
template<class T>
bool AttributeValue<T>::operator!=(const AttributeValue<T>& b) const{
    return _attrValue != b.getValue();
}
template<class T>
bool AttributeValue<T>::operator>(const AttributeValue<T>& b) const{
    return _attrValue > b.getValue();
}
template<class T>
bool AttributeValue<T>::operator<(const AttributeValue<T>& b) const{
    return _attrValue < b.getValue();
}
template<class T>
bool AttributeValue<T>::operator>=(const AttributeValue<T>& b) const{
    return _attrValue >= b.getValue();
}
template<class T>
bool AttributeValue<T>::operator<=(const AttributeValue<T>& b) const{
    return _attrValue <= b.getValue();
}


#endif