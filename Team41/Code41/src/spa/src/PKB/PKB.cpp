#include <utility>
#include <vector>

using namespace std;

#include "PKB.h"

PKB::PKB() {
    stmtTable = new StmtTable();
    entityTable = new EntityTable();
    usesTable = new UsesTable();
    modifiesTable = new ModifiesTable();
}

PKB::~PKB() {
    delete stmtTable;
    delete entityTable;
    delete usesTable;
    delete modifiesTable;
}

//======================================== Statements ==================================================

unordered_set<string> PKB::getAssigns() { return stmtTable->getAllStmtsByType(ASSIGN); }

unordered_set<string> PKB::getWhiles() { return stmtTable->getAllStmtsByType(WHILE); }

unordered_set<string> PKB::getIfs() { return stmtTable->getAllStmtsByType(IF); }

unordered_set<string> PKB::getReads() { return stmtTable->getAllStmtsByType(READ); }

unordered_set<string> PKB::getPrints() { return stmtTable->getAllStmtsByType(PRINT); }

unordered_set<string> PKB::getCalls() { return stmtTable->getAllStmtsByType(CALL); }

void PKB::registerAssign(string stmtNum) { return stmtTable->setStmt(move(stmtNum), ASSIGN); }

void PKB::registerWhile(string stmtNum) { return stmtTable->setStmt(move(stmtNum), WHILE); }

void PKB::registerIf(string stmtNum) { return stmtTable->setStmt(move(stmtNum), IF); }

void PKB::registerRead(string stmtNum) { return stmtTable->setStmt(move(stmtNum), READ); }

void PKB::registerPrint(string stmtNum) { return stmtTable->setStmt(move(stmtNum), PRINT); }

void PKB::registerCall(string stmtNum) { return stmtTable->setStmt(move(stmtNum), CALL); }

bool PKB::isAssignStmt(string stmtNum) { return stmtTable->isStmtType(move(stmtNum), ASSIGN); }

bool PKB::isWhileStmt(string stmtNum) { return stmtTable->isStmtType(move(stmtNum), WHILE); }

bool PKB::isIfStmt(string stmtNum) { return stmtTable->isStmtType(move(stmtNum), IF); }

bool PKB::isReadStmt(string stmtNum) { return stmtTable->isStmtType(move(stmtNum), READ); }

bool PKB::isPrintStmt(string stmtNum) { return stmtTable->isStmtType(move(stmtNum), PRINT); }

bool PKB::isCallStmt(string stmtNum) { return stmtTable->isStmtType(move(stmtNum), CALL); }

int PKB::getStatementCount() { return stmtTable->getStmtCount(); }


//======================================== Entities ==================================================

unordered_set<string> PKB::getVariables() { return entityTable->getVariables(); }

unordered_set<string> PKB::getConstants() { return entityTable->getConstants(); }

unordered_set<string> PKB::getProcedures() { return entityTable->getProcedures(); }

void PKB::registerVariable(string varName) { return entityTable->addVariable(move(varName)); }

void PKB::registerConstant(string constVal) { return entityTable->addConstant(move(constVal)); }

void PKB::registerProcedure(string procName) { return entityTable->addProcedure(move(procName)); }


//======================================== Uses ==================================================

bool PKB::isUsesS(string stmtNum, string varName) {
    return usesTable->isUsesS(move(stmtNum), move(varName));
}

unordered_set<string> PKB::getUsesSByVar(string varName) { return usesTable->getStmtsUsingVar(move(varName)); }

unordered_set<string> PKB::getUsesByStmt(string stmtNum) { return usesTable->getVarsUsedInStmt(move(stmtNum)); }

vector<pair<string, string>> PKB::getAllUsesS() { return usesTable->getStmtsVarEntries(); }

unordered_set<string> PKB::getAllStmtsUsingSomeVar() { return usesTable->getStmtsUsingSomeVar(); }

unordered_set<string> PKB::getAllVarsUsedInSomeStmt() { return usesTable->getVarsUsedInSomeStmt(); }

void PKB::registerUsesS(string stmtNum, string varName) {
    return usesTable->setVarUsedInStmt(move(stmtNum), move(varName));
}

bool PKB::isUsesP(string procName, string varName) {
    return usesTable->isUsesP(move(procName), move(varName));
}

unordered_set<string> PKB::getUsesPByVar(string varName) { return usesTable->getProcsUsingVar(move(varName)); }

unordered_set<string> PKB::getUsesByProc(string procName) { return usesTable->getVarsUsedInProc(move(procName)); }

vector<pair<string, string>> PKB::getAllUsesP() { return usesTable->getProcVarEntries(); }

unordered_set<string> PKB::getAllProcsUsingSomeVar() { return usesTable->getProcsUsingSomeVar(); }

unordered_set<string> PKB::getAllVarsUsedInSomeProc() { return usesTable->getVarsUsedInSomeProc(); }

void PKB::registerUsesP(string procName, string varName) {
    return usesTable->setVarUsedInProc(move(procName), move(varName));
}


//======================================== Modifies ==================================================

bool PKB::isModifiesS(string stmtNum, string varName) {
    return modifiesTable->isModifiesS(move(stmtNum), move(varName));
}

unordered_set<string> PKB::getModifiesSByVar(string varName) { return modifiesTable->getStmtsModifyingVar(move(varName)); }

unordered_set<string> PKB::getModifiesByStmt(string stmtNum) { return modifiesTable->getVarsModifiedInStmt(move(stmtNum)); }

vector<pair<string, string>> PKB::getAllModifiesS() { return modifiesTable->getStmtsVarEntries(); }

void PKB::registerModifiesS(string stmtNum, string varName) {
    return modifiesTable->setVarModifiedInStmt(move(stmtNum), move(varName));
}

bool PKB::isModifiesP(string procName, string varName) {
    return modifiesTable->isModifiesP(move(procName), move(varName));
}

unordered_set<string> PKB::getModifiesPByVar(string varName) { return modifiesTable->getProcsModifyingVar(move(varName)); }

unordered_set<string> PKB::getModifiesByProc(string procName) { return modifiesTable->getVarsModifiedInProc(move(procName)); }

vector<pair<string, string>> PKB::getAllModifiesP() { return modifiesTable->getProcVarEntries(); }

void PKB::registerModifiesP(string procName, string varName) {
    return modifiesTable->setVarModifiedInProc(move(procName), move(varName));
}