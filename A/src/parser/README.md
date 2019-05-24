# Parser

本部分实现了一个 SQL 语言解析器，可将输入的 SQL 指令解析成不同类型的 statement，传入后端进行下一步操作。

## Key Files

### token.h

一个 token 即为 SQL 指令中的一个单词（即每个用空格分开的 word）。本部分实现了一个 token 类，该类储存了 token 的相关信息：类型 type 和数据 data。token 主要分为四类：

1. **关键字**：其 type 即为该关键字（e.g. Token::CREATE），data 为空。可以使用 ```Type toKeyword()``` 来获取该关键字。
2. **运算符**：其 type 即为该运算符（e.g. EQ），data 为空。可以使用 ```Type toKeyword()``` 来获取该运算符。
3. **标识符**：其 type 为 ID，data 储存了该标识符的原始字符串（区分大小写）。可以使用 ```string& toID()``` 来获取其内容。
4. **操作数**（int，double 或 char）：其 type 为OPERAND，data 储存了其原始数据。可以使用```Variant toOperand()``` 来获取具体数据。

### lexer.h

本部分实现了一个 SQL 指令的文法分析器，可以从原始指令中按顺序提取出所有的 token。主要接口如下：

- Lexer(const std::string & cmd)：Lexer 类的构造函数，生成一个用于解析输入指令 cmd 的文法分析器。
- Token next()：返回当前输入流中下一个待扫描的 token。

### statements.h

StatementBase 类为所有 SQL 指令的基类，该类储存了某条指令的基本信息：指令类型 type 和标识符 id。其中，id 为该指令对应的 database 或 table 的名字。该基类主要接口如下：

- std::string & id()：返回该指令的标识符 id。
- StatementType type()：返回该指令的类型。
- void print()：输出该指令的基本信息。也可以直接对该类对象使用输出流符号 << 进行输出。

不同种类的 SQL 指令均通过继承 StatementBase 类来实现，它们根据自身特点分别拥有不同的接口。

### expr.h

用于表示whereclauses中的表达式。将表达式分为left, token, right三个部分，left, right为左、右"操作表达式"的指针，可为nullptr，token为关键字。

- eval(const std::map<std::string, Variant> &varMap) const：讨论token.type的类型，对表达式执行相应的运算，返回运算结果。
- bool isNull() const : 返回关键字是否为NONE。
- bool hasLeft()/hasRight()：返回左/右“操作表达式”是否为空。
- const Expr &left()/right()：返回左/右“操作表达式”的指针。
- const Token &token() const：返回关键字token。
- Token::Type tokenType()：返回关键字的类型token.type。
- bool isNull() const { return _token.type() == Token::NONE; }
- bool isValue() const：返回token的类型是否为数字或表格名或数据库名。
- void setLeft(const Expr &other)：用另一个Expr的左“操作表达式”设定当前Expr的left。
- void setRight(const Expr &other):用另一个Expr的右“操作表达式”设定当前Expr的right。
- void setToken(const Token &t)：用一个Token设定当前Expr的关键字。

### parser.h

用于逐部分地解析语句。其中最主要的函数为Statement parseStatement()，语句的解析结果封装在该函数的返回值Statement中。

