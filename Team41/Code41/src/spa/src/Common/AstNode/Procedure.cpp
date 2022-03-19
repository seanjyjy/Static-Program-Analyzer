#include "Procedure.h"

Procedure::Procedure(Token *name, StmtLst *stmtLst): TNode(TNodeType::procedure, name, {static_cast<TNode*>(stmtLst)}) {
}
