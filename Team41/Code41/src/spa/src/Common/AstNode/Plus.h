#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents expr -> expr '+' term
 */
class Plus : public RelFactor {
public:
    Plus(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isPlus() const override;
};
