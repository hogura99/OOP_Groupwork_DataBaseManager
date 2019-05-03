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
        AttributeValue(T attrValue):Value(), _attrValue(attrValue){
            if(typeid(int) == typeid(T)){
                _typeName = INT;
            }
            else if(typeid(std::string) == typeid(T)){
                _typeName = STRING;
            }
            else if(typeid(double) == typeid(T)){
                _typeName = DOUBLE;
            }
        }

        ~AttributeValue() {
            std::cerr << "delete Value(" << attrTypeInvMap.at(_typeName) << ") -->   " << this << std::endl;
        }
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