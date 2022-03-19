#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

class Eq: public RelExpr {
public:
    Eq(RelFactor *lhs, RelFactor *rhs);
};
