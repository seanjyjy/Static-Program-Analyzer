#pragma once

#include "TNode.h"
#include "Common/AstNode/Stmt.h"
#include <vector>

/**
 * Represents stmtLst -> stmt+
 */
class StmtLst : public TNode {
public:
    StmtLst(const vector<Stmt *> &stmts);
    string toString() override;
    [[nodiscard]] bool isStmtLst() const override;
};
