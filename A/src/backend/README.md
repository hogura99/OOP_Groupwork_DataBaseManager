# Backend 

The core part of the database project. It provides methods for database and table operations on DISK. 

## Key Files

### database.h

Database class provides methods of database and table operation. Support creating / showing / dropping / using database

- When creating a database, it creates a directory of the database name under ./data/
- When using a database, it changes current directory to the database directory.
- When dropping a database, it remove the database directory and all its containing files.
- When showing databases, it lists all directory under ./data/ and get all database names. 

It provides API for table operations. Support creating / showing / dropping / inserting / updating / deleting / selecting from tables. In terms of table operations, it calls Table class to execute.

DO NOT modify any content in ./data/ directory, otherwise it may not work properly. 

### table.h

Table class provides methods for table management and operation

Support creating / showing / dropping / inserting / updating / deleting / selecting from tables.

When creating a table, it creates 3 files under the current database directory

1. info file  (.info) : it stores the basic framework of the table, including fields info, primary key name, etc.
2. index file (.idx)  : it stores the all entries' primary keys and the corresponding positions in the data file.
3. data file  (.data) : it stores the data of all entries.

When insert / selecting / updating / deleting from a table, it loads the index file, and searches for all the entries in the index file that meet the condition of the where clause. Then search for the entire entry in the data file and do specific operations. Return query result if needed. 

Input & output methods are implemented. Support std::cout and DataStream I/O.

### field.h

Field class provide methods to manage field information.

Member `_key` specifies the key name.

Member `_type` specifies the field type. (e.g. int, double, char)

Member `_isNull` indicates whether the field is marked NOT NULL.

Member `_isPrimary` indicates whether the field is marked PRIMARY KEY.

Input & output methods are implemented. Support std::cout and DataStream I/O.

### entry.h

Entry class organizes database entry into vector format.

Inherit std::vector\<Variant\>. Constructors are overwritten.

Input & output methods are implemented. Support std::cout and DataStream I/O.

### datastream.h

DataStream class provides methods to serialize data into binary files

Support all c++ basic types, std::string, and all enum class.

It inherits std::fstream, and always open files in binary mode.

Operator >> and << are overloaded for input and output.

Example: 

```cpp
int a;
DataStream ds(a.txt);
ds << a; // write binary data
ds >> a; // read binary data
```

### variant.h

Variant class unifies variants of different types into one class.

Construct Variant directly with data of basic types (bool, char, int, double, std::string), get data type with `type()`, retrieve data with `toBool()`, `toChar()`, `toInt()`, etc.

Input & output methods are implemented. Support std::cout and DataStream I/O.

### page.h

Page class provides method of loading and dumping pages

### simsql.h

Basic constants and methods in the entire project are defined here.

### btree.h

BTree class implemented based on RAM, it is easy to convert it into disk-based BTree. 

It provides methods to insert / delete / search entries in the b-tree. 

It is never used, but we provides this API for possible future requirements. 

## License

This project is licensed under the Apache License - see the [LICENSE](../../LICENSE) file for details
