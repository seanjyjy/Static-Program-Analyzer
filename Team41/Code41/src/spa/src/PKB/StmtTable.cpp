#include <string>

#include "StmtTable.h"

using namespace std;

StmtTable::StmtTable():mapping("StmtTable") {}

unordered_set<string> StmtTable::getAllStmtsByType(StmtType type) {
    return mapping.getValuesFromKey(type);
}

StmtType StmtTable::getStmtType(string stmtNumber) {
    return mapping.getKeyFromValue(stmtNumber);
}

bool StmtTable::isStmtType(string stmtNumber, StmtType type) {
    return mapping.hasMapping(type, stmtNumber);
}

void StmtTable::setStmt(string stmtNum, StmtType type) {
    return mapping.addMapping(type, stmtNum);
}

int StmtTable::getStmtCount() {
    return mapping.valSize();
}

unordered_set<string> StmtTable::getAllStmts() {
    return mapping.getValues();
}

unordered_set<StmtType> StmtTable::getAllTypes() {
    return mapping.getKeys();
}