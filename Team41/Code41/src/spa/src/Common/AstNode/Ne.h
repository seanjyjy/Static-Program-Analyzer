#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

class Ne : public RelExpr {
public:
    Ne(RelFactor *lhs, RelFactor *rhs);
};
