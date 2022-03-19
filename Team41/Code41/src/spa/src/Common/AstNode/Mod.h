#pragma once

#include "Common/AstNode/RelFactor.h"

class Mod : public RelFactor {
public:
    Mod(RelFactor *lhs, RelFactor *rhs);
};
