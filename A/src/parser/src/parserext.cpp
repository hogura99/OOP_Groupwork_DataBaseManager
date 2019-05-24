#include "parserext.h"
#include "../../backend/src/field.h"
#include "../../backend/include/stack.h"
#include <algorithm>

Statement ParserExt::parseSelect()
{
    std::cerr << "!!! new parse select !!!" << std::endl;
    return Parser::parseSelect();
}