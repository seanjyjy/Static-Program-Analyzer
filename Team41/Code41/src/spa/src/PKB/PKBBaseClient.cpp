#include "PKB/Tables/StmtTable.h"
#include "PKB/Tables/EntityTable.h"
#include "PKBBaseClient.h"

using namespace std;

PKBBaseClient::PKBBaseClient() {
    stmtTable = new StmtTable();
    entityTable = new EntityTable();
}

PKBBaseClient::~PKBBaseClient() {
    delete stmtTable;
    delete entityTable;
}

//======================================== Statements ==================================================

unordered_set<string> PKBBaseClient::getAssigns() const { return stmtTable->getAllStmtsByType(ASSIGN); }

unordered_set<string> PKBBaseClient::getWhiles() const { return stmtTable->getAllStmtsByType(WHILE); }

unordered_set<string> PKBBaseClient::getIfs() const { return stmtTable->getAllStmtsByType(IF); }

unordered_set<string> PKBBaseClient::getReads() const { return stmtTable->getAllStmtsByType(READ); }

unordered_set<string> PKBBaseClient::getPrints() const { return stmtTable->getAllStmtsByType(PRINT); }

unordered_set<string> PKBBaseClient::getCalls() const { return stmtTable->getAllStmtsByType(CALL); }

bool PKBBaseClient::isAssignStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), ASSIGN); }

bool PKBBaseClient::isWhileStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), WHILE); }

bool PKBBaseClient::isIfStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), IF); }

bool PKBBaseClient::isReadStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), READ); }

bool PKBBaseClient::isPrintStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), PRINT); }

bool PKBBaseClient::isCallStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), CALL); }

int PKBBaseClient::getStatementCount() const { return stmtTable->getStmtCount(); }

//======================================== Entities ==================================================

unordered_set<string> PKBBaseClient::getVariables() const { return entityTable->getVariables(); }

unordered_set<string> PKBBaseClient::getConstants() const { return entityTable->getConstants(); }

unordered_set<string> PKBBaseClient::getProcedures() const { return entityTable->getProcedures(); }

unordered_set<string> PKBBaseClient::getStatements() const { return stmtTable->getAllStmts(); }

bool PKBBaseClient::isConstant(string constVal) const { return entityTable->isConstant(move(constVal)); }

bool PKBBaseClient::isProcedure(string procName) const { return entityTable->isProcedure(move(procName)); }

bool PKBBaseClient::isVariable(string varName) const { return entityTable->isVariable(move(varName)); }
