#include "If.h"

If::If(TNode *condExpr, StmtLst *ifStmtLst, StmtLst *elseStmtLst): Stmt(TNodeType::ifStmt, {condExpr, static_cast<TNode*>(ifStmtLst), static_cast<TNode*>(elseStmtLst)}) {
}
