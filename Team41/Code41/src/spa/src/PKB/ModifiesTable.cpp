//
// Created by JinHao on 27/1/22.
//

#include "ModifiesTable.h"

ModifiesTable::ModifiesTable() : modifyStmtRelation("ModifiesS"), modifyProcRelation("ModifiesP") {}

void ModifiesTable::setVarModifiedInStmt(string stmtNum, string varName) {
    modifyStmtRelation.addMapping(stmtNum, varName);
}

set<string> ModifiesTable::getVarsModifiedInStmt(string stmtNum) {
    return modifyStmtRelation.getValuesFromKey(stmtNum);
}

set<string> ModifiesTable::getStmtsModifyingVar(string varName) {
    return modifyStmtRelation.getKeyFromValue(varName);
}

set<pair<string, string>> ModifiesTable::getStmtsVarEntries() {
    return modifyStmtRelation.getEntries();
}

bool ModifiesTable::isModifiesS(string stmtNum, string varName) {
    return modifyStmtRelation.hasMapping(stmtNum, varName);
}

void ModifiesTable::setVarModifiedInProc(string procName, string varName) {
    modifyProcRelation.addMapping(procName, varName);
}

set<string> ModifiesTable::getVarsModifiedInProc(string procName) {
    return modifyProcRelation.getValuesFromKey(procName);
}

set<string> ModifiesTable::getProcsModifyingVar(string varName) {
    return modifyProcRelation.getKeyFromValue(varName);
}

set<pair<string, string>> ModifiesTable::getProcVarEntries() {
    return modifyProcRelation.getEntries();
}

bool ModifiesTable::isModifiesP(string procName, string varName) {
    return modifyProcRelation.hasMapping(procName, varName);
}