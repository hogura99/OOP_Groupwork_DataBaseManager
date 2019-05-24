#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"

int main()
{
    std::string cmd;
    while (getline(std::cin, cmd, ';'))
    {
        cmd += ';';
        Parser parser(cmd);
        try
        {
            auto statement = parser.parseStatement();
            statement.content()->print();
        }
        catch (TokenError &e)
        {
            std::cout << "line " << parser.lexer().lineNo() << ", ";
            std::cout << "column " << parser.lexer().columnNo() << ": ";
            std::cout << e.what() << '\n';
        }
        catch (LexerError &e)
        {
            std::cout << "line " << parser.lexer().lineNo() << ", ";
            std::cout << "column " << parser.lexer().columnNo() << ": ";
            std::cout << e.what() << '\n';
        }
        catch (ParserError &e)
        {
            std::cout << "line " << parser.lexer().lineNo() << ", ";
            std::cout << "column " << parser.lexer().columnNo() << ": ";
            std::cout << e.what() << '\n';
        }
        catch (std::exception &e)
        {
            std::cout << "line " << parser.lexer().lineNo() << ": ";
            std::cout << e.what() << '\n';
        }
        std::cout << '\n';
    }
    return 0;
}