#include <utility>
#include <vector>

using namespace std;

#include "PKB.h"
#include "StmtType.h"
#include "StmtTable.h"
#include "EntityTable.h"
#include "TNode.h"

PKB::PKB() {
    stmtTable = new StmtTable();
    entityTable = new EntityTable();
}
PKB::~PKB() {
    delete stmtTable;
    delete entityTable;
}

// metadata register/get/query methods
set<string> PKB::getAssigns() { return stmtTable->getAllStmtsByType(ASSIGN); }

set<string> PKB::getWhiles() { return stmtTable->getAllStmtsByType(WHILE); }

set<string> PKB::getIfs() { return stmtTable->getAllStmtsByType(IF); }

set<string> PKB::getReads() { return stmtTable->getAllStmtsByType(READ); }

set<string> PKB::getPrints() { return stmtTable->getAllStmtsByType(PRINT); }

set<string> PKB::getCalls() { return stmtTable->getAllStmtsByType(CALL); }

set<string> PKB::getVariables() { return entityTable->getVariables(); }

set<string> PKB::getConstants() { return entityTable->getConstants(); }

set<string> PKB::getProcedures() { return entityTable->getProcedures(); }

void PKB::registerAssign(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), ASSIGN); }

void PKB::registerWhile(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), WHILE); }

void PKB::registerIf(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), IF); }

void PKB::registerRead(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), READ); }

void PKB::registerPrint(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), PRINT); }

void PKB::registerCall(string stmtNum) { return stmtTable->setStmt(std::move(stmtNum), CALL); }

void PKB::registerVariable(string varName) { return entityTable->addVariable(std::move(varName)); }

void PKB::registerConstant(string constVal) { return entityTable->addConstant(std::move(constVal)); }

void PKB::registerProcedure(string procName) { return entityTable->addProcedure(std::move(procName)); }

bool PKB::isAssignStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), ASSIGN); }

bool PKB::isWhileStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), WHILE); }

bool PKB::isIfStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), IF); }

bool PKB::isReadStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), READ); }

bool PKB::isPrintStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), PRINT); }

bool PKB::isCallStmt(string stmtNum) { return stmtTable->isStmtType(std::move(stmtNum), CALL); }

int PKB::getStatementCount() { return stmtTable->getStmtCount(); }
