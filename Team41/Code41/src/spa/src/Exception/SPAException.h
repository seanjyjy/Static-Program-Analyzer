#pragma once

#include <exception>
#include <string>

using namespace std;

class SPAException : public exception {
protected:
    string errorMessage;
public:
    using exception::what;
    virtual string what() = 0;

    explicit SPAException(string errorMessage);

    virtual ~SPAException() noexcept {}
};
