#pragma once

#include "Base.h"

#include <string>

class dataInt : public Base{
    int data;
    public:
        dataInt();
        ~dataInt();

        dataInt* setData(int value) override;
        int getData();
        std::string getTypename() override;
};

class dataDouble : public Base{
    double data;
    public:
        dataDouble();
        ~dataDouble();

        dataDouble* setData(double value) override;
        double getData();
        std::string getTypename() override;
};

class dataString : public Base{
    std::string data;
    public:
        dataString();
        ~dataString();
        
        dataString* setData(std::string value) override;
        std::string getData();
        std::string getTypename() override;
};
