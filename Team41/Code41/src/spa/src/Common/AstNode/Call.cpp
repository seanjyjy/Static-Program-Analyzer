#include "Call.h"

Call::Call(ProcName *procedure) : Stmt(TNodeType::callStmt, {static_cast<TNode *>(procedure)}) {}

string Call::toString() {
    return "call";
}

bool Call::isCall() const {
    return true;
}
