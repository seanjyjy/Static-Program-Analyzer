#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

/**
 * Represents rel_expr -> rel_factor '>' rel_factor
 */
class Gt : public RelExpr {
public:
    Gt(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isGt() const override;
};
