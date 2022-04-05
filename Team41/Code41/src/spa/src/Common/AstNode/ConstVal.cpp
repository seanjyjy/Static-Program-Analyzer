#include "ConstVal.h"

ConstVal::ConstVal(Token *val) : RelFactor(TNodeType::constValue, val, {}) {
}

string ConstVal::toString() {
    return "constValue " + getTokenVal();
}

bool ConstVal::isConstVal() const {
    return true;
}
