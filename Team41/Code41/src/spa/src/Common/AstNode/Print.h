#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/VarName.h"

class Print : public Stmt {
 public:
  Print(VarName *var);
};

