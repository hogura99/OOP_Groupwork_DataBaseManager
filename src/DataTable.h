#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Data.h"
#define ATTRIBUTE std::pair<std::string, Base>

//typedef std::pair<std::string, Base> ATTRIBUTE;

class DataTable
{
    private:
        std::list<Data*> mData;  //是否要改为指针？
        const std::string tableName;
        const std::string primaryKey;
        const std::vector<std::string> notNullKey;
        const std::map<std::string, int> attrTable;
    public:
        enum attrType
        {
            INT,
            DOUBLE,
            STRING
        };
        DataTable(const std::string &_attrName, std::vector< std::pair<std::string, std::string> > attrType, const std::string &primaryKey, const std::vector<std::string> &notNullKey);
        ~DataTable();
        void insert(const std::vector< ATTRIBUTE > &attributes);
        //注意实现主键是否重复
        void remove(std::list<Data*> &dataList);
        void update(const ATTRIBUTE &attributes, std::list<Data*> &dataList);
        void select(const std::string &attrName, std::vector<Base*> &attrList, const std::list<Data*> &dataList);
        void getDataWhere(const std::string &clause, std::list<Data*> &dataList);
        friend bool checkLegality(DataTable _DataTable, const std::vector<ATTRIBUTE>& attributes);
};