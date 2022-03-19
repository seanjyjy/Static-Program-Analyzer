#pragma once

#include "Common/TNode.h"
#include "UnaryOp.h"

class Not: public UnaryOp {
public:
    Not(TNode *expr);
};
