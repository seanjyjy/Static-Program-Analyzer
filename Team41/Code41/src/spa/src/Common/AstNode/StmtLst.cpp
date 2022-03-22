#include "StmtLst.h"

#include <stdexcept>

vector<TNode *> convert(const vector<Stmt *> &stmts) {
    if (stmts.empty()) throw runtime_error("statements must not be empty");

    vector<TNode *> ret;
    ret.reserve(stmts.size());
    for (Stmt *stmt: stmts) ret.push_back(static_cast<TNode *>(stmt));
    return ret;
}

StmtLst::StmtLst(const vector<Stmt *> &stmts) : TNode(TNodeType::stmtLst, nullptr, convert(stmts)) {
    if (stmts.empty()) throw runtime_error("statement list must not be empty");
}
