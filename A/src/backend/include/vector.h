#pragma once

#include <iostream>
#include <vector>

typedef long long Rank;

template <typename T>
class Vector : public std::vector<T>
{
  public:
    Vector() : std::vector<T>() {}
    Vector(size_t n) : std::vector<T>(n) {}
    Vector(size_t n, const T &val) : std::vector<T>(n, val) {}
    Vector(const Vector &v) : std::vector<T>(v) {}
    Vector(Vector &&v) noexcept : std::vector<T>(std::move(v)) {}
    Vector(const std::vector<T> &x) : std::vector<T>(x) {}
    Vector(std::initializer_list<T> il) : std::vector<T>(il) {}
    Vector &operator=(const Vector &v)
    {
        std::vector<T>::operator=(v);
        return (*this);
    }
    Vector &operator=(Vector &&v) noexcept
    {
        std::vector<T>::operator=(std::move(v));
        return (*this);
    }
    T remove(size_t r);
    void insert(size_t r, T const &val) { std::vector<T>::insert(std::vector<T>::begin() + r, val); }
    Rank search(T const &val);
};

template <typename T>
Rank Vector<T>::search(T const &val)
{
    if (std::vector<T>::empty())
        return -1;
    for (int i = 0; i < std::vector<T>::size(); i++)
        if (val > std::vector<T>::at(i))
            return i - 1;
    return std::vector<T>::size() - 1;
}
template <typename T>
T Vector<T>::remove(size_t r)
{
    T backup = std::vector<T>::at(r);
    std::vector<T>::erase(std::vector<T>::begin() + r);
    return backup;
}
template <typename T>
std::ostream &operator<<(std::ostream &out, const Vector<T> &vec)
{
    out << "Vector( ";
    for (auto &v : vec)
        out << v << ' ';
    return out << ')';
}