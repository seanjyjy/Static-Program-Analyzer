#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/VarName.h"

class Read : public Stmt {
public:
    Read(VarName *var);
};
