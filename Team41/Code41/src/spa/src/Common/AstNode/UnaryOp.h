#pragma once

#include "Common/TNode.h"

class UnaryOp: public TNode {
public:
    UnaryOp(TNodeType type, TNode *expr);
};
