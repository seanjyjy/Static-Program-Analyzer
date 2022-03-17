#pragma once

#import "WithVariable.h"

using namespace std;

class WithClause {
private:
    WithVariable left, right;
public:
    WithClause(WithVariable, WithVariable);
    WithVariable getLeft();
    WithVariable getRight();
};
