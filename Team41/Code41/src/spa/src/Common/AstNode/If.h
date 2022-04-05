#pragma once

#include "TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/StmtLst.h"

/**
 * Represents if -> 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
 */
class If : public Stmt {
public:
    If(TNode *condExpr, StmtLst *ifStmtLst, StmtLst *elseStmtLst);
    string toString() override;
    [[nodiscard]] bool isIf() const override;
};
