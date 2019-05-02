#include "Value.h"
#include "AttributeValue.h"

#include <assert.h>

#define SWITCH_OPRT(pt_l, pt_r, opr) {						\
	switch (opr) {											\
		case opLE: res = pt_l->operator<(*pt_r); break;	\
		case opEQ: res = pt_l->operator==(*pt_r); break;	\
	}														\
}

bool Value::__compare(const Value *val_l, const Value *val_r, int type, int opr) const {
	Value *_val_l = const_cast<Value*>(val_l);
	Value *_val_r = const_cast<Value*>(val_r);
	bool res = false;
	switch (type) {
		case INT: {
			//assert(dynamic_cast<AttributeValue<int>*>(val_l) != NULL && dynamic_cast<AttributeValue<int>*>(val_r) != NULL);
			const AttributeValue<int> *pt_l = dynamic_cast<AttributeValue<int>*>(_val_l);
			const AttributeValue<int> *pt_r = dynamic_cast<AttributeValue<int>*>(_val_r);
			assert(pt_l != NULL && pt_r != NULL);
			SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
		case DOUBLE: {
			const AttributeValue<double> *pt_l = dynamic_cast<AttributeValue<double>*>(_val_l);
			const AttributeValue<double> *pt_r = dynamic_cast<AttributeValue<double>*>(_val_r);
			assert(pt_l != NULL && pt_r != NULL);
			SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
		case STRING: {
			const AttributeValue<std::string> *pt_l = dynamic_cast<AttributeValue<std::string>*>(_val_l);
			const AttributeValue<std::string> *pt_r = dynamic_cast<AttributeValue<std::string>*>(_val_r);
			assert(pt_l != NULL && pt_r != NULL);
			if (opr == opEQ)
				res = pt_l->operator==(*pt_r);
			else {
				assert(false);
			}
			//SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
	}
	//assert(false);
	return res;
}

int Value::getTypename() const {
	return NONE_TYPE;
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