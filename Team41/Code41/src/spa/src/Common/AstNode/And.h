#pragma once

#include "CondExpr.h"

/**
 * Represents cond_expr -> ( cond_expr ) && ( cond_expr )
 */
class And : public CondExpr {
public:
    And(CondExpr *lhsCondExpr, CondExpr *rhsCondExpr);
    string toString() override;
    [[nodiscard]] bool isAnd() const override;
};
