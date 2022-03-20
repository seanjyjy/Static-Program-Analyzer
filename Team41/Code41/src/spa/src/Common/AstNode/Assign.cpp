#include "Assign.h"

Assign::Assign(VarName *var, TNode *expr) : Stmt(TNodeType::assignStmt, {static_cast<TNode *>(var), expr}) {
}
