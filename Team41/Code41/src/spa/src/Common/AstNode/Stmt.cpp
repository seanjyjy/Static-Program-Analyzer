#include "Stmt.h"

#include <stdexcept>
#include <utility>

TNodeType Stmt::verify(TNodeType type) {
    if (type != TNodeType::readStmt &&
        type != TNodeType::printStmt &&
        type != TNodeType::callStmt &&
        type != TNodeType::whileStmt &&
        type != TNodeType::ifStmt &&
        type != TNodeType::assignStmt) {
        throw runtime_error("invalid statement type");
    }
    return type;
}


Stmt::Stmt(TNodeType type, vector<TNode *> children): TNode(Stmt::verify(type), nullptr, move(children)) {
}
