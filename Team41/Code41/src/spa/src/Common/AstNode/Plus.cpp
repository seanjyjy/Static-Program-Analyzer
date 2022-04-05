#include "Plus.h"

Plus::Plus(RelFactor *lhs, RelFactor *rhs) : RelFactor(TNodeType::plus, nullptr, {lhs, rhs}) {
}

string Plus::toString() {
    return "+";
}

bool Plus::isPlus() const {
    return true;
}

bool Plus::isArithmeticOp() const {
    return true;
}
