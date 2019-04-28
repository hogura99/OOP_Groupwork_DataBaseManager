#pragma once

#ifndef BASE_H
#define BASE_H
#include <string>


enum attrType
{
    NONE_TYPE,
    INT,
    DOUBLE,
    STRING
};

class Base{
private:
	virtual bool __compare(const Base *val_l, const Base *val_r, int type, int opr) const;
protected:
	int _typeName;
public:
	virtual Base() {_typeName = NONE_TYPE;}
    virtual ~Base(){}
    virtual Base* setData(){return NULL;}
    virtual int getTypename();

    bool operator==(const Base& b) const;
    bool operator!=(const Base& b) const;
    bool operator>(const Base& b) const;
    bool operator<(const Base& b) const;
    bool operator>=(const Base& b) const;
    bool operator<=(const Base& b) const;    
}
};

#endif