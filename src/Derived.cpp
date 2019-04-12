#include "Derived.h"

dataInt* dataInt::setData(int v){
	data = v;
	return this;
}

int dataInt::getValue(){
	return data;
}

std::string dataInt::getTypename(){
	return "int";
}

dataDouble* dataDouble::setData(double v){
	data = v;
	return this;
}

double dataDouble::getValue(){
	return data;
}

std::string dataDouble::getTypename(){
	return "double";
}

dataString* dataString::setData(std::string v){
	data = v;
	return this;
}

std::string dataString::getValue(){
	return data;
}

std::string dataString::getTypename(){
	return "string";
}



