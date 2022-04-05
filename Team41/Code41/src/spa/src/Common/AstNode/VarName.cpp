#include "VarName.h"

VarName::VarName(Token *val) : RelFactor(TNodeType::varName, val, {}) {
}

string VarName::toString() {
    return "varName " + getTokenVal();
}

bool VarName::isVarName() const {
    return true;
}
