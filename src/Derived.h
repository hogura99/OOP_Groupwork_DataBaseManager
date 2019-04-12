#pragma once

#ifndef DERIVED_H
#define DERIVED_H

#include "Base.h"
#include <string>

class dataInt : public Base{
    int data;
    public:
        dataInt(int v): data(v){};
        ~dataInt();

        dataInt* setData(int v) override;
        int getValue();
        std::string getTypename() override;
}

class dataDouble : public Base{
    double data;
    public:
        dataDouble(double v): data(v){};
        ~dataDouble();

        dataDouble* setData(double v) override;
        double getValue();
        std::string getTypename() override;
}

class dataString : public Base{
    std::string data;
    public:
        dataString(std::string v): data(v) {};
        ~dataString();
        
        dataString* setData(std::string v) override;
        std::string getValue();
        std::string getTypename() override;
}

//following funcs need to be called with dynamic_cast;
//getValue
//operator maybe

#endif