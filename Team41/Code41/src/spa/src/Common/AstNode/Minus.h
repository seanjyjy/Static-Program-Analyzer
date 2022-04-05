#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents expr -> expr '-' term
 */
class Minus : public RelFactor {
public:
    Minus(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isMinus() const override;
};
