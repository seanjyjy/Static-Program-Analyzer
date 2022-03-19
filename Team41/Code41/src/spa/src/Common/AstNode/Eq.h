#pragma once

#include "BinaryOp.h"

class Eq: public BinaryOp {
public:
    Eq(TNode *lhs, TNode *rhs);
};
