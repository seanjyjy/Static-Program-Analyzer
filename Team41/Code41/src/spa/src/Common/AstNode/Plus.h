#pragma once

#include "Common/AstNode/BinaryOp.h"

class Plus: public BinaryOp {
public:
    Plus(TNode *lhs, TNode *rhs);
};
