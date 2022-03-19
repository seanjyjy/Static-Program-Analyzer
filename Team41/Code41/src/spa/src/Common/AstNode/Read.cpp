#include "Read.h"

Read::Read(VarName *var): Stmt(TNodeType::readStmt, {static_cast<TNode*>(var)}) {
}
