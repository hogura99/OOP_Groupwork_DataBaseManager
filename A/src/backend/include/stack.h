#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class Stack : public std::vector<T>
{
  public:
    Stack() = default;
    void push(const T &val) { this->push_back(val); } // push the element into stack
    T pop();                                          // pop and return the popped element
    T &top() { return this->back(); }                 // return element on top
    void reverse() { std::reverse(this->begin(), this->end()); }
};

template <typename T>
T Stack<T>::pop()
{
    T backup = this->back();
    this->pop_back();
    return backup;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Stack<T> &stk) // output function
{
    os << "Stack( ";
    for (auto &s : stk)
        os << s << ' ';
    return os << ')';
}