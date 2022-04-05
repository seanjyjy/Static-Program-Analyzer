#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents var_name -> NAME
 */
class VarName : public RelFactor {
public:
    VarName(Token *val);
    string toString() override;
    [[nodiscard]] bool isVarName() const override;
};
