#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/CondExpr.h"

class Or : public CondExpr {
 public:
  Or(CondExpr *lhs, CondExpr *rhs);
};
