#include "Data.h"

Value* Data::getData(std::string attrName) const {
	auto fd = varMap.find(attrName);
	if(fd != varMap.end()){
		return fd->second;
	}
	else{
		return NULL;
	}
}
//传入Element* src之后是复制还是直接放?
Value* Data::setData(std::string attrName, Value* src){
	
	auto fd = varMap.find(attrName);

	if(fd != varMap.end()){//update
		delete fd->second;
		fd->second = src;
	}
	else{//insert
		varMap.insert(std::make_pair(attrName, src));
	}

	return varMap[attrName];
}

int Data::getTypename(std::string attrName){
	auto fd = varMap.find(attrName);
	
	if(fd != varMap.end()){
		return fd->second->getTypename();
	}
	else{
		return -1;
	}
}