#pragma once

#include "Common/AstNode/BinaryOp.h"

class Gt: public BinaryOp {
public:
    Gt(TNode *lhs, TNode *rhs);
};
