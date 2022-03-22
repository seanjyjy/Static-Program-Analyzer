#pragma once

#include "Common/TNode.h"
#include "Common/AstNode/StmtLst.h"

class Procedure : public TNode {
public:
    Procedure(Token *name, StmtLst *stmtLst);
};
