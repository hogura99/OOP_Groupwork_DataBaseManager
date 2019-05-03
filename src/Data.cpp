#include "Data.h"

Value* Data::getValue(std::string attrName) const{
	auto fd = varMap.find(attrName);
	if(fd != varMap.end()){
		return fd->second;
	}
	else{
		return NULL;
	}
}
//传入Value* src之后是复制还是直接放?
Value* Data::setValue(std::string attrName, Value* src){
	
	auto fd = varMap.find(attrName);

	Value* copy_src = src->getCopy();

	if(fd != varMap.end()){//update
		delete fd->second;
		fd->second = copy_src;
	}
	else{//insert
		varMap.insert(std::make_pair(attrName, copy_src));
	}

	return varMap[attrName];
}

int Data::getTypename(std::string attrName) const{
	auto fd = varMap.find(attrName);
	
	if(fd != varMap.end()){
		return fd->second->getTypename();
	}
	else{
		return -1;
	}
}