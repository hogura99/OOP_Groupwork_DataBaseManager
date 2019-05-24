#include "database.h"

Database::Database()
{
    createDir(SimSql::dataDir);
    chdir(SimSql::dataDir.c_str());
    _dataDir = getCwd() + '/';
}

void Database::createDatabase(const std::string &dbName)
{
    assertDatabaseNotExist(dbName);

    createDir(dbName);
}

void Database::useDatabase(const std::string &dbName)
{
    assertDatabaseExist(dbName);

    changeCwd(_dataDir + dbName);
    _dbName = dbName;

    // init tables
    auto tNames = tableNames();
    for (auto &tName : tNames)
    {
        _tables[tName] = Table(tName);
        _tables[tName].loadInfo();
    }
}

bool Database::dropDatabase(const std::string &dbName)
{
    assertDatabaseExist(dbName);
    if (_dbName == dbName)
    {
        changeCwd(_dataDir);
        _dbName.clear();
    }

    auto fnames = listDir(_dataDir + dbName);
    for (auto &fname : fnames)
    {
        if (removeFile(_dataDir + dbName + "/" + fname) == 0)
            return false;
    }
    return removeDir(_dataDir + dbName);
}

std::vector<std::string> Database::databaseNames() const
{
    return listDir(_dataDir);
}

QueryResult Database::showDatabases() const
{
    return QueryResult(new QueryResultShowDatabases(databaseNames()));
}

bool Database::isDatabase(const std::string &dbName) const
{
    return isDir(_dataDir + dbName);
}

void Database::createTable(const std::string &tableName, const std::vector<Field> &fields, const std::string &primaryKey)
{
    assertDatabaseSelected();
    assertTableNotExist(tableName);

    _tables[tableName] = Table(tableName);
    _tables[tableName].createTable(fields, primaryKey);
}

QueryResult Database::showColumnsFrom(const std::string &tableName)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    return _tables[tableName].showColumns();
}

void Database::insertInto(const std::string &tableName, const std::map<std::string, Variant> &entry)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    _tables[tableName].insertInto(entry);
}

void Database::dropTable(const std::string &tableName)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    removeFile(SimSql::indexFileName(tableName));
    removeFile(SimSql::dataFileName(tableName));
    removeFile(SimSql::infoFileName(tableName));
}

std::vector<std::string> Database::tableNames() const
{
    assertDatabaseSelected();
    auto tableFileNames = listDir("./");
    std::vector<std::string> tableNames;
    for (auto &fName : tableFileNames)
    {
        auto seg = split(fName, '.');
        if (seg.size() == 2 && seg.back() == SimSql::suffixDB.substr(1))
            tableNames.emplace_back(seg.front());
    }
    return tableNames;
}

QueryResult Database::showTables() const
{
    return QueryResult(new QueryResultShowTables(_dbName, tableNames()));
}

void Database::assertDatabaseExist(const std::string &dbName) const
{
    if (!isDatabase(dbName))
        throw DatabaseError("Unknown database " + dbName);
}

void Database::assertDatabaseNotExist(const std::string &dbName) const
{
    if (isDatabase(dbName))
        throw DatabaseError("Database " + dbName + " already exists");
}

void Database::assertTableExist(const std::string &tableName) const
{
    if (!isTable(tableName))
        throw DatabaseError("Unknown table " + tableName);
}

void Database::assertTableNotExist(const std::string &tableName) const
{
    if (isTable(tableName))
        throw DatabaseError("Table " + tableName + " already exists");
}

void Database::assertDatabaseSelected() const
{
    if (_dbName.empty())
        throw DatabaseError("Database not selected");
}

void Database::deleteFrom(const std::string &tableName, const Expr &expr)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    _tables[tableName].deleteWhere(expr);
}

QueryResult Database::selectAllFrom(const std::string &tableName, const Expr &expr)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    return _tables[tableName].selectAllFrom(expr);
}

QueryResult Database::selectFrom(const std::string &tableName, const std::vector<std::string> &keyNames, const Expr &expr)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    return _tables[tableName].selectFrom(keyNames, expr);
}

void Database::update(const std::string &tableName, const std::vector<std::string> &keys, const std::vector<Variant> &values, const Expr &expr)
{
    assertDatabaseSelected();
    assertTableExist(tableName);

    _tables[tableName].update(keys, values, expr);
}
