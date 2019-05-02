#include "AttributeValue.h"


template<class T>
const AttributeValue<T>*& AttributeValue<T>::setValue(T attrValue) const{
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

/*
_attrValueDouble* _attrValueDouble::set_attrValue(double v){
	_attrValue = v;
	return this;
}

double _attrValueDouble::getValue()const{
	return _attrValue;
}

int _attrValueDouble::getTypename() const {
	return DOUBLE;
}
bool _attrValueDouble::operator==(const _attrValueDouble& b) const{
	return _attrValue == b.getValue();
}
bool _attrValueDouble::operator!=(const _attrValueDouble& b) const{
	return _attrValue != b.getValue();
}
bool _attrValueDouble::operator>(const _attrValueDouble& b) const{
	return _attrValue > b.getValue();
}
bool _attrValueDouble::operator<(const _attrValueDouble& b) const{
	return _attrValue < b.getValue();
}
bool _attrValueDouble::operator>=(const _attrValueDouble& b) const{
	return _attrValue >= b.getValue();
}
bool _attrValueDouble::operator<=(const _attrValueDouble& b) const{
	return _attrValue <= b.getValue();
}

_attrValueString* _attrValueString::set_attrValue(std::string v){
	_attrValue = v;
	return this;
}

std::string _attrValueString::getValue() const{
	return _attrValue;
}

int _attrValueString::getTypename() const {
	return STRING;
}
bool _attrValueString::operator==(const _attrValueString& b) const{
	return _attrValue == b.getValue();
}
bool _attrValueString::operator!=(const _attrValueString& b) const{
	return _attrValue != b.getValue();
}*/