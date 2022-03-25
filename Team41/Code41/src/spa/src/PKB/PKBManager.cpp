#include "PKB/Tables/StmtTable.h"
#include "PKB/Tables/EntityTable.h"
#include "PKB/Tables/UsesTable.h"
#include "PKB/Tables/ModifiesTable.h"
#include "PKB/Tables/FollowsTable.h"
#include "PKB/Tables/ParentTable.h"
#include "PKB/Tables/CallsTable.h"
#include "PKB/Tables/AssignPatternTable.h"
#include "PKB/Tables/WhilePatternTable.h"
#include "PKB/Tables/IfPatternTable.h"

using namespace std;

#include "PKBManager.h"

//======================================== Statements ==================================================

void PKBManager::registerAssignStmt(const string &stmtNum) { return stmtTable->setStmt(stmtNum, ASSIGN); }

void PKBManager::registerWhileStmt(const string &stmtNum) { return stmtTable->setStmt(stmtNum, WHILE); }

void PKBManager::registerIfStmt(const string &stmtNum) { return stmtTable->setStmt(stmtNum, IF); }

void PKBManager::registerReadStmt(const string &stmtNum, const string &varName) {
    stmtTable->setStmt(stmtNum, READ);
    stmtTable->setStmtAttr(stmtNum, varName);
}

void PKBManager::registerPrintStmt(const string &stmtNum, const string &varName) {
    stmtTable->setStmt(stmtNum, PRINT);
    stmtTable->setStmtAttr(stmtNum, varName);
}

void PKBManager::registerCallStmt(const string &stmtNum, const string &procName) {
    stmtTable->setStmt(stmtNum, CALL);
    stmtTable->setStmtAttr(stmtNum, procName);
}

//======================================== Entities ==================================================

void PKBManager::registerVariable(const string &varName) { return entityTable->addVariable(varName); }

void PKBManager::registerConstant(const string &constVal) { return entityTable->addConstant(constVal); }

void PKBManager::registerProcedure(const string &procName) { return entityTable->addProcedure(procName); }

//======================================== Calls ==================================================

void PKBManager::registerCalls(const string &proc1, const string &proc2) {
    callsTable->setCalls(proc1, proc2);
}

void PKBManager::registerCallsT(const string &proc1, const string &proc2) {
    callsTable->setCallsT(proc1, proc2);
}

//======================================== Follows ==================================================

void PKBManager::registerFollows(const string &stmt1, const string &stmt2) {
    return followsTable->setFollows(stmt1, stmt2);
}

void PKBManager::registerFollowsT(const string &stmt1, const string &stmt2) {
    return followsTable->setFollowsT(stmt1, stmt2);
}

//======================================== Parent ==================================================

void PKBManager::registerParent(const string &parentStmt, const string &childStmt) {
    return parentTable->setParent(parentStmt, childStmt);
}

void PKBManager::registerParentT(const string &parentStmt, const string &childStmt) {
    return parentTable->setParentT(parentStmt, childStmt);
}

//======================================== Uses ==================================================

void PKBManager::registerUsesS(const string &stmtNum, const string &varName) {
    return usesTable->setVarUsedInStmt(stmtNum, varName);
}

void PKBManager::registerUsesP(const string &procName, const string &varName) {
    return usesTable->setVarUsedInProc(procName, varName);
}

//======================================== Modifies ==================================================

void PKBManager::registerModifiesS(const string &stmtNum, const string &varName) {
    return modifiesTable->setVarModifiedInStmt(stmtNum, varName);
}

void PKBManager::registerModifiesP(const string &procName, const string &varName) {
    return modifiesTable->setVarModifiedInProc(procName, varName);
}

//======================================== Pattern ==================================================

void PKBManager::registerAssignPattern(const string &stmtNum, const string &lhsVariable, TNode *rhsAssignAST) {
    return assignPatternTable->setPattern(stmtNum, lhsVariable, rhsAssignAST);
}

void PKBManager::registerIfPattern(const string &stmtNum, const string &condVariable) {
    return ifPatternTable->setPattern(stmtNum, condVariable);
}

void PKBManager::registerWhilePattern(const string &stmtNum, const string &condVariable) {
    return whilePatternTable->setPattern(stmtNum, condVariable);
}

//======================================== CFG ==================================================

void PKBManager::registerCFG(CFGNode *node, const StmtToNodeMap &map) {
    this->rootCFG = node;
    this->stmtToNodeMap = map;
    return nextTable->setCFGNode(node, map);
}