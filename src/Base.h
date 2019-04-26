#pragma once

#ifndef BASE_H
#define BASE_H
#include <string>


class Base{
    public:
        virtual ~Base(){}
        virtual Base* setData(){return NULL;}
        virtual int getTypename(){return -1;}
};

#endif