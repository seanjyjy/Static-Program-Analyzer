#pragma once

#include "Common/AstNode/CondExpr.h"
#include "Common/AstNode/RelFactor.h"
#include "Common/TNodeType.h"
#include <stdexcept>
#include <utility>

using namespace std;

class RelExpr: public CondExpr {
private:
    static TNodeType verify(TNodeType type);
public:
    RelExpr(TNodeType type, RelFactor *lhs, RelFactor *rhs);
};
