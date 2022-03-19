#pragma once

#include "Common/TNode.h"
#include "Common/TNodeType.h"

class Stmt: public TNode {
public:
    Stmt(TNodeType type, vector<TNode *> children);
};
