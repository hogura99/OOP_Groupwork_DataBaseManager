#include "databaseext.h"

QueryResult DatabaseExt::selectAllFrom(const std::string &tableName, const std::vector<Column> &columns,
                                       const Expr &expr, const std::string* fileName,
                                       const std::vector<Column>& groupByColumn,
                                       const std::vector<Column>& orderByColumns)
{
    auto selectResultBase = Database::selectAllFrom(tableName, expr);
    auto selectResult = dynamic_cast<QueryResultSelect*>(selectResultBase.result());

    assert(selectResult != nullptr);

    std::vector<std::string> _keyNames = selectResult->keyNames();
    std::vector<Entry> _entries = selectResult->entries();
    std::vector<Entry> _resultEntries;
    std::vector<Group> _groups;

    bool isGatherSelect = false;
    for (auto column: columns)
        if (column.type != Token::ID && column.type != Token::MUL)
        {
            isGatherSelect = true;
            break;
        }

    if (!isGatherSelect && groupByColumn.empty())
    {
        /*std::vector<Column> _groupByColumn;
        for (auto keyName: _keyNames)
            _groupByColumn.push_back(Column("", keyName, Token::ID));
        makeGroupBy(_groupByColumn, _keyNames, _entries, _groups);*/
        for (auto entry: _entries)
            _groups.push_back({entry});
    }
    else
    {
        makeGroupBy(groupByColumn, _keyNames, _entries, _groups);
    }
    gatherEntriesInGroup(columns, _keyNames, _groups, _resultEntries, isGatherSelect);
    orderEntriesBy(_resultEntries, columns, orderByColumns);

    std::vector<std::string> _columns;
    if (isGatherSelect)
        for (auto column: columns)
            _columns.push_back(toStdString(column));
    else
        _columns = _keyNames;

    auto queryResult = new QueryResultSelectInto(_columns, _resultEntries, fileName, groupByColumn, orderByColumns);

    return QueryResult(queryResult);
}

QueryResult DatabaseExt::selectFromMultTables(const std::vector<std::string> &tableNames,
                                            const std::vector<Column> &columns, const Expr &expr, const std::string* fileName,
                                            const std::vector<Column> &groupByColumn, const std::vector<Column>& orderByColumn)
{
    std::vector<Field> fields;
    for (auto tableName : tableNames)
    {
        Table table = _tables[tableName];
        for (auto field : table.fields())
        {
            fields.emplace_back(Field(tableName + "." + field.key(), field.type(), field.isNull(), field.isPrimary()));
        }
    }
    std::string primaryKey = _tables[tableNames[0]].fields()[0].key();
    const std::string name = "WoBuXinWoQuZheGeMingZiHaiNengChongMing";
    createTable(name,fields, primaryKey);
    std::vector<Entry> entries;
    InsertEntry(_tables[name], tableNames, 0, entries);

    auto queryResult = selectFrom(name, columns, expr, fileName, groupByColumn, orderByColumn);

    dropTable(name);
    return queryResult;
}

void DatabaseExt::InsertEntry(Table& currentTable, const std::vector<std::string> &tableNames, int tableNum, std::vector<Entry> &entries)
{
    if (tableNum == tableNames.size())
    {
        std::vector<Variant> entryInserted;
        for (auto entriesEach : entries)
            for (auto entry : entriesEach)
                entryInserted.emplace_back(entry);
        
        currentTable.insertInto(Entry(entryInserted));
        return;
    }
    auto selectResultBase = Database::selectAllFrom(tableNames[tableNum], Expr());
    auto selectResult = dynamic_cast<QueryResultSelect*>(selectResultBase.result());

    assert(selectResult != nullptr);

    std::vector<std::string> _keyNames = selectResult->keyNames();
    std::vector<Entry> _entries = selectResult->entries();
    for (auto entry : _entries)
    {
        entries.emplace_back(entry);
        InsertEntry(currentTable, tableNames, tableNum + 1, entries);
        entries.pop_back();
    }
}

QueryResult DatabaseExt::selectFrom(const std::string &tableName,
                                    const std::vector<Column> &columns, const Expr &expr, const std::string* fileName,
                                    const std::vector<Column> &groupByColumn, const std::vector<Column>& orderByColumn)
{
    std::vector<std::string> _keyNames;

    for (auto column: columns)
        if (column.type != Token::MUL && column.name != "*")
            _keyNames.push_back(column.name);

    auto selectResultBase = Database::selectFrom(tableName, _keyNames, expr);
    auto selectResult = dynamic_cast<QueryResultSelect*>(selectResultBase.result());

    std::vector<Entry> _entries = selectResult->entries();
    std::vector<Entry> _resultEntries;
    std::vector<Group> _groups;

    bool isGatherSelect = false;
    for (auto column: columns)
        if (column.type != Token::ID && column.type != Token::MUL)
        {
            isGatherSelect = true;
            break;
        }

    if (!isGatherSelect && groupByColumn.empty())
    {
        /*std::vector<Column> _groupByColumn;
        for (auto keyName: _keyNames)
            _groupByColumn.push_back(Column("", keyName, Token::ID));
        makeGroupBy(_groupByColumn, _keyNames, _entries, _groups);*/
        for (auto entry: _entries)
            _groups.push_back({entry});
    }
    else
    {
        makeGroupBy(groupByColumn, _keyNames, _entries, _groups);
    }
    gatherEntriesInGroup(columns, _keyNames, _groups, _resultEntries, isGatherSelect);
    orderEntriesBy(_resultEntries, columns, orderByColumn);

    std::vector<std::string> _columns;
    for (auto column: columns)
        _columns.push_back(toStdString(column));

    auto queryResult = new QueryResultSelectInto(_columns, _resultEntries, fileName, groupByColumn, orderByColumn);

    return QueryResult(queryResult);
}

