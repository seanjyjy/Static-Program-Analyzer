#pragma once
#include "SPAException.h"

using namespace std;

class SyntaxException : public SPAException {
public:
    SyntaxException();
    SyntaxException(string message);
    string what() override;
    ~SyntaxException();
};