#pragma once

#include <utility>
#include "variant.h"
#include <iostream>

/**
 * Field class provide methods to manage field information.
 *
 * Member `_key` specifies the key name.
 * Member `_type` specifies the field type. (e.g. int, double, char)
 * Member `_isNull` indicates whether the field is marked NOT NULL.
 * Member `_isPrimary` indicates whether the field is marked PRIMARY KEY.
 *
 * Input & output methods are implemented. Support std::cout and DataStream I/O.
 */
class Field
{
  public:
    Field() = default;
    Field(std::string key, Variant::Type type, bool isNull = true, bool isPrimary = false)
        : _key(std::move(key)), _type(type), _isNull(isNull), _isPrimary(isPrimary) {}
    const std::string &key() const { return _key; }
    Variant::Type type() const { return _type; }
    bool isNull() const { return _isNull; }
    bool isPrimary() const { return _isPrimary; }
    void setNull(bool isNull) { _isNull = isNull; }
    void setPrimary(bool isPrimary) { _isPrimary = isPrimary; }
    friend std::ostream &operator<<(std::ostream &out, const Field &f);
    friend DataStream &operator<<(DataStream &out, const Field &f);
    friend DataStream &operator>>(DataStream &in, Field &f);

  protected:
    std::string _key;
    Variant::Type _type = Variant::NONE;
    bool _isNull = true;
    bool _isPrimary = false;
};
