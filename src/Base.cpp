#include "Base.h"
#include "Derived.h"

#include <assert.h>

#define SWITCH_OPRT(pt_l, pt_r, opr) {						\
	switch (opr) {											\
		case opLE: res = pt_l->operator<(*pt_r); break;	\
		case opEQ: res = pt_l->operator==(*pt_r); break;	\
	}														\
}

bool Base::__compare(const Base *val_l, const Base *val_r, int type, int opr) const {
	Base *_val_l = const_cast<Base*>(val_l);
	Base *_val_r = const_cast<Base*>(val_r);
	bool res = false;
	switch (type) {
		case INT: {
			//assert(dynamic_cast<dataInt*>(val_l) != NULL && dynamic_cast<dataInt*>(val_r) != NULL);
			const dataInt *pt_l = dynamic_cast<dataInt*>(_val_l);
			const dataInt *pt_r = dynamic_cast<dataInt*>(_val_r);
			assert(pt_l != NULL && pt_r != NULL);
			SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
		case DOUBLE: {
			const dataDouble *pt_l = dynamic_cast<dataDouble*>(_val_l);
			const dataDouble *pt_r = dynamic_cast<dataDouble*>(_val_r);
			assert(pt_l != NULL && pt_r != NULL);
			SWITCH_OPRT(pt_l, pt_r, opr);
			break;
		}
		case STRING: {
			const dataString *pt_l = dynamic_cast<dataString*>(_val_l);
			const dataString *pt_r = dynamic_cast<dataString*>(_val_r);
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

int Base::getTypename() const {
	return NONE_TYPE;
}

bool Base::operator<(const Base& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return __compare(this, &b, getTypename(), opLE);
}

bool Base::operator==(const Base& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return __compare(this, &b, getTypename(), opEQ);
}

bool Base::operator!=(const Base& b) const{
	return !b.operator==(*this);
}

bool Base::operator>(const Base& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return !(operator<(b) || operator==(b));
}

bool Base::operator>=(const Base& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return !operator<(b);
}

bool Base::operator<=(const Base& b) const{
	if (getTypename() != b.getTypename())
		return false;
	else
		return !operator>(b);
}

std::ostream& operator<<(std::ostream& out, Base &b) {
	switch (b.getTypename())
	{
		case INT:
		{
			dataInt &val = dynamic_cast<dataInt&>(b);
			out << val.getValue();
			break;
		}
		case STRING:
		{
			dataString &val = dynamic_cast<dataString&>(b);
			std::string tmp = val.getValue();
			if (tmp.length() > 2)
				out << tmp.substr(1, tmp.length() - 2);
			else
				out << "NULL";
			break;
		}
		case DOUBLE:
		{
			dataDouble &val = dynamic_cast<dataDouble&>(b);
			out << val.getValue();
			break;
		}
	}
	return out;
}