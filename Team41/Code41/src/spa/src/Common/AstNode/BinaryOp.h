#pragma once

#include "Common/TNode.h"
#include "Common/TNodeType.h"

class BinaryOp: public TNode {
public:
    BinaryOp(TNodeType type, TNode *lhs, TNode *rhs);
};
