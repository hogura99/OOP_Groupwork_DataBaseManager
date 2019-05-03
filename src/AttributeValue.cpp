#include "AttributeValue.h"


template<class T>
AttributeValue<T>* AttributeValue<T>::setValue(T attrValue) {
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
