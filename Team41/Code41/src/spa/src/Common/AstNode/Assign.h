#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/VarName.h"

class Assign : public Stmt {
 public:
  Assign(VarName *var, TNode *expr);
};
