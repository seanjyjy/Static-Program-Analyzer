#include "RelExpr.h"

TNodeType RelExpr::verify(TNodeType type) {
    if (type != TNodeType::gt &&
        type != TNodeType::ge &&
        type != TNodeType::lt &&
        type != TNodeType::le &&
        type != TNodeType::eq &&
        type != TNodeType::ne) {
        throw runtime_error("invalid node type for relexpr");
    }
    return type;
}

RelExpr::RelExpr(TNodeType type, RelFactor *lhs, RelFactor *rhs) : CondExpr(RelExpr::verify(type), {lhs, rhs}) {
}

// only leaf nodes in the inheritance chain should override toString
string RelExpr::toString() {
    throw runtime_error("toString() not implemented for RelExpr");
}

