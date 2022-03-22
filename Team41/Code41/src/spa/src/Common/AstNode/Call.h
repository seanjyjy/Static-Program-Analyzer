#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/ProcName.h"

class Call : public Stmt {
 public:
  Call(ProcName *procedure);
};
