#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

/**
 * Represents rel_expr -> rel_factor '>=' rel_factor
 */
class Ge : public RelExpr {
public:
    Ge(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isGe() const override;
};
