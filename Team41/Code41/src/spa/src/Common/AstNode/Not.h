#pragma once

#include "TNode.h"
#include "CondExpr.h"

/**
 * Represents cond_expr -> '!' '(' cond_expr ')'
 */
class Not : public CondExpr {
public:
    Not(CondExpr *expr);
    string toString() override;
    [[nodiscard]] bool isNot() const override;
};
