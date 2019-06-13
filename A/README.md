# SimpleSQL

A simple cross-platform relational database with SQL query support. 

## Prerequisites

1. g++ (with c++ 11 support )
2. GNU Make
3. cmake

## Build

### Linux

```bash
$ cd SimpleSQL
$ mkdir build && cd build 
$ cmake ../
$ make -j4
```

### Windows

```bash
$ cd SimpleSQL
$ mkdir build && cd build 
$ cmake -G "Unix Makefiles" ../
$ make -j4
```

## Run

### Linux

```bash
$ ./main
```

### Windows

```bash
$ main.exe 
```

## 组织结构

本工程分为三个模块，分别为parser, backend, shell。

+ parser为解析器模块，将用户键盘输入的SQL命令解析成SQL statement，提供给后端进一步处理。

+ backend为后端模块，是真正执行SQL语句的部分，涉及文件读写操作。
+ shell为用户界面模块，接受用户输入的SQL命令，调用parser解析命令，调用backend执行命令。

具体实现及接口说明请参考三个模块的README.md

+ Parser: [Parser README](src/parser/README.md) 
+ Backend: [Backend README](src/backend/README.md) 
+ Shell: [Shell README](src/shell/README.md) 

文件目录树状结构如下

<!-- tree SimpleSQL -I "CMakeFiles|cmake-build-debug|sln|build|*.zip|test" -->


```
SimpleSQL
├── CMakeLists.txt
├── developer.txt
├── LICENSE
├── README.md
├── README_TEMPLATE.md
└── src
    ├── backend
    │   ├── CMakeLists.txt
    │   ├── include
    │   │   ├── bitset.h
    │   │   ├── stack.h
    │   │   └── vector.h
    │   ├── README.md
    │   └── src
    │       ├── btree.cpp
    │       ├── btree.h
    │       ├── database.cpp
    │       ├── databaseexcept.h
    │       ├── database.h
    │       ├── datastream.cpp
    │       ├── datastream.h
    │       ├── entry.cpp
    │       ├── entry.h
    │       ├── field.cpp
    │       ├── field.h
    │       ├── fileio.h
    │       ├── page.cpp
    │       ├── page.h
    │       ├── print.h
    │       ├── queryresults.h
    │       ├── simsql.h
    │       ├── table.cpp
    │       ├── table.h
    │       ├── utils.h
    │       ├── variant.cpp
    │       └── variant.h
    ├── parser
    │   ├── CMakeLists.txt
    │   ├── README.md
    │   └── src
    │       ├── expr.cpp
    │       ├── expr.h
    │       ├── lexer.cpp
    │       ├── lexer.h
    │       ├── parser.cpp
    │       ├── parser.h
    │       ├── statements.cpp
    │       ├── statements.h
    │       ├── token.cpp
    │       └── token.h
    └── shell
        ├── CMakeLists.txt
        ├── README.md
        └── src
            └── shell.cpp
```

## Bonus

### BTree

我们提供基于内存的BTree接口，很容易将它改成基于磁盘的BTree。

### Support complicated where clause 

除了基本要求之外(AND, OR, =, >, <)，我们还支持操作符：>=, !=, <=，支持带括号的表达式，支持NOT，支持带+,-,*,/的表达式。

Sample input: 

```sql
create database oop;
use oop;
create table info(a int, b int, primary key(a));
insert into info (a,b) values (1,1);
insert into info (a,b) values (2,2);
insert into info (a,b) values (3,3);
select * from info where 2*(1+a) = 1+2*3 - 8/2 + b and (not 1<=2 or 1>=2 or 1!=2);
drop database oop;
```

Sample output: 

```sql
a       b
1       1
```

### Update multiple values in one shot 

Sample input 

```sql
create database oop;
use oop;
create table info(a int, b int, primary key(a));
insert into info (a,b) values (1,1);
insert into info (a,b) values (2,2);
insert into info (a,b) values (3,3);
update info set a=5,b=10 where a=1 and b=1;
select * from info;
drop database oop;
```

Sample output

```
a       b
2       2
3       3
5       10
```

### More...

**更多Bonus等你来发现**

## License

This project is licensed under the Apache License - see the [LICENSE](LICENSE) file for details

## 开发者约定

### 命名规范

#### 局部变量 & struct的成员变量 & class的公有/静态成员变量

驼峰命名法，首字母**小写**。例如

```cpp
std::string table;
SqlParser sqlParser;

struct Point
{
    double xPos; 
    double yPos; 
};

class Point
{
public:
    double xPos; 
    double yPos; 
    static int visits; 
};
```

#### class的私有成员变量

以下划线开头，其后与局部变量相同。例如

```cpp
class Point
{
protected:
    double _xPos; 
    double _yPos; 
};
```

#### 函数命名

驼峰命名法，首字母**小写**。例如

```cpp
selectFromWhere()
insertInto()
```

特别的，当类的成员函数返回某个成员变量时，成员函数与变量名一致，无下划线。例如

```cpp
class Variant
{
public:
    bool isNull() const { return _isNull; }
protected:
    bool _isNull; 
};
```

#### 类命名

驼峰命名法，首字母**大写**。例如

```cpp
class SqlSelectStatement; 
```

#### 枚举类型命名

驼峰命名法，首字母**大写**。其中枚举值**全大写**，例如

```cpp
enum VarType{INT, CHAR, DOUBLE};
```

#### 文件命名

.h, .cpp 文件名为类名的全小写。如

```cpp
class SqlParser
```

文件名为 sqlparser.h, sqlparser.cpp

### 头文件卫士

统一写成

```cpp
#pragma once
```

### 缩进规范

大括号展开时换行，例如

```cpp
int main()
{
    return 0; 
}
```

缩进统一使用4个空格，不要用tab。

头文件内，一句话能写完的函数体不用换行。例如

```cpp
class A
{
public:
    int size() const { return _size; }
}
```

超过两句话的函数体，非模板请写到.cpp文件里，模板请写在.h的类定义后面。

### 注释规范

#### Template

```cpp
/**
 *  @brief  一句话的简要说明
 *  @param  a  参数a的简要说明
 *  @param  b  参数b的简要说明
 *  @return  返回值的说明
 * 
 *  函数功能的详细说明
 */
```

某些IDE是会自动生成以上模板的。

#### Warning

+ 请不要在代码里写中文注释，经常因为编码问题看不到！

+ 接口的注释全部写在.h里。如果实现上有一些巧妙之处（别人看不懂的地方），在.cpp内补充。

#### Examples

可以参考std::vector的注释

#### 函数注释

```cpp
/**
 *  @brief  Creates a %vector with default constructed elements.
 *  @param  __n  The number of elements to initially create.
 *  @param  __a  An allocator.
 *
 *  This constructor fills the %vector with @a __n default
 *  constructed elements.
 */
explicit
    vector(size_type __n, const allocator_type& __a = allocator_type())
    : _Base(__n, __a)
    { _M_default_initialize(__n); }
```

#### 类注释

```cpp
  /**
   *  @brief A standard container which offers fixed time access to
   *  individual elements in any order.
   *
   *  @ingroup sequences
   *
   *  @tparam _Tp  Type of element.
   *  @tparam _Alloc  Allocator type, defaults to allocator<_Tp>.
   *
   *  In some terminology a %vector can be described as a dynamic
   *  C-style array, it offers fast and efficient access to individual
   *  elements in any order and saves the user from worrying about
   *  memory and size allocation.  Subscripting ( @c [] ) access is
   *  also provided as with C-style arrays.
   */
  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {...}
```



