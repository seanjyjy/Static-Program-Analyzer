#include "StmtLst.h"

#include <stdexcept>

vector<TNode *> convert(const vector<Stmt *> &stmts) {
    vector<TNode *> ret;
    ret.reserve(stmts.size());
    for (Stmt *stmt: stmts) ret.push_back(static_cast<TNode *>(stmt));
    return ret;
}

StmtLst::StmtLst(const vector<Stmt *> &stmts) : TNode(TNodeType::stmtLst, nullptr, convert(stmts)) {
}

string StmtLst::toString() {
    return "stmtLst";
}

bool StmtLst::isStmtLst() const {
    return true;
}
