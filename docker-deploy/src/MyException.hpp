#ifndef __MYEXCEPTION_H__
#define __MYEXCEPTION_H__

#include <exception>
#include <stdexcept>
#include <string>

class MyException : public std::exception {
public:
    std::string errMsg;
    MyException(const std::string &errDescr){
        errMsg.append(errDescr);
        errMsg.append(" ");
    }
    //virtual ~MyException() _NOEXCEPT{}
    virtual const char * what() const throw() {
        return errMsg.c_str();
    }
};

#endif
