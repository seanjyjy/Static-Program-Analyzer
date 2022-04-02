#include "ModifiesTable.h"

#include <utility>

ModifiesTable::ModifiesTable() : modifyStmtRelation("ModifiesS"), modifyProcRelation("ModifiesP") {}

void ModifiesTable::setVarModifiedInStmt(const string &stmtNum, const string &varName) {
    modifyStmtRelation.addMapping(stmtNum, varName);
}

unordered_set<string> ModifiesTable::getVarsModifiedInStmt(const string &stmtNum) {
    return modifyStmtRelation.getValuesFromKey(stmtNum);
}

unordered_set<string> ModifiesTable::getStmtsModifyingVar(const string &varName) {
    return modifyStmtRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> ModifiesTable::getStmtsVarEntries() {
    return modifyStmtRelation.getEntries();
}

unordered_set<string> ModifiesTable::getVarsModifiedInSomeStmt() {
    return modifyStmtRelation.getValues();
}

unordered_set<string> ModifiesTable::getStmtsModifyingSomeVar() {
    return modifyStmtRelation.getKeys();
}

bool ModifiesTable::isModifiesS(string stmtNum, string varName) {
    return modifyStmtRelation.hasMapping(stmtNum, varName);
}

void ModifiesTable::setVarModifiedInProc(const string &procName, const string &varName) {
    modifyProcRelation.addMapping(procName, varName);
}

unordered_set<string> ModifiesTable::getVarsModifiedInProc(const string &procName) {
    return modifyProcRelation.getValuesFromKey(procName);
}

unordered_set<string> ModifiesTable::getProcsModifyingVar(const string &varName) {
    return modifyProcRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> ModifiesTable::getProcVarEntries() {
    return modifyProcRelation.getEntries();
}

unordered_set<string> ModifiesTable::getVarsModifiedInSomeProc() {
    return modifyProcRelation.getValues();
}

unordered_set<string> ModifiesTable::getProcsModifyingSomeVar() {
    return modifyProcRelation.getKeys();
}

bool ModifiesTable::isModifiesP(const string &procName, const string &varName) {
    return modifyProcRelation.hasMapping(procName, varName);
}