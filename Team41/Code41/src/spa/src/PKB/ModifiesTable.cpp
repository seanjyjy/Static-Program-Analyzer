#include "ModifiesTable.h"

ModifiesTable::ModifiesTable() : modifyStmtRelation("ModifiesS"), modifyProcRelation("ModifiesP") {}

void ModifiesTable::setVarModifiedInStmt(string stmtNum, string varName) {
    modifyStmtRelation.addMapping(stmtNum, varName);
}

unordered_set<string> ModifiesTable::getVarsModifiedInStmt(string stmtNum) {
    return modifyStmtRelation.getValuesFromKey(stmtNum);
}

unordered_set<string> ModifiesTable::getStmtsModifyingVar(string varName) {
    return modifyStmtRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> ModifiesTable::getStmtsVarEntries() {
    return modifyStmtRelation.getEntries();
}

bool ModifiesTable::isModifiesS(string stmtNum, string varName) {
    return modifyStmtRelation.hasMapping(stmtNum, varName);
}

void ModifiesTable::setVarModifiedInProc(string procName, string varName) {
    modifyProcRelation.addMapping(procName, varName);
}

unordered_set<string> ModifiesTable::getVarsModifiedInProc(string procName) {
    return modifyProcRelation.getValuesFromKey(procName);
}

unordered_set<string> ModifiesTable::getProcsModifyingVar(string varName) {
    return modifyProcRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> ModifiesTable::getProcVarEntries() {
    return modifyProcRelation.getEntries();
}

bool ModifiesTable::isModifiesP(string procName, string varName) {
    return modifyProcRelation.hasMapping(procName, varName);
}