#pragma once

#include <exception>
#include <string>

class ServerError : public std::exception
{
public:
    ServerError(const std::string &msg) : _msg("Server Error: " + msg) {}
    virtual ~ServerError() throw() {}
    const char *what() const throw() { return _msg.c_str(); }

protected:
    std::string _msg;
};