void DatabaseExt::gatherEntriesInGroup(const std::vector<Column> &columns, const std::vector<std::string> &keyNames,
                                       const std::vector<Group> &groups, std::vector<Entry> &resultEntries, bool isGatherSelect)
{
    for (auto _group: groups)
    {
        Entry _resultEntry;
        if (isGatherSelect)
        {
            gatherEntries(columns, keyNames, _group, _resultEntry);
            resultEntries.push_back(_resultEntry);
        }
        else
        {
            for (auto member: _group)
                resultEntries.push_back(member);
        }
    }
}

void DatabaseExt::gatherEntries(std::vector<Column> columns,
                                std::vector<std::string> keyNames,
                                const std::vector<Entry> &entries,
                                Entry &resultEntries)
{
    int iter_entry = 0;
    for (size_t j = 0; j < columns.size(); j ++)
    {
        Column column = columns[j];
        int pos = 0;
        for (size_t i = 0; i < keyNames.size(); i ++)
        {
            std::string v = keyNames[i];
            if (strcmp(v.c_str(), column.name.c_str()) == 0)
                break;
            pos++;
        }
        //assert(pos < keyNames.size() || column.name == "*");
        switch (column.type)
        {
            case Token::ID:
                if (!entries.empty())
                    resultEntries.push_back(entries[0][iter_entry ++]);
                break;
            case Token::COUNT: case Token::MAX: case Token::MIN: case Token::AVG: case Token::SUM:
            {
                Variant result;
                gatherFunction(column.type, entries, pos, result);
                resultEntries.push_back(result);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void DatabaseExt::gatherFunction(const Token::Type &gatherType,
                                 const std::vector<Entry> &entries,
                                 int columnPos, Variant &gatherResult)
{
    // TODO: bug may appear here. 2019.06.23.
    int totalColumns = entries.empty() ? 0 : entries[0].size();
    int countAVG = 0;
    switch (gatherType)
    {
        case Token::COUNT: case Token::SUM: case Token::MAX:
            gatherResult = 0;
            break;
        case Token::AVG:
            gatherResult = .0;
            break;
        case Token::MIN:
            gatherResult = INT32_MAX;
            break;
        default:
            break;
    }
    for (auto entry: entries)
    {
        if (gatherType != Token::COUNT && columnPos >= totalColumns)
            throw DatabaseError(Token::name[gatherType] + " error.");
        Variant var;
        if (columnPos < totalColumns)
            var = entry[columnPos];
        switch (gatherType)
        {
            case Token::COUNT:
            {
                if (columnPos < totalColumns)
                    gatherResult = gatherResult + int(var.type() != Variant::NONE);
                else
                    gatherResult = gatherResult + 1;
                break;
            }
            case Token::MAX:
            {
                if (var.type() != Variant::NONE)
                    gatherResult = std::max(gatherResult, var);
                break;
            }
            case Token::MIN:
            {
                if (var.type() != Variant::NONE)
                    gatherResult = std::min(gatherResult, var);
                break;
            }
            case Token::SUM:
            {
                if (var.type() != Variant::NONE)
                    gatherResult = gatherResult + var;
                break;
            }
            case Token::AVG:
            {
                if (var.type() != Variant::NONE)
                {
                    gatherResult = gatherResult + var;
                    countAVG ++;
                }
                break;
            }
        }
    }
    if (gatherType == Token::AVG)
        gatherResult = gatherResult / countAVG;
}

void DatabaseExt::makeGroupBy(const std::vector<Column> &groupByKey,
                              const std::vector<std::string> &keyNames,
                              const Group &entries, std::vector<Group> &groups)
{
    auto _groupByKey = groupByKey;
    std::sort(_groupByKey.begin(), _groupByKey.end());
    std::vector<int> rankOfKey(groupByKey.size(), -1);
    for (int i = 0; i < keyNames.size(); i ++)
    {
        auto rank = std::lower_bound(_groupByKey.begin(), _groupByKey.end(), Column("", keyNames[i], Token::NONE)) - _groupByKey.begin();
        if (rank < _groupByKey.size())
            rankOfKey[rank] = i;
    }

    // TODO: check the count(*) statements.
    auto _equal = [=] (const Entry &entry1, const Entry &entry2)
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
            if (_equal(entry, group.front()))
            {
                group.push_back(entry);
                flag = true;
                break;
            }
        if (!flag)
            groups.push_back({entry});
    }
}

void DatabaseExt::orderEntriesBy(std::vector<Entry> &entries,
                                 const std::vector<Column> &columns,
                                 const std::vector<Column> &orderByColumns)
{
    if (orderByColumns.empty())
        return;

    std::vector<int> orderByPosition(orderByColumns.size());
    for (int i = 0; i < orderByColumns.size(); i ++)
    {
        int k = -1;
        for (int j = 0; j < columns.size(); j ++)
            if (columns[j] == orderByColumns[i])
            {
                k = j;
                break;
            }
        if (k == -1)
            throw DatabaseError("order by error");
        orderByPosition[i] = k;
    }

    auto _less_compare = [=, &orderByPosition](const Entry &a, const Entry &b)
    {
        for (int pos: orderByPosition)
            if (a[pos] < b[pos])
                return true;
            else if (a[pos] > b[pos])
                return false;
        return false;
    };

    sort(entries.begin(), entries.end(), _less_compare);
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

/*
bool DatabaseExt::dropDatabase(const std::string &dbName)
{
    useDatabase(dbName);
    for (auto tablePair: _tables)
        if (isTable(tablePair.first))
            dropTable(tablePair.first);
    return Database::dropDatabase(dbName);
}*/
