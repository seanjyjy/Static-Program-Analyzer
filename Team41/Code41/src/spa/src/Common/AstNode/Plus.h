#pragma once

#include "Common/AstNode/RelFactor.h"

class Plus : public RelFactor {
 public:
  Plus(RelFactor *lhs, RelFactor *rhs);
};
