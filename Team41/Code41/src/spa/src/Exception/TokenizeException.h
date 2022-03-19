#pragma once

#include "SPAException.h"

using namespace std;

class TokenizeException : public SPAException {
public:
    TokenizeException();

    TokenizeException(string message);

    string what() override;

    ~TokenizeException();
};
