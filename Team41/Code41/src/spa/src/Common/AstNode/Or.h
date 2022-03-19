#pragma once

#include "Common/AstNode/BinaryOp.h"

class Or: public BinaryOp {
public:
    Or(TNode *lhsCondExpr, TNode *rhsCondExpr);
};
