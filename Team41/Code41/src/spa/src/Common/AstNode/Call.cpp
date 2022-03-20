#include "Call.h"

Call::Call(ProcName *procedure) : Stmt(TNodeType::callStmt, {static_cast<TNode *>(procedure)}) {
}
