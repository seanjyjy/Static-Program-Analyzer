#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/StmtLst.h"

class If : public Stmt {
 public:
  If(TNode *condExpr, StmtLst *ifStmtLst, StmtLst *elseStmtLst);
};
