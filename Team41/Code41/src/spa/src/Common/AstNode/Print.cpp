#include "Print.h"

Print::Print(VarName *var) : Stmt(TNodeType::printStmt, {static_cast<TNode *>(var)}) {
}
