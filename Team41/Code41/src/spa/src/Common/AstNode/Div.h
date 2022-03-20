#pragma once

#include "Common/AstNode/RelFactor.h"

class Div : public RelFactor {
public:
    Div(RelFactor *lhs, RelFactor *rhs);
};
