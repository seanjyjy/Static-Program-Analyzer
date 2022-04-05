#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

/**
 * Represents rel_expr -> rel_factor '<' rel_factor
 */
class Lt : public RelExpr {
public:
    Lt(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isLt() const override;
};
