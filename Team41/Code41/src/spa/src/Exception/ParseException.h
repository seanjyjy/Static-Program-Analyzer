#pragma once

#include "SPAException.h"

using namespace std;

class ParseException: public SPAException {
public:
    ParseException();
    ParseException(string message);
    string what() override;
    ~ParseException();
};
