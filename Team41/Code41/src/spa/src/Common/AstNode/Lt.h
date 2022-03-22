#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

class Lt : public RelExpr {
 public:
  Lt(RelFactor *lhs, RelFactor *rhs);
};
