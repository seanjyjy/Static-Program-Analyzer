#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

class Le : public RelExpr {
 public:
  Le(RelFactor *lhs, RelFactor *rhs);
};
