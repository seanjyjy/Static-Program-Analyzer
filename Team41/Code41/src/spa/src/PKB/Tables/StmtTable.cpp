#include <string>
#include <utility>

#include "StmtTable.h"

using namespace std;

StmtTable::StmtTable():mapping("StmtTable") {}

unordered_set<string> StmtTable::getAllStmtsByType(StmtType type) {
    return mapping.getValuesFromKey(move(type));
}

StmtType StmtTable::getStmtType(string stmtNumber) {
    return mapping.getKeyFromValue(move(stmtNumber));
}

bool StmtTable::isStmtType(string stmtNumber, StmtType type) {
    return mapping.hasMapping(type, move(stmtNumber));
}

void StmtTable::setStmt(const string& stmtNum, StmtType type) {
    return mapping.addMapping(type, stmtNum);
}

size_t StmtTable::getStmtCount() {
    return mapping.valSize();
}

unordered_set<string> StmtTable::getAllStmts() {
    return mapping.getValues();
}

unordered_set<StmtType> StmtTable::getAllTypes() {
    return mapping.getKeys();
}