#include "Value.h"
#include "str_algorithm.h"
#include "AttributeValue.h"

#include <assert.h>

std::map<std::string, int> Value::attrTypeMap =
{
	{"INT", INT},
	{"DOUBLE", DOUBLE},
	{"CHAR", STRING},
};

std::map<int, std::string> Value::attrTypeInvMap = 
{
    {INT, "int"},
    {DOUBLE, "double"},
    {STRING, "char"},
};

std::map<int, int> Value::attrTypeWidth = 
{
    {INT, 11},
    {DOUBLE, 0},
    {STRING, 1},
};

#define SWITCH_OPRT(pt_l, pt_r, opr) {						\
	switch (opr) {											\
		case opLE: res = (*pt_l)< (*pt_r); break;			\
		case opEQ: res = (*pt_l)==(*pt_r); break;			\
	}														\
}

Value* Value::transValue(std::string StrVal, int dataType)
{
	Value *pt = NULL;
	switch (dataType)
	{
		case INT :
		{
			int val = 0;
			if (stralgo::str2int(StrVal, val))
				pt = new AttributeValue<int>(val);
			break;
		}
		case DOUBLE :
		{
			double val = 0;
			if (stralgo::str2double(StrVal, val))
				pt = new AttributeValue<double>(val);
			break;
		}
		case STRING :
		{
			pt = new AttributeValue<std::string>(StrVal);
			break;
		}
	}

	return pt;
}

bool Value::__compare(const Value *val_l, const Value *val_r, int type, int opr) const {
	Value *_val_l = const_cast<Value*>(val_l);
	Value *_val_r = const_cast<Value*>(val_r);
	bool res = false;
	switch (type) {
		case INT: {
			AttributeValue<int> *pt_l = dynamic_cast<AttributeValue<int>*>(_val_l);
			AttributeValue<int> *pt_r = dynamic_cast<AttributeValue<int>*>(_val_r);
			//assert(pt_l != NULL && pt_r != NULL);
			SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
		case DOUBLE: {
			const AttributeValue<double> *pt_l = dynamic_cast<AttributeValue<double>*>(_val_l);
			const AttributeValue<double> *pt_r = dynamic_cast<AttributeValue<double>*>(_val_r);
			//assert(pt_l != NULL && pt_r != NULL);
			SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
		case STRING: {
			const AttributeValue<std::string> *pt_l = dynamic_cast<AttributeValue<std::string>*>(_val_l);
			const AttributeValue<std::string> *pt_r = dynamic_cast<AttributeValue<std::string>*>(_val_r);
			//assert(pt_l != NULL && pt_r != NULL);
			SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
	}
	return res;
}

#undef SWITCH_OPRT

int Value::getTypename() const {
	return _typeName;
}

Value* Value::getCopy() {
	Value* copy_src = NULL;
	switch (_typeName) {
		case INT: {
			AttributeValue<int>* tmp_ptr = dynamic_cast<AttributeValue<int>*>(this);
			copy_src = new AttributeValue<int>(*tmp_ptr);
			break;
		}
		case STRING: {
			AttributeValue<std::string>* tmp_ptr = dynamic_cast<AttributeValue<std::string>*>(this);
			copy_src = new AttributeValue<std::string>(*tmp_ptr);
			break;
		}
		case DOUBLE: {
			AttributeValue<double>* tmp_ptr = dynamic_cast<AttributeValue<double>*>(this);
			copy_src = new AttributeValue<double>(*tmp_ptr);
			break;
		}
	}
	return copy_src;
}

bool Value::operator<(const Value& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return __compare(this, &b, getTypename(), opLE);
}

bool Value::operator==(const Value& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return __compare(this, &b, getTypename(), opEQ);
}

bool Value::operator!=(const Value& b) const{
	return !b.operator==(*this);
}

bool Value::operator>(const Value& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return !(operator<(b) || operator==(b));
}

bool Value::operator>=(const Value& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return !operator<(b);
}

bool Value::operator<=(const Value& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return !operator>(b);
}

std::ostream& operator<<(std::ostream& out, Value &b) {
	switch (b.getTypename())
	{
		case INT:
		{
			AttributeValue<int> &val = dynamic_cast<AttributeValue<int>&>(b);
			out << val.getValue();
			break;
		}
		case STRING:
		{
			AttributeValue<std::string> &val = dynamic_cast<AttributeValue<std::string>&>(b);
			std::string tmp = val.getValue();
			if (tmp.length() > 2)
				out << tmp.substr(1, tmp.length() - 2);
			else
				out << "NULL";
			break;
		}
		case DOUBLE:
		{
			AttributeValue<double> &val = dynamic_cast<AttributeValue<double>&>(b);
			out << val.getValue();
			break;
		}
	}
	return out;
}