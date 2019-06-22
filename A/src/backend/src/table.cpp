
#include "table.h"

std::map<std::string, Variant> Table::entryToMap(const Entry &entry)
{
    std::map<std::string, Variant> m;
    for (size_t i = 0; i < _fields.size(); i++)
        m[_fields[i].key()] = entry[i];
    return m;
}

void Table::loadIndex()
{
    assert(!_name.empty());
    DataStream fsIdx(SimSql::indexFileName(_name), std::ios::in);
    Page::loadPage(fsIdx, 0, _entryPos);
}

void Table::loadInfo()
{
    assert(!_name.empty());
    DataStream fsInfo(SimSql::infoFileName(_name), std::ios::in);
    Page::loadPage(fsInfo, 0, *this);
}

void Table::insertInto(const Entry &entry)
{
    assert(!_name.empty());
    DataStream fsIdx(SimSql::indexFileName(_name), std::ios::in | std::ios::out);
    DataStream fsData(SimSql::dataFileName(_name), std::ios::in | std::ios::out);
    loadIndex();

    // find the insertion pos
    fsData.setWriterPos(0, std::ios::end);

    // save entry pos
    _entryPos[entry[_keyToRank[_priKeyName]]] = (size_t)fsData.writerPos();
    fsData << entry;
    fsData.flush();
    Page::dumpPage(fsIdx, 0, _entryPos);
}

void Table::insertInto(const std::map<std::string, Variant> &entryMap)
{
    Entry entry;
    for (auto &f : _fields)
    {
        if (!entryMap.count(f.key())) // key not found
            entry.emplace_back(Variant());
        else
            entry.emplace_back(entryMap.at(f.key()).convertTo(f.type()));
    }
    insertInto(entry);
}

void Table::createTable(const std::vector<Field> &fields, const std::string &primaryKey)
{
    assert(!_name.empty());
    _fields = fields;
    for (size_t i = 0; i < _fields.size(); i++)
        _keyToRank[_fields[i].key()] = i;

    _priKeyName = primaryKey;
    createFile(SimSql::dataFileName(_name));
    createFile(SimSql::indexFileName(_name));
    createFile(SimSql::infoFileName(_name));

    DataStream fsIdx(SimSql::indexFileName(_name), std::ios::out | std::ios::in);

    Page::dumpPlaceholder(fsIdx, 0);     // reserve space
    Page::dumpPage(fsIdx, 0, _entryPos); // init table block

    DataStream fsInfo(SimSql::infoFileName(_name), std::ios::out | std::ios::in);
    Page::dumpPage(fsInfo, 0, *this);
}

QueryResult Table::selectAllFrom(const Expr &expr)
{
    assert(!_name.empty());
    loadIndex();
    std::vector<std::string> keyNames;
    for (auto &f : _fields)
        keyNames.emplace_back(f.key());

    return selectFrom(keyNames, expr);
}

QueryResult Table::selectFrom(const std::vector<std::string> &keyNames, const Expr &expr)
{
    assert(!_name.empty());
    DataStream fsIdx(SimSql::indexFileName(_name), std::ios::in);
    DataStream fsData(SimSql::dataFileName(_name), std::ios::in);
    loadIndex();
    std::vector<size_t> keyRanks;
    for (auto &kn : keyNames)
    {
        if (!_keyToRank.count(kn))
            throw DatabaseError("Key error");
        keyRanks.emplace_back(_keyToRank[kn]);
    }
    std::vector<Entry> result;
    for (auto &ep : _entryPos)
    {
        fsData.setReaderPos(ep.second);
        Entry entry;
        fsData >> entry;
        if (expr.eval(entryToMap(entry)).toBool())
            result.emplace_back(slice(entry, keyRanks));
    }
    auto priKeyRank = std::find(keyNames.begin(), keyNames.end(), _priKeyName) - keyNames.begin();
    if (priKeyRank != keyNames.size()) // primary key selected
        std::sort(result.begin(), result.end(), [=](const Entry &a, const Entry &b) { return a[priKeyRank] < b[priKeyRank]; });

    return QueryResult(new QueryResultSelect(keyNames, result));
}

QueryResult Table::showColumns()
{
    assert(!_name.empty());
    return QueryResult(new QueryResultShowColumns(_fields));
}

void Table::update(const std::vector<std::string> &keys, const std::vector<Variant> &values, const Expr &expr)
{
    assert(!_name.empty());
    DataStream fsIdx(SimSql::indexFileName(_name));
    DataStream fsData(SimSql::dataFileName(_name));
    loadIndex();

    bool isPriKeyModified = false;
    for (auto &k : keys)
    {
        if (!_keyToRank.count(k))
            throw DatabaseError("Wrong keys");
        if (k == _priKeyName)
            isPriKeyModified = true;
    }

    if (!isPriKeyModified) // modify secondary keys
    {
        for (auto &ei : _entryPos)
        {
            Entry entry;
            fsData.seekg(ei.second);
            fsData >> entry;
            if (expr.eval(entryToMap(entry)).toBool())
            {
                for (size_t i = 0; i < keys.size(); i++)
                    entry[_keyToRank[keys[i]]] = values[i].convertTo(_fields[_keyToRank[keys[i]]].type());

                fsData.seekp(ei.second);
                fsData << entry;
                fsData.flush();
            }
        }
    }
    else // primary keys is modified
    {
        auto result = dynamic_cast<QueryResultSelect *>(selectAllFrom(expr).result())->entries();
        deleteWhere(expr);
        for (auto &e : result)
        {
            for (size_t i = 0; i < keys.size(); i++)
                e[_keyToRank[keys[i]]] = values[i].convertTo(_fields[_keyToRank[keys[i]]].type());

            insertInto(e);
        }
    }
}

void Table::deleteWhere(const Expr &expr)
{
    assert(!_name.empty());
    DataStream fsIdx(SimSql::indexFileName(_name));
    DataStream fsData(SimSql::dataFileName(_name));

    loadIndex();

    for (auto it = _entryPos.begin(); it != _entryPos.end();)
    {
        Entry entry;
        fsData.seekg(it->second);
        fsData >> entry;

        if (expr.eval(entryToMap(entry)).toBool())
            _entryPos.erase(it++);
        else
            it++;
    }
    Page::dumpPage(fsIdx, 0, _entryPos);
}