#include "Derived.h"

dataInt::~dataInt(){

}
dataDouble::~dataDouble(){

}
dataString::~dataString(){
	
}
dataInt* dataInt::setData(int v){
	data = v;
	return this;
}

int dataInt::getValue()const{
	return data;
}

int dataInt::getTypename(){
	return INT;
}
bool dataInt::operator==(const dataInt& b) const{
	return data == b.getValue();
}
bool dataInt::operator!=(const dataInt& b) const{
	return data != b.getValue();
}
bool dataInt::operator>(const dataInt& b) const{
	return data > b.getValue();
}
bool dataInt::operator<(const dataInt& b) const{
	return data < b.getValue();
}
bool dataInt::operator>=(const dataInt& b) const{
	return data >= b.getValue();
}
bool dataInt::operator<=(const dataInt& b) const{
	return data <= b.getValue();
}
dataDouble* dataDouble::setData(double v){
	data = v;
	return this;
}

double dataDouble::getValue()const{
	return data;
}

int dataDouble::getTypename(){
	return DOUBLE;
}
bool dataDouble::operator==(const dataDouble& b) const{
	return data == b.getValue();
}
bool dataDouble::operator!=(const dataDouble& b) const{
	return data != b.getValue();
}
bool dataDouble::operator>(const dataDouble& b) const{
	return data > b.getValue();
}
bool dataDouble::operator<(const dataDouble& b) const{
	return data < b.getValue();
}
bool dataDouble::operator>=(const dataDouble& b) const{
	return data >= b.getValue();
}
bool dataDouble::operator<=(const dataDouble& b) const{
	return data <= b.getValue();
}

dataString* dataString::setData(std::string v){
	data = v;
	return this;
}

std::string dataString::getValue() const{
	return data;
}

int dataString::getTypename(){
	return STRING;
}
bool dataString::operator==(const dataString& b) const{
	return data == b.getValue();
}
bool dataString::operator!=(const dataString& b) const{
	return data != b.getValue();
}