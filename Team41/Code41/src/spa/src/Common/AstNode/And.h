#pragma once

#include "CondExpr.h"

class And : public CondExpr {
public:
    And(CondExpr *lhsCondExpr, CondExpr *rhsCondExpr);
};
