#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include "datastream.h"

/**
 * Variant class unifies variants of different types into one class.
 *
 * Construct Variant directly with data of basic types (bool, char, int, double, std::string),
 * get data type with `type()`, retrieve data with `toBool()`, `toChar()`, `toInt()`, etc.
 *
 * Input & output methods are implemented. Support std::cout and DataStream I/O.
 */
class Variant
{
  public:
    enum Type
    {
        NONE,
        INT,
        BOOL,
        CHAR,
        DOUBLE,
        STRING
    };
    Variant() = default;
    Variant(const Variant &v) = default;

    Type type() const { return _type; }

    // constructors
    Variant(int i) : _type(INT) { _data.i = i; }
    Variant(bool b) : _type(BOOL) { _data.b = b; }
    Variant(char c) : _type(CHAR) { _data.c = c; }
    Variant(double d) : _type(DOUBLE) { _data.d = d; }
    Variant(const std::string &s) : _type(STRING) { _str = s; }
    virtual ~Variant() = default;

    // data retriever
    bool toBool() const { return _data.b; }
    int toInt() const { return _data.i; }
    char toChar() const { return _data.c; }
    double toDouble() const { return _data.d; }
    const std::string &toStdString() const { return _str; }

    // assign
    Variant &operator=(const Variant &v) = default;
    Variant &operator=(Variant &&v) = default;

    // operators
    bool operator<(const Variant &v) const { return compare(v) < 0; }
    bool operator==(const Variant &v) const { return compare(v) == 0; }
    bool operator>(const Variant &v) const { return compare(v) > 0; }
    bool operator!=(const Variant &v) const { return compare(v) != 0; }
    bool operator>=(const Variant &v) const { return compare(v) >= 0; }
    bool operator<=(const Variant &v) const { return compare(v) <= 0; }

    Variant operator+(const Variant &v) const;
    Variant operator-(const Variant &v) const;
    Variant operator*(const Variant &v) const;
    Variant operator/(const Variant &v) const;
    Variant operator%(const Variant &v) const;
    /**
     * convert data to specific type
     * @tparam T data type
     * @param type destination Variant type
     * @param data input data
     * @return data of specific type. return null variant if conversion failed.
     */
    template <typename T>
    Variant convertTo(Type type, T data) const;

    /**
     * convert *this to variant of specific type
     * @param type destination Variant type
     * @return data of specific type. return null variant if conversion failed.
     */
    Variant convertTo(Type type) const;

    /**
     * Compare *this and variant v
     * @param v another instance of variant
     * @return 1 if *this > v, 0 if *this == v, -1 if *this < v
     */
    int compare(const Variant &v) const;

    // output methods
    friend std::ostream &operator<<(std::ostream &out, const Type &type);
    friend std::ostream &operator<<(std::ostream &out, const Variant &var);
    friend DataStream &operator<<(DataStream &ds, const Variant &var);
    friend DataStream &operator>>(DataStream &ds, Variant &var);

  protected:
    Type commonType(Type a, Type b) const;

  protected:
    Type _type = NONE;
    union {
        int i;
        bool b;
        char c;
        double d;
    } _data;
    std::string _str;
};

template <typename T>
Variant Variant::convertTo(Variant::Type type, T data) const
{
    switch (type)
    {
    case Variant::INT:
        return Variant((int)data);
    case Variant::DOUBLE:
        return Variant((double)data);
    case Variant::CHAR:
        return Variant((char)data);
    case Variant::BOOL:
        return Variant((bool)data);
    case Variant::STRING:
        return Variant(data);
    default:
        return Variant();
    }
}