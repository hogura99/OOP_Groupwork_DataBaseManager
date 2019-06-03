#include "databaseext.h"

QueryResult DatabaseExt::selectAllFrom(const std::string &tableName, const std::vector<std::string> &fieldNames, const Expr &expr, const std::string* file_name)
{
    std::vector<std::string> keyNames = fieldNames;
    std::reverse(keyNames.begin(), keyNames.end());
    keyNames.pop_back();
    std::reverse(keyNames.begin(), keyNames.end()); // delete the first '*' of keynames

    std::vector<std::string> _keyNames;
    std::vector<std::string> _cntNames;
    std::vector<size_t> _cntPos;

    getCountNames(keyNames, _keyNames, _cntNames, _cntPos);

    auto selectResult = dynamic_cast<QueryResultSelect*>(Database::selectAllFrom(tableName, expr).result());

    std::vector<Entry> _entries = selectResult->entries();
    std::vector<Entry> _resultEntries(_entries.size());
    countEntries(_keyNames, _cntNames, _cntPos, _entries, _resultEntries);

    delete selectResult;
    auto queryResult = new QueryResultSelectInto(keyNames, _resultEntries, file_name);

    return QueryResult(queryResult);
}

QueryResult DatabaseExt::selectFrom(const std::string &tableName, const std::vector<std::string> &keyNames, const Expr &expr, const std::string* file_name)
{
    std::vector<std::string> _keyNames;
    std::vector<std::string> _cntNames;
    std::vector<size_t> _cntPos;

    getCountNames(keyNames, _keyNames, _cntNames, _cntPos);

    auto selectResult = dynamic_cast<QueryResultSelect*>(Database::selectFrom(tableName, _keyNames, expr).result());

    std::vector<Entry> _entries = selectResult->entries();
    std::vector<Entry> _resultEntries(_entries.size());
    countEntries(_keyNames, _cntNames, _cntPos, _entries, _resultEntries);

    delete selectResult;
    auto queryResult = new QueryResultSelectInto(keyNames, _resultEntries, file_name);

    return QueryResult(queryResult);
}

void DatabaseExt::getCountNames(const std::vector<std::string> &keyNames,
                   std::vector<std::string> &_keyNames,
                   std::vector<std::string> &_cntNames,
                   std::vector<size_t> &_cntPos)
{
    const int COUNT_POS = 7;

    for (int i = 0; i < keyNames.size(); i ++)
        if (keyNames[i][0] == '\\')
        {                                               // current key name is a counting one
            _cntPos.push_back(_keyNames.size());
            _cntNames.push_back(keyNames[i].substr(COUNT_POS)); // get the counting names
        }
        else
        {
            _keyNames.push_back(keyNames[i]);
        }
}

void DatabaseExt::countEntries(const std::vector<std::string> &keyNames,
                  const std::vector<std::string> &cntNames,
                  const std::vector<size_t> &cntPos,
                  const std::vector<Entry> &entries,
                  std::vector<Entry> &resultEntries)

{
    int i = 0;
    int pos = i < cntPos.size() ? cntPos[i] : -1;
    for (int j = 0; j < keyNames.size(); j ++)
        if (j == pos)
        {
            int cntResult = 0;
            if (cntNames[i] == "*")
                cntResult = entries.size();
            else
            {
                int countKey = -1;
                for (int k = 0; k < keyNames.size(); k ++)
                    if (keyNames[k] == cntNames[i])
                    {
                        countKey = k;
                        break;
                    }

                if (countKey == -1)
                    throw DatabaseError("Counting key " + cntNames[i] + " not found.");

                for (int k = 0; k < entries.size(); k ++)
                    if (entries[k][countKey].type() != Variant::NONE)
                        cntResult ++;
            }
            for (auto &entry: resultEntries)
                entry.push_back(cntResult);
        }
        else
        {
            for (int k = 0; k < entries.size(); k ++)
                resultEntries[k].push_back(entries[k][j]);
        }
}

void DatabaseExt::load(const std::string &tableName, const std::vector< std::map<std::string, Variant> > &entries)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    for (size_t i = 0; i < entries.size(); i++)
    {
        _tables[tableName].insertInto(entries[i]);
    }
}