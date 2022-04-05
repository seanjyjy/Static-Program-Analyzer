#include "Read.h"

Read::Read(VarName *var) : Stmt(TNodeType::readStmt, {static_cast<TNode *>(var)}) {
}

string Read::toString() {
    return "read";
}

bool Read::isRead() const {
    return true;
}
