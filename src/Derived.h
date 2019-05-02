#pragma once

#ifndef DERIVED_H
#define DERIVED_H

#include "Value.h"
#include <string>


class dataInt : public Value{
    int data;
    public:
        dataInt(int v): data(v){}
        ~dataInt();

        dataInt* setData(int v);
        int getValue() const;
        int getTypename() const override;

        bool operator==(const dataInt& b) const;
        bool operator!=(const dataInt& b) const;
        bool operator>(const dataInt& b) const;
        bool operator<(const dataInt& b) const;
        bool operator>=(const dataInt& b) const;
        bool operator<=(const dataInt& b) const;    
};

class dataDouble : public Value{
    double data;
    public:
        dataDouble(double v): data(v){}
        ~dataDouble();

        dataDouble* setData(double v);
        double getValue() const;
        int getTypename() const override;
        bool operator==(const dataDouble& b) const;
        bool operator!=(const dataDouble& b) const;
        bool operator>(const dataDouble& b) const;
        bool operator<(const dataDouble& b) const;
        bool operator>=(const dataDouble& b) const;
        bool operator<=(const dataDouble& b) const;
};

class dataString : public Value{
    std::string data;
    public:
        dataString(std::string v): data(v) {}
        ~dataString();
        
        dataString* setData(std::string v);
        std::string getValue() const;
        int getTypename() const override;
        bool operator==(const dataString& b) const;
        bool operator!=(const dataString& b) const;
        
};

//following funcs need to be called with dynamic_cast;
//getValue
//operator maybe

#endif