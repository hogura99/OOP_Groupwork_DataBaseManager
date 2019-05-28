#pragma once

#include "database.h"

class DatabaseExt: public Database
{
public:
    DatabaseExt();
    virtual ~DatabaseExt() = default;

    void load(const std::string &tableName, const std::vector< std::map<std::string, Variant> > &entries);
};