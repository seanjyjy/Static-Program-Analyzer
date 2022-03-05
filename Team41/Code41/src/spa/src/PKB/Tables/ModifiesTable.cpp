#include "ModifiesTable.h"

#include <utility>

ModifiesTable::ModifiesTable() : modifyStmtRelation("ModifiesS"), modifyProcRelation("ModifiesP") {}

void ModifiesTable::setVarModifiedInStmt(const string& stmtNum, const string& varName) {
    modifyStmtRelation.addMapping(stmtNum, varName);
}

unordered_set<string> ModifiesTable::getVarsModifiedInStmt(string stmtNum) {
    return modifyStmtRelation.getValuesFromKey(std::move(stmtNum));
}

unordered_set<string> ModifiesTable::getStmtsModifyingVar(string varName) {
    return modifyStmtRelation.getKeysFromValue(std::move(varName));
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
    return modifyStmtRelation.hasMapping(std::move(stmtNum), std::move(varName));
}

void ModifiesTable::setVarModifiedInProc(const string& procName, const string& varName) {
    modifyProcRelation.addMapping(procName, varName);
}

unordered_set<string> ModifiesTable::getVarsModifiedInProc(string procName) {
    return modifyProcRelation.getValuesFromKey(std::move(procName));
}

unordered_set<string> ModifiesTable::getProcsModifyingVar(string varName) {
    return modifyProcRelation.getKeysFromValue(std::move(varName));
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

bool ModifiesTable::isModifiesP(string procName, string varName) {
    return modifyProcRelation.hasMapping(std::move(procName), std::move(varName));
}