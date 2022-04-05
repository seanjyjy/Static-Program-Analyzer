#include "While.h"

While::While(TNode *condExpr, StmtLst *stmtLst) : Stmt(TNodeType::whileStmt,
                                                       {condExpr, static_cast<TNode *>(stmtLst)}) {
}

string While::toString() {
    return "while";
}

bool While::isWhile() const {
    return true;
}
