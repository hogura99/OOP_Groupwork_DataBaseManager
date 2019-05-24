#pragma once

#include "datastream.h"
#include "simsql.h"

/**
 * Page class provides method of page loading and dumping
 */
class Page
{
  public:
    Page() = default;
    template <typename T>
    static size_t dumpPage(DataStream &out, PageIndex i, const T &data)
    {
        cur = i;
        out.seekp(i * SimSql::blockSize);
        out << next << data;
        out.flush();
        return (size_t)out.tellp() % SimSql::blockSize;
    }
    static size_t dumpPlaceholder(DataStream &out, PageIndex i)
    {
        cur = i;
        out.seekp(i * SimSql::blockSize);
        out.write(std::string(SimSql::blockSize, SimSql::placeHolder).c_str(), SimSql::blockSize);
        out.flush();
        return SimSql::blockSize;
    }
    static PageIndex newPageIndex(DataStream &_fs)
    {
        _fs.seekg(0, std::ios::end);
        return PageIndex(_fs.tellg()) / SimSql::blockSize;
    }
    template <typename T>
    static size_t loadPage(DataStream &in, PageIndex i, T &data)
    {
        cur = i;
        in.seekg(i * SimSql::blockSize);
        in >> next >> data;
        return (size_t)in.tellg() % SimSql::blockSize;
    }

  public:
    static PageIndex next;
    static PageIndex cur;
};