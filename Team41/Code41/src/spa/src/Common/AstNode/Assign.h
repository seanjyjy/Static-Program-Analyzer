#pragma once

#include "TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/VarName.h"

/**
 * Represents assign -> var_name '=' expr ';'
 */
class Assign : public Stmt {
public:
    Assign(VarName *var, TNode *expr);
    string toString() override;
    [[nodiscard]] bool isAssign() const override;
};
