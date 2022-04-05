#include "Procedure.h"

Procedure::Procedure(Token *name, StmtLst *stmtLst) : TNode(TNodeType::procedure, name,
                                                            {static_cast<TNode *>(stmtLst)}) {
}

string Procedure::toString() {
    return "procedure";
}

bool Procedure::isProcedure() const {
    return true;
}
