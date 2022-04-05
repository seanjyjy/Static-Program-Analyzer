#include "Ge.h"

Ge::Ge(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::ge, lhs, rhs) {}

string Ge::toString() {
    return ">=";
}

bool Ge::isGe() const {
    return true;
}
