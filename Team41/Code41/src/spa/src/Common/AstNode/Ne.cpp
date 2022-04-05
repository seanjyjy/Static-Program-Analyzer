#include "Ne.h"

Ne::Ne(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::ne, lhs, rhs) {
}

string Ne::toString() {
    return "!=";
}

bool Ne::isNe() const {
    return true;
}
