#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <regex>

static bool regMatch(const std::string &str, const std::string &pattern)
{
    std::regex reg(pattern);
    return std::regex_match(str, reg);
}

static bool sqlLike(const std::string &str, const std::string &pattern)
{
    bool inBrack = false;
    std::string _pattern;
    std::stringstream token(pattern);
    while (!token.eof())
    {
        char c = token.get();
        if (token.eof())
            break;
        switch (c)
        {
            case '%':
                _pattern += ".*";
                break;
            case '_':
                _pattern += ".{1}";
                break;
            case '[':
                inBrack = true;
                _pattern += '[';
                if (token.peek() == '^' || token.peek() == '!')
                    _pattern += token.get();
                _pattern += token.get();
                break;
            case ']':
                inBrack = false;
                _pattern += ']';
                break;
            case '^':
            case '!':
                _pattern += c;
                break;
            default:
                if (inBrack)
                    _pattern += '|';
                _pattern += c;
                break;
        }
    }
    return regMatch(str, _pattern);
}