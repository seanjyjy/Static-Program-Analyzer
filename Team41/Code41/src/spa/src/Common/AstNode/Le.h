#pragma once

#include "BinaryOp.h"

class Le: public BinaryOp {
public:
    Le(TNode *lhs, TNode *rhs);
};
