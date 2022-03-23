#pragma once

#include "WithVariable.h"

using namespace std;

class WithClause {
private:
    WithVariable left, right;
public:
    WithClause();
    WithClause(WithVariable, WithVariable);
    WithVariable getLeft() const;
    WithVariable getRight() const;
};
