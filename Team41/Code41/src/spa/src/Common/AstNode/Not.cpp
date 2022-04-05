#include "Not.h"

Not::Not(CondExpr *expr) : CondExpr(TNodeType::notOp, {static_cast<TNode *>(expr)}) {
}

string Not::toString() {
    return "!";
}

bool Not::isNot() const {
    return true;
}
