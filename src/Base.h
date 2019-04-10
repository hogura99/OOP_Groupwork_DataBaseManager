#pragma once

#include <string>

class Base{
    public:
        virtual ~Base() = 0;
        virtual Base* setData() = 0;
        virtual std::string getTypename() = 0;
}
