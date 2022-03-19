#pragma once

#include "BinaryOp.h"

class And: public BinaryOp {
public:
    And(TNode *lhsCondExpr, TNode *rhsCondExpr);
};
