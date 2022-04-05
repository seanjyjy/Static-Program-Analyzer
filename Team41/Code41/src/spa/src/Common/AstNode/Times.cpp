#include "Times.h"

Times::Times(RelFactor *lhs, RelFactor *rhs) : RelFactor(TNodeType::times, nullptr, {lhs, rhs}) {
}

string Times::toString() {
    return "*";
}

bool Times::isTimes() const {
    return true;
}

bool Times::isArithmeticOp() const {
    return true;
}
