#include "databaseext.h"

QueryResult DatabaseExt::selectAllFrom(const std::string &tableName, const std::vector<Column> &columns,
                                       const Expr &expr, const std::string* file_name,
                                       const std::vector<std::string>& groupByColumn)
{
    /*std::vector<std::string> keyNames = fieldNames;
    std::reverse(keyNames.begin(), keyNames.end());
    keyNames.pop_back();
    std::reverse(keyNames.begin(), keyNames.end()); // delete the first '*' of keynames
*/
    // TODO: change the key names

    /*std::vector<std::string> _keyNames;
    std::vector<std::string> _cntNames;
    std::vector<size_t> _cntPos;

    getCountNames(keyNames, _keyNames, _cntNames, _cntPos);*/

    auto selectResult = dynamic_cast<QueryResultSelect*>(Database::selectAllFrom(tableName, expr).result());

    std::vector<std::string> _keyNames = selectResult->keyNames();
    std::vector<Entry> _entries = selectResult->entries();
    std::vector<Entry> _resultEntries;
    std::vector<Group> _groups;
    makeGroupBy(groupByColumn, _keyNames, _entries, _groups);
    for (auto _group: _groups)
    {
        Entry _resultEntry;
        if (!columns.empty())
        {
            gatherEntries(columns, _keyNames, _group, _resultEntry);
            _resultEntries.push_back(_resultEntry);
        }
        else
        {
            _resultEntries.emplace_back(_group);
        }
    }

    delete selectResult;
    auto queryResult = new QueryResultSelectInto(_keyNames, _resultEntries, file_name, groupByColumn);

    return QueryResult(queryResult);
}

QueryResult DatabaseExt::selectFrom(const std::string &tableName,
                                    const std::vector<std::string> &keyNames, const Expr &expr, const std::string* file_name,
                                    const std::vector<std::string> &groupByColumn, std::string *orderByKey)
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
    auto queryResult = new QueryResultSelectInto(keyNames, _resultEntries, file_name, groupByColumn);

    return QueryResult(queryResult);
}

void DatabaseExt::gatherEntries(const std::vector<Column> &columns,
                                const std::vector<std::string> &keyNames,
                                const std::vector<Entry> &entries,
                                Entry &resultEntries)
{
    int iter_entry = 0;
    for (auto column: columns)
    {
        switch (column.type)
        {
            case Token::ID:
                if (!entries.empty())
                    resultEntries.push_back(entries[0][iter_entry ++]);
                break;
            case Token::COUNT:
            {
                int countRes = 0;
                int pos = -1;
                for (int i = 0; i < keyNames.size(); i++)
                    if (keyNames[i] == column.columnName) {
                        pos = i;
                        break;
                    }
                if (pos != -1)
                    for (auto value: entries[pos])
                        if (value.type() != Variant::NONE)
                            countRes ++;
                resultEntries.push_back(countRes);
                break;
            }
            default:
            {
                break;
            }
        }
    }
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

void DatabaseExt::makeGroupBy(const std::vector<std::string> &groupByKey,
                              const std::vector<std::string> &keyNames,
                              const Group &entries, std::vector<Group> &groups)
{
    auto _groupByKey = groupByKey;
    std::sort(_groupByKey.begin(), _groupByKey.end());
    std::vector<int> rankOfKey(groupByKey.size());
    for (int i = 0; i < keyNames.size(); i ++)
    {
        auto rank = std::lower_bound(_groupByKey.begin(), _groupByKey.end(), keyNames[i]) - _groupByKey.begin();
        if (rank < _groupByKey.size())
            rankOfKey[rank] = i;
        else
            rankOfKey[rank] = -1;
    }

    // TODO: check the count(*) statements.
    auto _compare = [=] (const Entry &entry1, const Entry &entry2)
    {
        for (int i : rankOfKey)
            if (i != -1 && entry1[i] != entry2[i])
                return false;
        return true;
    };

    for (auto entry: entries)
    {
        bool flag = false;
        for (auto &group: groups)
            if (_compare(entry, group.front()))
            {
                group.push_back(entry);
                flag = true;
                break;
            }
        if (!flag)
            groups.push_back({entry});
    }
}

void DatabaseExt::load(const std::string &tableName, const std::vector< std::map<std::string, Variant> > &entries)
{
    std::ifstream infile;
    infile.open("output_file");
    if (!infile.is_open())
    {
        throw DatabaseError("open file failed");
    }
    assertDatabaseSelected();
    assertTableExist(tableName);

    for (size_t i = 0; i < entries.size(); i++)
    {
        _tables[tableName].insertInto(entries[i]);
    }
}