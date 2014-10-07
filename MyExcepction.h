#include "tools.h"

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

enum Error
{
    FormatError = 1,
    OverflowError = 2,
    UnderflowError = 3,
    LogicalError = 4
};

class MyExcepction
{
public :
    MyExcepction(Error e)
    {
        error = e;
    }
    string getError()
    {
        if(error == FormatError)return "FormatError";
        else if(error == OverflowError) return "OverflowError";
        else if(error == UnderflowError) return "UnderflowError";
        else return "LogicalError";
    }
    void setErrorInfo(string info)
    {
        errorInfo = info;
    }
    string getErrorInfo()
    {
        return errorInfo;
    }
private:
    Error error;
    string errorInfo;
};

#endif // ERROR_H_INCLUDED
