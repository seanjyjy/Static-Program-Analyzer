#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents term -> term '%' factor
 */
class Mod : public RelFactor {
public:
    Mod(RelFactor *lhs, RelFactor *rhs);
    string toString() override;
    [[nodiscard]] bool isMod() const override;
    [[nodiscard]] bool isArithmeticOp() const override;
};
