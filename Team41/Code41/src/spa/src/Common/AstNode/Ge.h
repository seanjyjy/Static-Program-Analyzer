#pragma once

#include "BinaryOp.h"

class Ge: public BinaryOp {
public:
    Ge(TNode *lhs, TNode *rhs);
};
