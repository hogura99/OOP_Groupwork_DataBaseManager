
#include "field.h"

std::ostream &operator<<(std::ostream &out, const Field &f)
{
    return out << f._key << '\t' << f._type << '\t' << (f._isNull ? "YES" : "NO") << '\t'
               << (f._isPrimary ? "PRI" : "") << "\tNULL";
}

DataStream &operator<<(DataStream &out, const Field &f)
{
    return out << f._key << f._type << f._isNull << f._isPrimary;
}

DataStream &operator>>(DataStream &in, Field &f)
{
    return in >> f._key >> f._type >> f._isNull >> f._isPrimary;
}
