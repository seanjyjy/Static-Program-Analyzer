#pragma once
#include "SPAException.h"

using namespace std;

class SyntheticException : public SPAException {
public:
    SyntheticException(string message);
    string what() override;
    ~SyntheticException();
};