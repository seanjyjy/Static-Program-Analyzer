#pragma once

#include "Common/AstNode/BinaryOp.h"

class Mod: public BinaryOp {
public:
    Mod(TNode *lhs, TNode *rhs);
};
