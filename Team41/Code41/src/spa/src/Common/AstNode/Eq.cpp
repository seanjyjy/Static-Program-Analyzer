#include "Eq.h"

Eq::Eq(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::eq, lhs, rhs) {}

string Eq::toString() {
    return "==";
}

bool Eq::isEq() const {
    return true;
}
