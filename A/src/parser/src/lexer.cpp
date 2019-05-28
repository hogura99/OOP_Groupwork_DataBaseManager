#include "lexer.h"
#include <iostream>

std::map<std::string, Token::Type> Lexer::keywords{
    {"create", Token::CREATE},
    {"table", Token::TABLE},
    {"tables", Token::TABLES},
    {"database", Token::DATABASE},
    {"databases", Token::DATABASES},
    {"int", Token::INT},
    {"double", Token::DOUBLE},
    {"char", Token::CHAR},
    {"default", Token::DEFAULT},
    {"primary", Token::PRIMARY},
    {"key", Token::KEY},
    {"insert", Token::INSERT},
    {"into", Token::INTO},
    {"values", Token::VALUES},
    {"delete", Token::DELETE},
    {"from", Token::FROM},
    {"where", Token::WHERE},
    {"select", Token::SELECT},
    {"show", Token::SHOW},
    {"columns", Token::COLUMNS},
    {"update", Token::UPDATE},
    {"drop", Token::DROP},
    {"use", Token::USE},
    {"not", Token::NOT},
    {"null", Token::NULL_SQL},
    {"and", Token::AND},
    {"or", Token::OR},
    {"set", Token::SET},
	{"outfile", Token::OUTFILE}};

std::map<char, Token::Type> Lexer::singleOp{
    {'+', Token::PLUS},
    {'-', Token::MINUS},
    {'*', Token::MUL},
    {'/', Token::DIV},
    {'(', Token::L_PAREN},
    {')', Token::R_PAREN},
    {',', Token::COMMA},
    {';', Token::SEMICOLON}};

std::map<std::string, Token::Type> Lexer::ops{
    {"!", Token::NOT},
    {"<", Token::LT},
    {">", Token::GT},
    {"<>", Token::NEQ},
    {"!=", Token::NEQ},
    {"=", Token::EQ},
    {">=", Token::GEQ},
    {"<=", Token::LEQ},
    {"+", Token::PLUS},
    {"-", Token::MINUS},
    {"*", Token::MUL},
    {"/", Token::DIV},
    {"(", Token::L_PAREN},
    {")", Token::R_PAREN},
    {",", Token::COMMA},
    {";", Token::SEMICOLON}};

bool Lexer::isOp(char ch) const
{
    return ch == '!' || ch == '<' || ch == '>' || ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == ',' || ch == ';';
}

Token Lexer::next()
{
    while (!isEnd())
    {
        // parse number
        if (isdigit(_peek))
        {
            int bufInt = 0; // int
            do
            {
                bufInt = bufInt * 10 + advance() - '0';
            } while (isdigit(_peek));
            if (_peek == '.') // double
            {
                double bufDbl = bufInt;
                advance();
                for (double frac = 0.1; isdigit(_peek); advance())
                {
                    bufDbl += frac * (_peek - '0');
                    frac *= 0.1;
                }
                return Token(Token::OPERAND, bufDbl);
            }
            else
            {
                return Token(Token::OPERAND, bufInt);
            }
        }
        // parse keywords or identifiers
        else if (isalpha(_peek) || _peek == '_')
        {
            memset(_buffer, 0, sizeof(_buffer));
            int cnt = 0;
            for (; cnt < BUF_SIZE; cnt++)
            {
                if (!(isalnum(_peek) || _peek == '_'))
                    break;
                _buffer[cnt] = advance();
            }
            if (cnt == BUF_SIZE)
                throw LexerError("Exceed maximum identifier length");

            std::string str(_buffer);
            for (char &i : str)
                i = tolower(i); // case insensitive

            if (keywords.find(str) != keywords.end()) // keyword
                return Token(keywords[str]);
            else // identifier case sensitive
                return Token(Token::ID, std::string(_buffer));
        }
        // char
        else if (_peek == '"' || _peek == '\'')
        {
            char quote = _peek;
            advance();
            char c = advance();
            if (advance() != quote)
                throw LexerError("Bad char");
            return Token(Token::OPERAND, Variant(c));
        }
        // white space
        else if (isspace(_peek))
        {
            if (advance() == '\n')
            {
                _line++;
                _col = 1;
            }
        }
        // determine single character operators
        else if (singleOp.find(_peek) != singleOp.end())
        {
            return Token(singleOp[advance()]);
        }
        // operators
        else if (isOp(_peek))
        {
            memset(_buffer, 0, sizeof(_buffer));
            for (int cnt = 0; cnt < MAX_OP_SIZE; cnt++)
            {
                if (!isOp(_peek))
                    break;
                _buffer[cnt] = advance();
            }

            std::string str(_buffer);
            while (!str.empty() && ops.find(str) == ops.end()) // too long
            {
                char temp = str.back(); // last character
                str = str.substr(0, str.size() - 1);
                retreat(temp);
            }

            if (str.empty())
                throw LexerError("Invalid operator");

            return Token(ops[str]);
        }
        else // error
        {
            std::string msg = "Invalid lexeme ";
            msg.push_back(advance());
            throw LexerError(msg);
        }
    }
    // _peek == EOF
    advance();
    return Token(Token::END);
}

char Lexer::advance()
{
    char last = _stream.get();
    _peek = _stream.peek();
    _col++; // record column
    return last;
}

// put ch back to stream
void Lexer::retreat(char ch)
{
    _stream.putback(ch);
    _col--; // record column
    _peek = ch;
}