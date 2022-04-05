#include "Gt.h"

Gt::Gt(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::gt, lhs, rhs) {}

string Gt::toString() {
    return ">";
}

bool Gt::isGt() const {
    return true;
}
