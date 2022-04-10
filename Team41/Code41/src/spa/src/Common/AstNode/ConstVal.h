#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents const_value -> INTEGER
 */
class ConstVal : public RelFactor {
public:
    ConstVal(Token *val);

    string toString() override;

    [[nodiscard]] bool isConstVal() const override;
};
