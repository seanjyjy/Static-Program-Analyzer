#pragma once

#include "Common/AstNode/BinaryOp.h"

class Minus: public BinaryOp {
public:
    Minus(TNode *lhs, TNode *rhs);
};
