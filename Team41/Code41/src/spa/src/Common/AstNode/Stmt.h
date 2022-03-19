#pragma once

#include "Common/TNode.h"
#include "Common/TNodeType.h"

class Stmt: public TNode {
private:
    static TNodeType verify(TNodeType type);
public:
    Stmt(TNodeType type, vector<TNode *> children);
};
