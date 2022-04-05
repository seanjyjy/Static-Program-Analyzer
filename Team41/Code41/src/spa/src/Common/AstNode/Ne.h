#pragma once

#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

/**
 * Represents rel_expr -> rel_factor '!=' rel_factor
 */
class Ne : public RelExpr {
public:
    Ne(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isNe() const override;
};
