#include "ProcName.h"

ProcName::ProcName(Token *val) : TNode(TNodeType::procName, val, {}) {
}

string ProcName::toString() {
    return "procName " + getTokenVal();
}

bool ProcName::isProcName() const {
    return true;
}
