#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

class Ge : public RelExpr {
 public:
  Ge(RelFactor *lhs, RelFactor *rhs);
};
