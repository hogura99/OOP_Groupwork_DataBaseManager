#include "Data.h"

Base* Data::getData(std::string attrName){
	auto fd = varMap.find(attrName);
	if(fd != varMap.end()){
		return fd->second;
	}
	else{
		return NULL;
	}
}

Base* Data::setData(std::string attrName, Base* src){
	auto fd = varMap.find(attrName);
	if(fd != varMap.end()){//update
		delete fd->second;
		fd->second = src;
	}
	else{//insert
		varMap.insert(std::make_pair(attrName, src));
	}
}

std::string getTypename(std::attrName){
	auto fd = varMap.find(attrName);
	if(fd != varMap.end()){
		return fd->second->getTypename();
	}
	else{
		return "NULL";
	}
}