#pragma once

#include "TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/VarName.h"

/**
 * Represents print -> 'print' var_name ';'
 */
class Print : public Stmt {
public:
    Print(VarName *var);

    string toString() override;

    [[nodiscard]] bool isPrint() const override;
};

