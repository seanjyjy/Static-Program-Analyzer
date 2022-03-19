#pragma once

#include "Common/AstNode/BinaryOp.h"

class Times: public BinaryOp {
public:
    Times(TNode *lhs, TNode *rhs);
};
