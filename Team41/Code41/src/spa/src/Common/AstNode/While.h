#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/StmtLst.h"

class While: public Stmt {
public:
    While(TNode *condExpr, StmtLst *stmtLst);
};
