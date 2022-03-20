#pragma once

#include <exception>
#include <string>

using namespace std;

class SPAException : public exception {
protected:
    string errorMessage;
public:

    virtual string what() = 0;

    SPAException(string errorMessage);

    virtual ~SPAException() noexcept {}
};
