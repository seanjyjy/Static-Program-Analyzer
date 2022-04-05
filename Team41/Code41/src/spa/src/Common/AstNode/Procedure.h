#pragma once

#include "TNode.h"
#include "Common/AstNode/StmtLst.h"

/**
 * Represents procedure -> 'procedure' proc_name '{' stmtLst '}'
 */
class Procedure : public TNode {
public:
    Procedure(Token *name, StmtLst *stmtLst);
    string toString() override;
    [[nodiscard]] bool isProcedure() const override;
};
