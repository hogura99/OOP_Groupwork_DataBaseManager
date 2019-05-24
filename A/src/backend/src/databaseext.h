#pragma once

#include "database.h"

class DatabaseExt: public Database
{
public:
    DatabaseExt();
    virtual ~DatabaseExt() = default;
};