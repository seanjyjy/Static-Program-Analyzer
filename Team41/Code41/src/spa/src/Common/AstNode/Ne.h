#pragma once

#include "BinaryOp.h"

class Ne: public BinaryOp {
public:
    Ne(TNode *lhs, TNode *rhs);
};