- std::map<Token::Type, int> opPrioMap：将与运算符相关的关键字一一对应为整数，数值大者优先级越高。
- Statement parseStatement()：按照语句中的第一个关键字的不同(CREATE, DROP, SHOW, USE, UPDATE, INSERT，SELECT)分类，分别用相应函数进行解析处理。最后将解析结果存在Statement类型中返回，含有不同关键字的语句分别按照其语法，用相应的构造函数构造Statement变量。
- Statement parseCreate()：实现对CREATE关键字语句的解析。对于读入的语句，需要读入下一个关键字后按照DATABASE和TABLE分两种情况处理，对于DATABASE的情况，接下来只需读入数据库的名称databaseId即可，最终返回用StatementCreateDatabase(databaseId)构造的Statement；对于TABLE的情况，则在读取表格名称table_id之后，通过函数void parseFieldList()读入表格的各项属性名和可能存在的primary key，并分别将其存在向量fields和std::string priKey之中,最后返回用StatementCreateTable(table_id, fields, priKey)构造的Statement。
- Expr parseWhereClause()：实现对whereclauses的解析。主要通过栈的数据结构实现。主要涉及两个栈：运算符栈ops和运算数栈vals。运算数栈存运算符对应的数值，运算符栈存opPrioMap中约定的关键字一一对应的数值。自左向右地扫描whereclause，遇到运算数则将其入运算数栈vals，遇到运算符时，则需要先将其与ops栈顶的运算符比较优先级，若当前运算符优先级较低，则运算数栈ops中弹出运算数，先执行栈顶运算符的运算，并将运算结果入vals栈。不断如此，直至ops中新的栈顶运算符的优先级低于当前运算符的优先级时，再将当前运算符入ops栈。执行运算时，需要按照运算符是一元运算符、二元运算符分别考虑。运算结束后，最终vals栈将仅剩下一个元素，将其作为该whereclause对应的表达式返回。

- Statement parseDelete()：实现对DELETE关键字语句的解析。对于读入的语句，首先读入FROM关键字后的表格名称table_id，之后，若有WHERE关键字，则通过函数Expr parseWhereClause()解析whereclauses，解析结果存入where；反之，若不含WHERE关键字，则将where设为空值。最后返回用StatementDelete(table_id,where)构造的Statement。
- Statement parseInsert()：实现对INSERT关键字语句的解析。对于读入的语句，首先读入INTO关键字后的表格名称tableId,再读取左括号后通过函数std::vector\<std::string\> parseColumnList()读取属性名的序列，并将其存入向量columns中，读取右括号后通过函数std::vector\<Variant\> parseColumnList()读取待插入值的序列，并将其存入向量values中。再用名为entry的map将columns与values中的每一个元素按照顺序一一对应。最后返回用StatementInsert(tableId,entry)构造的Statement。
- Statement parseUse()：实现对USE关键字语句的解析。对于读入的语句，读入USE关键字后的数据库名称dbName，最后返回用StatementUseDatabase(dbName)构造的Statement。
- Statement parseShow()：实现对SHOW关键字语句的解析。对于读入的语句，需要按照SHOW之后下一关键字的类型分两种情况考虑：若下一关键字为TABLES或DATABASES，则读入句尾的分号后，分别直接返回用StatementShowTables()和StatementShowDatabases()构造的Statement；若下一关键字为COLUMNS，则需要读取FROM关键字后的表格名tableName，返回用StatementShowColumns(tableName)构造的Statement。
- Statement parseDrop()：实现对DROP关键字语句的解析。对于读入的语句，按照SHOW之后下一关键字的类型分两种情况考虑：若下一关键字为TABLE，则读入TABLE后的表格名tableName之后，返回用StatementDropTable(tableName)构造的Statement;若下一关键字为DATABASE，则读入DATABASE后的数据库名dbName，返回用StatementDropDatabase(dbName)构造的Statement。
- Statement parseUpdate()：实现对UPDATE关键字语句的解析。对于读入的语句，首先读入UPDATE关键字后的表格名tableName，之后用void parseSetList(keys, values)函数读取属性名和值，并将属性名序列存入std::vector\<std::string\> keys，将值存入std::vector\<Variant\> values之中。之后，若有WHERE关键字，则通过函数Expr parseWhereClause()解析WHERE关键字后的whereclauses，将解析结果存入expr中；反之若不含WHERE关键字，则将expr设为空值。最后返回用StatementUpdate(tableName, keys, values, expr)构造的Statement。
- Statement parseSelect()：实现对SELECT关键字语句的解析。对于读入的语句，首先通过函数std::vector \<std::string\>parseSelectList()读入要返回的属性名列表，并将其存入向量columns中，再读入FROM关键字后的表格名称table_id。之后，若有WHERE关键字，则用Expr parseWhereClause()函数解析whereclauses，解析结果存入where；反之，若不含WHERE关键字，则将where设为约定的空值NULL_EXPR。最后返回用StatementSelect(table_id, columns, where)构造的Statement。


## License

This project is licensed under the Apache License - see the [LICENSE](../../LICENSE) file for details.