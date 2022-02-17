#pragma once
#include "SPAException.h"

using namespace std;

class SemanticException : public SPAException {
public:
    SemanticException(string errorMessage);
    string what() override;
    ~SemanticException();
};

