#include "databaseext.h"

void DatabaseExt::load(const std::string &tableName, const std::vector< std::map<std::string, Variant> > &entries)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    for (size_t i = 0; i < entries.size(); i++)
    {
        _tables[tableName].insertInto(entries[i]);
    }
}