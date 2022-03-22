#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/Stmt.h"
#include <vector>

class StmtLst : public TNode {
 public:
  StmtLst(const vector<Stmt *> &stmts);
};
