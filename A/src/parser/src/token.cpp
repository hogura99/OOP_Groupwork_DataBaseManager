#include "token.h"

std::map<Token::Type, std::string> Token::name{
    {Token::ID, "ID"},
    {Token::OPERAND, "OPERAND"},
    {Token::CREATE, "CREATE"},
    {Token::TABLE, "TABLE"},
    {Token::INT, "INT"},
    {Token::DOUBLE, "DOUBLE"},
    {Token::CHAR, "CHAR"},
    {Token::DEFAULT, "DEFAULT"},
    {Token::PRIMARY, "PRIMARY"},
    {Token::KEY, "KEY"},
    {Token::INSERT, "INSERT"},
    {Token::INTO, "INTO"},
    {Token::VALUES, "VALUES"},
    {Token::DELETE, "DELETE"},
    {Token::FROM, "FROM"},
    {Token::WHERE, "WHERE"},
    {Token::SELECT, "SELECT"},
    {Token::LT, "LT"},
    {Token::GT, "GT"},
    {Token::NEQ, "NEQ"},
    {Token::EQ, "EQ"},
    {Token::GEQ, "GEQ"},
    {Token::LEQ, "LEQ"},
    {Token::PLUS, "PLUS"},
    {Token::MINUS, "MINUS"},
    {Token::MUL, "MUL"},
    {Token::DIV, "DIV"},
    {Token::AND, "AND"},
    {Token::OR, "OR"},
    {Token::NOT, "NOT"},
    {Token::L_PAREN, "L_PAREN"},
    {Token::R_PAREN, "R_PAREN"},
    {Token::COMMA, "COMMA"},
    {Token::SEMICOLON, "SEMICOLON"},
    {Token::END, "END"},
    {Token::NONE, "NONE"},
    {Token::INFILE, "INFILE"},
    {Token::OUTFILE, "OUTFILE"},
    {Token::GROUP, "GROUP"},
    {Token::BY, "BY"},
    {Token::ORDER, "ORDER"},
    {Token::COUNT, "COUNT"},
    {Token::LOAD, "LOAD"},
};

Token::Token(Type type, const Variant &data)
{
    if ((type == Token::ID || type == Token::OPERAND) && data.type() == Variant::NONE) // number or identifier
        throw TokenError("Expected non-keyword");
    _type = type;
    _data = data;
}

// identifiers
const std::string &Token::toId() const
{
    if (_type != ID)
        throw TokenError("Expected identifer, get otherwise");
    return _data.toStdString();
}

// keywords and operators
Token::Type Token::toKeyword() const
{
    if (_type == Token::ID || _type == Token::OPERAND)
        throw TokenError("Expected keywords, get otherwise");
    return _type;
}

// operands
Variant Token::toOperand() const
{
    if (_type != OPERAND)
        throw TokenError("Expected number, get otherwise");
    return _data;
}

std::ostream &operator<<(std::ostream &s, const Token &token)
{
    Token::Type _type = token._type;
    std::string token_name = Token::name[_type];

    switch (_type)
    {
    case Token::ID:
        return s << "(" << token_name << ", " << token.toId() << ")";
    case Token::OPERAND:
        return s << "(" << token_name << ", " << token.toOperand() << ")";
    default:
        return s << token_name;
    }
}

Token &Token::operator=(const Token &rhs)
{
    _type = rhs._type;
    _data = rhs._data;
    return *this;
}
Token &Token::operator=(Token &&rhs)
{
    _type = std::move(rhs._type);
    _data = std::move(rhs._data);
    return *this;
}
