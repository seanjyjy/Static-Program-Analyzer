#pragma once

#include "Common/AstNode/RelFactor.h"

class Minus: public RelFactor {
public:
    Minus(RelFactor *lhs, RelFactor *rhs);
};
