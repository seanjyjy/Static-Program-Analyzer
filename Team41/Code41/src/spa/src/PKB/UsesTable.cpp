#include "UsesTable.h"

UsesTable::UsesTable() : usesStmtRelation("UsesS"), usesProcRelation("UsesP") {}

void UsesTable::setVarUsedInStmt(string stmtNum, string varName) {
    usesStmtRelation.addMapping(stmtNum, varName);
}

unordered_set<string> UsesTable::getVarsUsedInStmt(string stmtNum) {
    return usesStmtRelation.getValuesFromKey(stmtNum);
}

unordered_set<string> UsesTable::getStmtsUsingVar(string varName) {
    return usesStmtRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> UsesTable::getStmtsVarEntries() {
    return usesStmtRelation.getEntries();
}

bool UsesTable::isUsesS(string stmtNum, string varName) {
    return usesStmtRelation.hasMapping(stmtNum, varName);
}

void UsesTable::setVarUsedInProc(string procName, string varName) {
    usesProcRelation.addMapping(procName, varName);
}

unordered_set<string> UsesTable::getVarsUsedInProc(string procName) {
    return usesProcRelation.getValuesFromKey(procName);
}

unordered_set<string> UsesTable::getProcsUsingVar(string varName) {
    return usesProcRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> UsesTable::getProcVarEntries() {
    return usesProcRelation.getEntries();
}

bool UsesTable::isUsesP(string procName, string varName) {
    return usesProcRelation.hasMapping(procName, varName);
}