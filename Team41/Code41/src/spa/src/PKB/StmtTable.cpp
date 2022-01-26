//
// Created by JinHao on 26/1/22.
//
#include <stdexcept>
#include <string>

#include "StmtTable.h"

using namespace std;

StmtTable::StmtTable() = default;

set<string> StmtTable::getAllStmtsByType(StmtType type) {
    return typesToStmts.get(type);
}

StmtType StmtTable::getStmtType(string stmtNumber) {
    return stmtToType.get(stmtNumber);
}

bool StmtTable::isStmtType(string stmtNumber, StmtType type) {
    return stmtToType.hasKeyValue(stmtNumber, type);
}

void StmtTable::setStmt(string stmtNum, StmtType type) {
    if (stmtToType.hasKey(stmtNum) && !stmtToType.hasKeyValue(stmtNum, type)) {
        throw runtime_error("[PKB][StmtTable] Multiple statement types detected in statement: " + stmtNum);
    }
    stmtToType.put(stmtNum, type);
    typesToStmts.add(type, stmtNum);
}

int StmtTable::getStmtCount() {
    return stmtToType.keySize();
}

set<string> StmtTable::getAllStmts() {
    return stmtToType.keys();
}

set<StmtType> StmtTable::getAllTypes() {
    return typesToStmts.keys();
}