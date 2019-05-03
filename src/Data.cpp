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

	Value* copy_src;
	if (src->getTypename() == INT){
		AttributeValue<int>* tmp_ptr = dynamic_cast<AttributeValue<int>*>(src);
		copy_src = new AttributeValue<int>(*tmp_ptr);
	}
	else if(src->getTypename() == STRING){
		AttributeValue<std::string>* tmp_ptr = dynamic_cast<AttributeValue<std::string>*>(src);
		copy_src = new AttributeValue<std::string>(*tmp_ptr);
	}
	else if (src->getTypename() == DOUBLE){
		AttributeValue<double>* tmp_ptr = dynamic_cast<AttributeValue<double>*>(src);
		copy_src = new AttributeValue<double>(*tmp_ptr);
	}

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