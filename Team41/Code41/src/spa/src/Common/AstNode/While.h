#pragma once

#include "TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/StmtLst.h"

/**
 * Represents while -> 'while' '(' cond_expr ')' '{' stmtLst '}'
 */
class While : public Stmt {
public:
    While(TNode *condExpr, StmtLst *stmtLst);
    string toString() override;
    [[nodiscard]] bool isWhile() const override;
};
