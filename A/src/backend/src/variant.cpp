#include "variant.h"

int Variant::compare(const Variant &v) const
{
    auto comType = commonType(_type, v.type());
    auto a = this->convertTo(comType);
    auto b = v.convertTo(comType);
    switch (a.type())
    {
    case INT:
        return (a.toInt() < b.toInt() ? -1 : a.toInt() > b.toInt());
    case BOOL:
        return (a.toBool() < b.toBool() ? -1 : a.toBool() > b.toBool());
    case CHAR:
        return (a.toChar() < b.toChar() ? -1 : a.toChar() > b.toChar());
    case DOUBLE:
        return (a.toDouble() < b.toDouble() ? -1 : a.toDouble() > b.toDouble());
    case NONE:
        return -1;
    case STRING:
        throw std::runtime_error("Cannot compare strings");
    default:
        throw std::runtime_error("Cannot compare types");
    }
}

std::ostream &operator<<(std::ostream &out, const Variant::Type &type)
{
    switch (type)
    {
    case Variant::BOOL:
        return out << "bool";
    case Variant::INT:
        return out << "int(11)";
    case Variant::DOUBLE:
        return out << "double";
    case Variant::CHAR:
        return out << "char(1)";
    case Variant::STRING:
        return out << "string";
    case Variant::NONE:
        return out << "NONE";
    default:
        return out;
    }
}

std::ostream &operator<<(std::ostream &os, const Variant &var)
{
    switch (var.type())
    {
    case Variant::INT:
        return os << var.toInt();
    case Variant::BOOL:
        return os << (var.toBool() ? "True" : "False");
    case Variant::CHAR:
        return os << var.toChar();
    case Variant::DOUBLE:
        os.setf(std::ios::fixed);
        return os << std::setprecision(4) << var.toDouble();
    case Variant::STRING:
        return os << var.toStdString();
    case Variant::NONE:
        return os << "NULL";
    default:
        return os;
    }
}

DataStream &operator<<(DataStream &ds, const Variant &var)
{
    ds << (char)var._type;
    switch (var._type)
    {
    case Variant::INT:
        return ds << var.toInt();
    case Variant::BOOL:
        return ds << (var.toBool() ? "True" : "False");
    case Variant::CHAR:
        return ds << var.toChar();
    case Variant::DOUBLE:
        return ds << var.toDouble();
        case Variant::STRING:
            return ds << var._str;
    default:
        return ds;
    }
}

DataStream &operator>>(DataStream &ds, Variant &var)
{
    char type;
    ds >> type;
    var._type = (Variant::Type)type;
    switch (var._type)
    {
    case Variant::INT:
        return ds >> var._data.i;
    case Variant::BOOL:
        return ds >> var._data.b;
    case Variant::CHAR:
        return ds >> var._data.c;
    case Variant::DOUBLE:
        return ds >> var._data.d;
        case Variant::STRING:
            return ds >> var._str;
    default:
        return ds;
    }
}

Variant Variant::convertTo(Variant::Type type) const
{
    switch (_type)
    {
    case INT:
        return convertTo(type, toInt());
    case DOUBLE:
        return convertTo(type, toDouble());
    case CHAR:
        return convertTo(type, toChar());
    case BOOL:
        return convertTo(type, toBool());
    case STRING:
        return Variant(toStdString());
    default:
        return Variant();
    }
}

Variant::Type Variant::commonType(Variant::Type a, Variant::Type b) const
{
    if (a == Variant::DOUBLE || b == Variant::DOUBLE)
        return Variant::DOUBLE;
    if (a == Variant::INT || b == Variant::INT)
        return Variant::INT;
    if (a == Variant::CHAR || b == Variant::CHAR)
        return Variant::CHAR;
    if (a == Variant::BOOL && b == Variant::BOOL)
        return Variant::BOOL;

    //throw std::runtime_error("Variant math operation failed");
}

Variant Variant::operator+(const Variant &v) const
{
    auto retType = commonType(_type, v._type);
    auto a = convertTo(retType);
    auto b = v.convertTo(retType);
    switch (a.type())
    {
    case INT:
        return a.toInt() + b.toInt();
    case DOUBLE:
        return a.toDouble() + b.toDouble();
    case CHAR:
        return a.toChar() + b.toChar();
    case BOOL:
        return a.toBool() + b.toBool();
    case NONE:
        return Variant();
    default:
        throw std::runtime_error("Operator + failed");
    }
}

Variant Variant::operator-(const Variant &v) const
{
    auto retType = commonType(_type, v._type);
    auto a = convertTo(retType);
    auto b = v.convertTo(retType);
    switch (a.type())
    {
    case INT:
        return a.toInt() - b.toInt();
    case DOUBLE:
        return a.toDouble() - b.toDouble();
    case CHAR:
        return a.toChar() - b.toChar();
    case BOOL:
        return a.toBool() - b.toBool();
    case NONE:
        return Variant();
    default:
        throw std::runtime_error("Operator - failed");
    }
}

Variant Variant::operator*(const Variant &v) const
{
    auto retType = commonType(_type, v._type);
    auto a = convertTo(retType);
    auto b = v.convertTo(retType);
    switch (a.type())
    {
    case INT:
        return a.toInt() * b.toInt();
    case DOUBLE:
        return a.toDouble() * b.toDouble();
    case CHAR:
        return a.toChar() * b.toChar();
    case BOOL:
        return a.toBool() * b.toBool();
    case NONE:
        return Variant();
    default:
        throw std::runtime_error("Operator - failed");
    }
}

Variant Variant::operator/(const Variant &v) const
{
    auto retType = commonType(_type, v._type);
    auto a = convertTo(retType);
    auto b = v.convertTo(retType);
    switch (a.type())
    {
    case INT:
        if(b.toInt() == 0)
            return Variant();
        return a.toInt() / b.toInt();
    case DOUBLE:
        if(b.toDouble() == 0)
            return Variant();
        return a.toDouble() / b.toDouble();
    case CHAR:
        return a.toChar() / b.toChar();
    case BOOL:
        return a.toBool() / b.toBool();
    case NONE:
        return Variant();
    default:
        throw std::runtime_error("Operator - failed");
    }
}
Variant Variant::operator%(const Variant &v) const
{
    auto retType = commonType(_type, v._type);
    auto a = convertTo(retType);
    auto b = v.convertTo(retType);
    switch (a.type())
    {
        case INT:
            if(b.toInt() == 0)
                return Variant();
            return a.toInt() % b.toInt();
        case DOUBLE:
            throw std::runtime_error("ERROR: Use % on double");
        case CHAR:
            throw std::runtime_error("ERROR: Use % on char");
        case BOOL:
            throw std::runtime_error("ERROR: Use % on bool");
        case NONE:
            return Variant();
        default:
            throw std::runtime_error("Operator % failed");
    }
}