#pragma once

#include "Common/AstNode/RelFactor.h"

/**
 * Represents proc_name -> NAME
 */
class ProcName : public TNode {
public:
    ProcName(Token *val);
    string toString() override;
    [[nodiscard]] bool isProcName() const override;
};
