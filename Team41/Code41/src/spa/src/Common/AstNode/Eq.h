#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

/**
 * Represents rel_expr -> rel_factor '==' rel_factor
 */
class Eq : public RelExpr {
public:
    Eq(RelFactor *lhs, RelFactor *rhs);

    string toString() override;

    [[nodiscard]] bool isEq() const override;
};
