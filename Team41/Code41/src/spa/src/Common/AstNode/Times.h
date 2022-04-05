#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents term -> term '*' factor
 */
class Times : public RelFactor {
public:
    Times(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isTimes() const override;
    [[nodiscard]] bool isArithmeticOp() const;
};
