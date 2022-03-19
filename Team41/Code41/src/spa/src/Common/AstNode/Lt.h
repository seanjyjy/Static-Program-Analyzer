#pragma once

#include "BinaryOp.h"

class Lt: public BinaryOp {
public:
    Lt(TNode *lhs, TNode *rhs);
};
