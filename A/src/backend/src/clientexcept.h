#pragma once

#include <exception>
#include <string>

class ClientError : public std::exception
{
public:
    ClientError(const std::string &msg) : _msg("Client Error: " + msg) {}
    virtual ~ClientError() throw() {}
    const char *what() const throw() { return _msg.c_str(); }

protected:
    std::string _msg;
};