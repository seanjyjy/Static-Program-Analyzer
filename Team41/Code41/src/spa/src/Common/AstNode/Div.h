#pragma once

#include "Common/AstNode/BinaryOp.h"

class Div: public BinaryOp {
public:
    Div(TNode *lhs, TNode *rhs);
};
