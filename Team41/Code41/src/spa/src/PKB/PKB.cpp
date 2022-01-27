#include <utility>
#include <vector>

using namespace std;

#include "PKB.h"
#include "StmtType.h"
#include "StmtTable.h"
#include "EntityTable.h"
#include "UsesTable.h"
#include "ModifiesTable.h"
#include "TNode.h"

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

set<string> PKB::getAssigns() { return stmtTable->getAllStmtsByType(ASSIGN); }

set<string> PKB::getWhiles() { return stmtTable->getAllStmtsByType(WHILE); }

set<string> PKB::getIfs() { return stmtTable->getAllStmtsByType(IF); }

set<string> PKB::getReads() { return stmtTable->getAllStmtsByType(READ); }

set<string> PKB::getPrints() { return stmtTable->getAllStmtsByType(PRINT); }

set<string> PKB::getCalls() { return stmtTable->getAllStmtsByType(CALL); }

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

set<string> PKB::getVariables() { return entityTable->getVariables(); }

set<string> PKB::getConstants() { return entityTable->getConstants(); }

set<string> PKB::getProcedures() { return entityTable->getProcedures(); }

void PKB::registerVariable(string varName) { return entityTable->addVariable(move(varName)); }

void PKB::registerConstant(string constVal) { return entityTable->addConstant(move(constVal)); }

void PKB::registerProcedure(string procName) { return entityTable->addProcedure(move(procName)); }


//======================================== Uses ==================================================

bool PKB::isUsesS(string stmtNum, string varName) {
    return usesTable->isUsesS(move(stmtNum), move(varName));
}

set<string> PKB::getUsesSByVar(string varName) { return usesTable->getStmtsUsingVar(move(varName)); }

set<string> PKB::getUsesByStmt(string stmtNum) { return usesTable->getVarsUsedInStmt(move(stmtNum)); }

set<pair<string, string>> PKB::getAllUsesS() { return usesTable->getStmtsVarEntries(); }

void PKB::registerUsesS(string stmtNum, string varName) {
    return usesTable->setVarUsedInStmt(move(stmtNum), move(varName));
}

bool PKB::isUsesP(string procName, string varName) {
    return usesTable->isUsesP(move(procName), move(varName));
}

set<string> PKB::getUsesPByVar(string varName) { return usesTable->getProcsUsingVar(move(varName)); }

set<string> PKB::getUsesByProc(string procName) { return usesTable->getVarsUsedInProc(move(procName)); }

set<pair<string, string>> PKB::getAllUsesP() { return usesTable->getProcVarEntries(); }

void PKB::registerUsesP(string procName, string varName) {
    return usesTable->setVarUsedInProc(move(procName), move(varName));
}


//======================================== Modifies ==================================================

bool PKB::isModifiesS(string stmtNum, string varName) {
    return modifiesTable->isModifiesS(move(stmtNum), move(varName));
}

set<string> PKB::getModifiesSByVar(string varName) { return modifiesTable->getStmtsModifyingVar(move(varName)); }

set<string> PKB::getModifiesByStmt(string stmtNum) { return modifiesTable->getVarsModifiedInStmt(move(stmtNum)); }

set<pair<string, string>> PKB::getAllModifiesS() { return modifiesTable->getStmtsVarEntries(); }

void PKB::registerModifiesS(string stmtNum, string varName) {
    return modifiesTable->setVarModifiedInStmt(move(stmtNum), move(varName));
}

bool PKB::isModifiesP(string procName, string varName) {
    return modifiesTable->isModifiesP(move(procName), move(varName));
}

set<string> PKB::getModifiesPByVar(string varName) { return modifiesTable->getProcsModifyingVar(move(varName)); }

set<string> PKB::getModifiesByProc(string procName) { return modifiesTable->getVarsModifiedInProc(move(procName)); }

set<pair<string, string>> PKB::getAllModifiesP() { return modifiesTable->getProcVarEntries(); }

void PKB::registerModifiesP(string procName, string varName) {
    return modifiesTable->setVarModifiedInProc(move(procName), move(varName));
}