#include "Lt.h"

Lt::Lt(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::lt, lhs, rhs) {
}

string Lt::toString() {
    return "<";
}

bool Lt::isLt() const {
    return true;
}
