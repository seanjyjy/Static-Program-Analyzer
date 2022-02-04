#pragma once
#include "PQLTable.h"

class Evaluator {
public:
    virtual PQLTable* evaluate() const = 0;
};