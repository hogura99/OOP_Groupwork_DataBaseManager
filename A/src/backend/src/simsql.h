#pragma once

#include <string>
#include "variant.h"

typedef size_t TablePos;
typedef size_t PageIndex;
typedef size_t EntryPos;

/**
 * Basic constants and methods in the entire project are defined here.
 */
namespace SimSql
{

static const TablePos nullPos = 0xFFFFFFFF;
static const std::string suffixDB(".data");
static const std::string suffixIndex(".idx");
static const std::string suffixInfo(".info");
static const size_t blockSize = 1024;
static const char placeHolder = '_';
static const size_t maxBlockSize = blockSize - (blockSize >> 1);
static const std::string dataDir = "./data/";

static inline std::string indexFileName(const std::string &tableName)
{
    return tableName + suffixIndex;
}

static inline std::string dataFileName(const std::string &tableName)
{
    return tableName + suffixDB;
}

static inline std::string infoFileName(const std::string &tableName)
{
    return tableName + suffixInfo;
}

} // namespace SimSql
