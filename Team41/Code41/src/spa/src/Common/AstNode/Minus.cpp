#include "Minus.h"

Minus::Minus(RelFactor *lhs, RelFactor *rhs) : RelFactor(TNodeType::minus, nullptr, {lhs, rhs}) {
}

string Minus::toString() {
    return "-";
}

bool Minus::isMinus() const {
    return true;
}
