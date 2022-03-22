#include <string>
#include <utility>

#include "StmtTable.h"

using namespace std;

StmtTable::StmtTable() : stmtMapping("StmtTable") {}

unordered_set<string> StmtTable::getAllStmtsByType(StmtType type) {
    return stmtMapping.getValuesFromKey(move(type));
}

StmtType StmtTable::getStmtType(string stmtNumber) {
    return stmtMapping.getKeyFromValue(move(stmtNumber));
}

string StmtTable::getStmtAttr(string stmtNumber) {
    return attrMapping.find(stmtNumber) == attrMapping.end() ? "" : attrMapping[stmtNumber];
}

bool StmtTable::isStmtType(string stmtNumber, StmtType type) {
    return stmtMapping.hasMapping(type, move(stmtNumber));
}

void StmtTable::setStmt(const string &stmtNum, StmtType type) {
    return stmtMapping.addMapping(type, stmtNum);
}

void StmtTable::setStmtAttr(const string &stmtNum, const string &attr) {
    attrMapping.insert({stmtNum, attr});
}

size_t StmtTable::getStmtCount(StmtType type) {
    return stmtMapping.size(move(type));
}

size_t StmtTable::getStmtCount() {
    return stmtMapping.valSize();
}

unordered_set<string> StmtTable::getAllStmts() {
    return stmtMapping.getValues();
}

unordered_set<StmtType> StmtTable::getAllTypes() {
    return stmtMapping.getKeys();
}