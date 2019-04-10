#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Data.h"

typedef std::pair<string, Base> attribute;

class DataTable
{
    private:
        std::vector<Data> mData;
        const std::string tableName;
        const std::string primaryKey;
        const std::vector<string> notNullKey;
        const std::map<string, attrType> attrTable;
    public:
        enum attrType
        {
            INT;
            DOUBLE;
            STRING;
        };
        Table(const std::string &_attrName, std::vector< pair<string, string> > attrType, const std::string &primaryKey, const std::vector<string> &notNullKey);
        ~Table();
        void insert(const std::vector< attribute > &attributes);
        void remove(std::list<Data*> &dataList);
        void update(const attribute , std::list<Data*> &dataList);
        void select(const std::string &attrName, std::vector<attribute> &attrList, const std::list<Data*> &dataList)
        void getDataWhere(const std::string &clause, std::list<Data*> &dataList);
};