#pragma once

#include <algorithm>
#include <string>
#include <list>
#include <cstring>
#include <vector>
#include <dirent.h>
#include <iostream>
#include <fstream>
#ifdef WIN32
#include <io.h>
#include <direct.h>
#elif linux
#include <unistd.h>
#include <dirent.h>
#endif

/**
 * trim spaces at front IN PLACE
 * @param s input & output string
 */
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
}

/**
 * trim spaces at end IN PLACE
 * @param s input & output string
 */
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), s.end());
}

/**
  * trim spaces from both ends IN PLACE
  * @param s input & output string
  */
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

/**
 * convert the entire string to lower case
 * @param s input string
 * @return converted string in upper case
 */
static inline std::string toLower(const std::string &s)
{
    std::string ret(s);
    std::transform(s.begin(), s.end(), ret.begin(), ::tolower);
    return ret;
}

/**
 * conert the entire string to upper case
 * @param s input string
 * @return converted string in upper case
 */
static inline std::string toUpper(const std::string &s)
{
    std::string ret(s);
    std::transform(s.begin(), s.end(), ret.begin(), ::toupper);
    return ret;
}

/**
 * get random int in [0, n)
 * @param n upper bound
 * @return random int
 */
static inline int dice(int n)
{
    return rand() % n;
}

/**
 * split string with separator sep
 * @param s input string
 * @param sep separator
 * @return split string
 */
static std::vector<std::string> split(const std::string &s, char sep)
{
    std::vector<std::string> res;
    std::string tmp(s);
    while (true)
    {
        size_t firstSep = tmp.find_first_of(sep);
        if (firstSep == std::string::npos)
            break;
        res.push_back(tmp.substr(0, firstSep));
        tmp = tmp.substr(firstSep + 1);
    }
    res.push_back(tmp);
    return res;
}

/**
 * slice a vector
 * @tparam T data type
 * @param vec input vector
 * @param indices slice indices
 * @return sliced vector
 */
template <typename T>
static std::vector<T> slice(const std::vector<T> &vec, const std::vector<size_t> &indices)
{
    std::vector<T> res;
    for (auto &i : indices)
        res.emplace_back(vec[i]);
    return res;
}