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

bool PKBBaseClient::isAssignStmt(const string &stmtNum) const { return stmtTable->isStmtType(stmtNum, ASSIGN); }

bool PKBBaseClient::isWhileStmt(const string &stmtNum) const { return stmtTable->isStmtType(stmtNum, WHILE); }

bool PKBBaseClient::isIfStmt(const string &stmtNum) const { return stmtTable->isStmtType(stmtNum, IF); }

bool PKBBaseClient::isReadStmt(const string &stmtNum) const { return stmtTable->isStmtType(stmtNum, READ); }

bool PKBBaseClient::isPrintStmt(const string &stmtNum) const { return stmtTable->isStmtType(stmtNum, PRINT); }

bool PKBBaseClient::isCallStmt(const string &stmtNum) const { return stmtTable->isStmtType(stmtNum, CALL); }

size_t PKBBaseClient::getStatementCount() const { return stmtTable->getStmtCount(); }

size_t PKBBaseClient::getAssignCount() const { return stmtTable->getStmtCount(ASSIGN); }

size_t PKBBaseClient::getWhileCount() const { return stmtTable->getStmtCount(WHILE); }

size_t PKBBaseClient::getIfCount() const { return stmtTable->getStmtCount(IF); }

size_t PKBBaseClient::getReadCount() const { return stmtTable->getStmtCount(READ); }

size_t PKBBaseClient::getPrintCount() const { return stmtTable->getStmtCount(PRINT); }

size_t PKBBaseClient::getCallCount() const { return stmtTable->getStmtCount(CALL); }

string PKBBaseClient::getCallsProcNameAttr(const string &stmtNum) const {
    return isCallStmt(stmtNum) ? stmtTable->getStmtAttr(stmtNum) : "";
}

string PKBBaseClient::getPrintVarNameAttr(const string &stmtNum) const {
    return isPrintStmt(stmtNum) ? stmtTable->getStmtAttr(stmtNum) : "";
}

string PKBBaseClient::getReadVarNameAttr(const string &stmtNum) const {
    return isReadStmt(stmtNum) ? stmtTable->getStmtAttr(stmtNum) : "";
}

bool PKBBaseClient::hasStmt(const string &stmtNum) const {
    return stmtTable->getStmtType(stmtNum) != StmtType::UNKNOWN_STMT;
}

//======================================== Entities ==================================================

unordered_set<string> PKBBaseClient::getVariables() const { return entityTable->getVariables(); }

unordered_set<string> PKBBaseClient::getConstants() const { return entityTable->getConstants(); }

unordered_set<string> PKBBaseClient::getProcedures() const { return entityTable->getProcedures(); }

unordered_set<string> PKBBaseClient::getStatements() const { return stmtTable->getAllStmts(); }

bool PKBBaseClient::isConstant(const string &constVal) const { return entityTable->isConstant(constVal); }

bool PKBBaseClient::isProcedure(const string &procName) const { return entityTable->isProcedure(procName); }

bool PKBBaseClient::isVariable(const string &varName) const { return entityTable->isVariable(varName); }

size_t PKBBaseClient::getConstantCount() const { return entityTable->getEntityCount(CONST); }

size_t PKBBaseClient::getProcedureCount() const { return entityTable->getEntityCount(PROC); }

size_t PKBBaseClient::getVariableCount() const { return entityTable->getEntityCount(VAR); }
