#include "statements.h"

std::map<StatementBase::StatementType, std::string> StatementBase::typeStringMap{
    {StatementBase::NONE, "none"},

    {StatementBase::CREATE_TABLE, "create table"},
    {StatementBase::CREATE_DATABASE, "create database"},

    {StatementBase::USE_DATABASE, "use database"},

    {StatementBase::SHOW_TABLES, "show tables"},
    {StatementBase::SHOW_DATABASES, "show databases"},
    {StatementBase::SHOW_COLUMNS, "show columns"},

    {StatementBase::DROP_TABLE, "drop table"},
    {StatementBase::DROP_DATABASE, "drop database"},

    {StatementBase::SELECT, "select"},
    {StatementBase::INSERT, "insert"},
    {StatementBase::UPDATE, "update"},
    {StatementBase::DELETE, "delete"},
    {StatementBase::INTO_OUTFILE, "into outfile"},
};
