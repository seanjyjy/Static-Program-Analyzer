#include "Print.h"

Print::Print(VarName *var) : Stmt(TNodeType::printStmt, {static_cast<TNode *>(var)}) {
}

string Print::toString() {
    return "print";
}

bool Print::isPrint() const {
    return true;
}
