#include "Le.h"

Le::Le(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::le, lhs, rhs) {
}

string Le::toString() {
    return "<=";
}

bool Le::isLe() const {
    return true;
}
