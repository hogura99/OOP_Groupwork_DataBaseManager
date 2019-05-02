#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "ParamSpliter.h"
#include "expression.h"
#include "Data.h"

//name and the corresponding value of the attribute
#define ATTRIBUTE std::pair<std::string, Value*>

class DataTable
{
    private:
        
        //the Data in the DataTable
        std::vector<Data*> mData_;

        const std::string table_name_;
        
        //the name of the primary key
        const std::string primary_key_;

        //tells whether an attribute is a not null key.
        std::map<std::string, bool> not_null_key_;

        //records the names and types of the attributes.
        std::map<std::string, int> attribute_table_;

    protected:
        void __PopStack(std::stack<bool> &val, std::stack<int> &opr);
        bool checkSingleClause(const Data* it, const std::vector<std::string> &_param);
        bool calcExpr(const Data* it, const std::string &clause);
        Value* transValue(const Data* _attr, std::string val, int dataType);

    public:
        //@param table_name: the name of the table.
        //@param attribute_table: a vector containing the names and the types of the attributes. i.e. < <stu_name, STRING>, <stu_id, INT> >
        //@param primary_key: the name of the primary key.
        //@param not_null_key: the names of the not null key.
        DataTable(const std::string& table_name, std::vector< std::pair<std::string, int> >& attribute_table,
          const std::string& primary_key, const std::vector<std::string>& not_null_key);

        //delete the DataTable and the Data inside it.
        ~DataTable();

        //insert a data into the table.
        //@param attributes: names and the corresponding value of the attributes
        void Insert(const std::vector< ATTRIBUTE >& attributes);

        //remove the chosen data.
        //@param data_list: the position of the data to be removed.
        void Remove(std::vector<Data*> &data_list);

        //update the chosen data's single attribute
        void Update(const ATTRIBUTE &attribute, std::vector<Data*> &data_list);

        //get the value (corresponding to the name) of the chosen data.
        //@param attribute_value: the returned value.
        void Select(const std::string &attribute_name, const std::vector<Data*> &data_list, std::vector<Value*> &attribute_value);

        //get the position of the data satisfying the clause.
        //@param clause: the string after the "where".
        //@param data_list: the position of the chosen data (pointer)
        void GetDataWhere(const std::string &clause, std::vector<Data*> &data_list);
        
        int GetTypeof(const std::string &attrName);

        friend bool checkLegality(DataTable _DataTable, const std::vector<ATTRIBUTE>& attributes);

        std::map<std::string, int>& GetAttributeTable();
        
        virtual void PrintAttributeTable();
};