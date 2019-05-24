#pragma once

class DatabaseError : public std::exception
{
  public:
    DatabaseError(const std::string &msg) : _msg("Database Error: " + msg) {}
    virtual ~DatabaseError() throw() {}
    const char *what() const throw() { return _msg.c_str(); }

  protected:
    std::string _msg;
};