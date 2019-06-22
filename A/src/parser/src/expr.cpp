#include "expr.h"
#include "../../backend/src/regexpr.h"

Expr::Expr(const Expr &other) : _token(other._token)
{
    if (other._left)
        setLeft(*other._left);
    if (other._right)
        setRight(*other._right);
}

Expr::Expr(Expr &&other) : _token(std::move(other._token))
{
    _left = other._left;
    other._left = nullptr;
    _right = other._right;
    other._right = nullptr;
}

Expr::Expr(const Token &token, const Expr &left, const Expr &right) : _token(token)
{
    setLeft(left);
    setRight(right);
}

void Expr::setLeft(const Expr &other)
{
    delete _left;
    _left = new Expr(other);
}

void Expr::setRight(const Expr &other)
{
    delete _right;
    _right = new Expr(other);
}

Expr &Expr::operator=(const Expr &rhs)
{
    if (rhs._left)
    { // other has a _left
        setLeft(*rhs._left);
    }
    else
    { // others doesn't have _left
        delete _left;
        _left = nullptr;
    }

    if (rhs._right != nullptr)
    {
        setRight(*rhs._right);
    }
    else
    {
        delete _right;
        _right = nullptr;
    }

    _token = rhs._token;
    return *this;
}

std::ostream &operator<<(std::ostream &s, const Expr &expr)
{
    if (expr.isValue())
    { // id or number
        s << expr.token();
        return s;
    }

    // real expression
    s << "(";
    if (expr.hasLeft())
        s << expr.left();

    if (!expr.isNull()) // operator
        s << " " << Token(expr.tokenType()) << " ";

    if (expr.hasRight())
        s << expr.right();

    s << ")";
    return s;
}

Variant Expr::eval(const std::map<std::string, Variant> &varMap) const
{
    switch (_token.type())
    {
        case Token::NONE:
            return true;
        case Token::OPERAND:
            return _token.toOperand();
        case Token::ID:
            return varMap.at(_token.toId());
        case Token::AND:
            if (_left->eval(varMap).type() == Variant::NONE || _right->eval(varMap).type() == Variant::NONE)
                return Variant();
            return _left->eval(varMap).toBool() && _right->eval(varMap).toBool();
        case Token::OR:
            if (_left->eval(varMap).type() == Variant::NONE || _right->eval(varMap).type() == Variant::NONE)
                return Variant();
            return _left->eval(varMap).toBool() || _right->eval(varMap).toBool();
        case Token::NOT:
            if (_right->eval(varMap).type() == Variant::NONE)
                return Variant();
            return !_right->eval(varMap).toBool();
        case Token::XOR:
            if (_left->eval(varMap).type() == Variant::NONE || _right->eval(varMap).type() == Variant::NONE)
                return Variant();
            return _left->eval(varMap).toBool() xor _right->eval(varMap).toBool();
        case Token::GT:
            return _left->eval(varMap) > _right->eval(varMap);
        case Token::LT:
            return _left->eval(varMap) < _right->eval(varMap);
        case Token::EQ:
            return _left->eval(varMap) == _right->eval(varMap);
        case Token::NEQ:
            return _left->eval(varMap) != _right->eval(varMap);
        case Token::GEQ:
            return _left->eval(varMap) >= _right->eval(varMap);
        case Token::LEQ:
            return _left->eval(varMap) <= _right->eval(varMap);
        case Token::PLUS:
            return _left->eval(varMap) + _right->eval(varMap);
        case Token::MINUS:
            return _left->eval(varMap) - _right->eval(varMap);
        case Token::MUL:
            return _left->eval(varMap) * _right->eval(varMap);
        case Token::DIV:
            return _left->eval(varMap) / _right->eval(varMap);
        case Token::MOD:
            return _left->eval(varMap) % _right->eval(varMap);
        case Token::LIKE:
            return sqlLike(_left->eval(varMap).toStdString(), _right->eval(varMap).toStdString());
        default:
            throw std::runtime_error("Expr eval fail");

    }
}

Expr::~Expr()
{
    delete _left;
    delete _right;
    _left = _right = nullptr;
}
