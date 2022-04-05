#pragma once

#include "TNode.h"
#include "Common/AstNode/CondExpr.h"

/**
 * Represents cond_expr -> '(' cond_expr ')' '||' '(' cond_expr ')'
 */
class Or : public CondExpr {
public:
    Or(CondExpr *lhs, CondExpr *rhs);
    string toString() override;
    [[nodiscard]] bool isOr() const override;
};
