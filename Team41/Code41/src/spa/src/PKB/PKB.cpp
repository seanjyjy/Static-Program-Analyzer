#include <utility>
#include <vector>

using namespace std;

#include "PKB.h"
#include "StmtType.h"
#include "StmtTable.h"
#include "EntityTable.h"
#include "UsesTable.h"
#include "TNode.h"

PKB::PKB() {
    stmtTable = new StmtTable();
    entityTable = new EntityTable();
    usesTable = new UsesTable();
}

PKB::~PKB() {
    delete stmtTable;
    delete entityTable;
    delete usesTable;
}

//======================================== Statements ==================================================

set<string> PKB::getAssigns() { return stmtTable->getAllStmtsByType(ASSIGN); }

set<string> PKB::getWhiles() { return stmtTable->getAllStmtsByType(WHILE); }

set<string> PKB::getIfs() { return stmtTable->getAllStmtsByType(IF); }

set<string> PKB::getReads() { return stmtTable->getAllStmtsByType(READ); }

set<string> PKB::getPrints() { return stmtTable->getAllStmtsByType(PRINT); }

set<string> PKB::getCalls() { return stmtTable->getAllStmtsByType(CALL); }

void PKB::registerAssign(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), ASSIGN); }

void PKB::registerWhile(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), WHILE); }

void PKB::registerIf(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), IF); }

void PKB::registerRead(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), READ); }

void PKB::registerPrint(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), PRINT); }

void PKB::registerCall(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), CALL); }

bool PKB::isAssignStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), ASSIGN); }

bool PKB::isWhileStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), WHILE); }

bool PKB::isIfStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), IF); }

bool PKB::isReadStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), READ); }

bool PKB::isPrintStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), PRINT); }

bool PKB::isCallStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), CALL); }

int PKB::getStatementCount() { return stmtTable->getStmtCount(); }


//======================================== Entities ==================================================

set<string> PKB::getVariables() { return entityTable->getVariables(); }

set<string> PKB::getConstants() { return entityTable->getConstants(); }

set<string> PKB::getProcedures() { return entityTable->getProcedures(); }

void PKB::registerVariable(string varName) { return entityTable->addVariable(std::move(varName)); }

void PKB::registerConstant(string constVal) { return entityTable->addConstant(std::move(constVal)); }

void PKB::registerProcedure(string procName) { return entityTable->addProcedure(std::move(procName)); }


//======================================== Uses ==================================================

bool PKB::isUses(string stmtNum, string varName) {
    return usesTable->isUsesS(std::move(stmtNum), std::move(varName));
}

set<string> PKB::getUsesByVar(string varName) { return usesTable->getStmtsUsingVar(std::move(varName)); }

set<string> PKB::getUsesByStmt(string stmtNum) { return usesTable->getVarsUsedInStmt(std::move(stmtNum)); }

set<pair<string, string>> PKB::getAllUses() { return usesTable->getStmtsVarEntries(); }

void PKB::registerUsesS(string stmtNum, string varName) {
    return usesTable->setVarUsedInStmt(std::move(stmtNum), std::move(varName));
}

bool PKB::isUsesP(string procName, string varName) {
    return usesTable->isUsesP(std::move(procName), std::move(varName));
}

set<string> PKB::getUsesPByVar(string varName) { return usesTable->getProcsUsingVar(std::move(varName)); }

set<string> PKB::getUsesByProc(string procName) { return usesTable->getVarsUsedInProc(std::move(procName)); }

set<pair<string, string>> PKB::getAllUsesP() { return usesTable->getProcVarEntries(); }

void PKB::registerUsesP(string procName, string varName) {
    return usesTable->setVarUsedInProc(std::move(procName), std::move(varName));
}
