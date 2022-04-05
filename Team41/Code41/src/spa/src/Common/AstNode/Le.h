#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

/**
 * Represents rel_expr -> rel_factor '<=' rel_factor
 */
class Le : public RelExpr {
public:
    Le(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isLe() const override;
};
