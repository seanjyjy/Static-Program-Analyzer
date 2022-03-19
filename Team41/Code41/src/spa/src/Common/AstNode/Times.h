#pragma once

#include "Common/AstNode/RelFactor.h"

class Times: public RelFactor {
public:
    Times(RelFactor *lhs, RelFactor *rhs);
};
