#pragma once

#include "Common/TNode.h"
#include "CondExpr.h"

class Not : public CondExpr {
 public:
  Not(CondExpr *expr);
};
