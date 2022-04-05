#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents term -> term '/' factor
 */
class Div : public RelFactor {
public:
    Div(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isDiv() const override;
    [[nodiscard]] bool isArithmeticOp() const;
};
