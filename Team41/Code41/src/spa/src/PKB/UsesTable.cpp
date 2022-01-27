//
// Created by JinHao on 27/1/22.
//

#include "UsesTable.h"

UsesTable::UsesTable() : usesStmtRelation("UsesS"), usesProcRelation("UsesP") {}

void UsesTable::setVarUsedInStmt(string stmtNum, string varName) {
    usesStmtRelation.addMapping(stmtNum, varName);
}

set<string> UsesTable::getVarsUsedInStmt(string stmtNum) {
    return usesStmtRelation.getValuesFromKey(stmtNum);
}

set<string> UsesTable::getStmtsUsingVar(string varName) {
    return usesStmtRelation.getKeyFromValue(varName);
}

set<pair<string, string>> UsesTable::getStmtsVarEntries() {
    return usesStmtRelation.getEntries();
}

bool UsesTable::isUsesS(string stmtNum, string varName) {
    return usesStmtRelation.hasMapping(stmtNum, varName);
}

void UsesTable::setVarUsedInProc(string procName, string varName) {
    usesProcRelation.addMapping(procName, varName);
}

set<string> UsesTable::getVarsUsedInProc(string procName) {
    return usesProcRelation.getValuesFromKey(procName);
}

set<string> UsesTable::getProcsUsingVar(string varName) {
    return usesProcRelation.getKeyFromValue(varName);
}

set<pair<string, string>> UsesTable::getProcVarEntries() {
    return usesProcRelation.getEntries();
}

bool UsesTable::isUsesP(string procName, string varName) {
    return usesProcRelation.hasMapping(procName, varName);
}