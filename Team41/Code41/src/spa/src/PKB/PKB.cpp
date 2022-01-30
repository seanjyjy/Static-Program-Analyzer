#include <utility>
#include <vector>
#include <iostream>
#include "StmtTable.h"
#include "EntityTable.h"
#include "UsesTable.h"
#include "ModifiesTable.h"
#include "FollowsTable.h"
#include "ParentTable.h"

using namespace std;

#include "PKB.h"

PKB::PKB() {
    stmtTable = new StmtTable();
    entityTable = new EntityTable();
    usesTable = new UsesTable();
    modifiesTable = new ModifiesTable();
    followsTable = new FollowsTable();
    parentTable = new ParentTable();
}

PKB::~PKB() {
    delete stmtTable;
    delete entityTable;
    delete usesTable;
    delete modifiesTable;
    delete followsTable;
    delete parentTable;
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

bool PKB::isConstant(string constVal) { return entityTable->isConstant(move(constVal)); };

bool PKB::isProcedure(string procName) { return entityTable->isProcedure(move(procName)); }

bool PKB::isVariable(string varName) { return entityTable->isVariable(move(varName)); }

//======================================== Follows ==================================================

void PKB::registerFollows(string stmt1, string stmt2) { return followsTable->setFollows(move(stmt1), move(stmt2)); }

bool PKB::isFollows(string stmt1, string stmt2) { return followsTable->isFollows(move(stmt1), move(stmt2)); }

string PKB::getStmtFollowing(string stmtNum) { return followsTable->getStmtFollowing(move(stmtNum)); }

string PKB::getStmtFollowedBy(string stmtNum) { return followsTable->getStmtFollowedBy(move(stmtNum)); }

vector<pair<string, string>> PKB::getAllFollows() { return followsTable->getFollowEntries(); }

bool PKB::isFollowsT(string stmt1, string stmt2) { return followsTable->isFollowsT(move(stmt1), move(stmt2)); }

unordered_set<string> PKB::getAllStmtsFollowingT(string stmtNum) {
    return followsTable->getStmtsFollowingT(move(stmtNum));
}

unordered_set<string> PKB::getAllStmtsFollowedTBy(string stmtNum) {
    return followsTable->getStmtsFollowedTBy(move(stmtNum));
}

vector<pair<string, string>> PKB::getAllFollowsT() { return followsTable->getFollowTEntries(); }

//======================================== Parent ==================================================

void PKB::registerParent(string parentStmt, string childStmt) {
    return parentTable->setParent(move(parentStmt), move(childStmt));
}

bool PKB::isParent(string stmt1, string stmt2) { return parentTable->isParent(move(stmt1), move(stmt2)); }

unordered_set<string> PKB::getChildStmtsOf(string parentStmt) {
    return parentTable->getAllChildrenOf(move(parentStmt));
}

string PKB::getParentOf(string childStmt) { return parentTable->getParentOf(move(childStmt)); }

vector<pair<string, string>> PKB::getAllParent() { return parentTable->getParentEntries(); }

bool PKB::isParentT(string stmt1, string stmt2) { return parentTable->isParentT(move(stmt1), stmt2); }

unordered_set<string> PKB::getDescendantStmtsOf(string parentStmt) {
    return parentTable->getAllDescendantsOf(move(parentStmt));
}

unordered_set<string> PKB::getAncestorStmtsOf(string childStmt) {
    return parentTable->getAllAncestorsOf(move(childStmt));
}

vector<pair<string, string>> PKB::getAllParentT() { return parentTable->getParentTEntries(); }

//======================================== Uses ==================================================

bool PKB::isUsesS(string stmtNum, string varName) {
    return usesTable->isUsesS(move(stmtNum), move(varName));
}

unordered_set<string> PKB::getUsesSByVar(string varName) { return usesTable->getStmtsUsingVar(move(varName)); }

unordered_set<string> PKB::getUsesByStmt(string stmtNum) { return usesTable->getVarsUsedInStmt(move(stmtNum)); }

vector<pair<string, string>> PKB::getAllUsesS() { return usesTable->getStmtsVarEntries(); }

void PKB::registerUsesS(string stmtNum, string varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerUsesS] Used variable is not registered" << endl;
    }
    if (!isAssignStmt(stmtNum) && !isPrintStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerUsesS] Statement is not of type assign/print/if/while" << endl;
    }
    return usesTable->setVarUsedInStmt(move(stmtNum), move(varName));
}

bool PKB::isUsesP(string procName, string varName) {
    return usesTable->isUsesP(move(procName), move(varName));
}

unordered_set<string> PKB::getUsesPByVar(string varName) { return usesTable->getProcsUsingVar(move(varName)); }

unordered_set<string> PKB::getUsesByProc(string procName) { return usesTable->getVarsUsedInProc(move(procName)); }

vector<pair<string, string>> PKB::getAllUsesP() { return usesTable->getProcVarEntries(); }

void PKB::registerUsesP(string procName, string varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerUsesP] Used variable is not registered" << endl;
    }
    if (!(isProcedure(procName))) {
        cout << "Warning: " << "[PKB][registerUsesP] Procedure is not registered" << endl;
    }
    return usesTable->setVarUsedInProc(move(procName), move(varName));
}

//======================================== Modifies ==================================================

bool PKB::isModifiesS(string stmtNum, string varName) {
    return modifiesTable->isModifiesS(move(stmtNum), move(varName));
}

unordered_set<string> PKB::getModifiesSByVar(string varName) {
    return modifiesTable->getStmtsModifyingVar(move(varName));
}

unordered_set<string> PKB::getModifiesByStmt(string stmtNum) {
    return modifiesTable->getVarsModifiedInStmt(move(stmtNum));
}

vector<pair<string, string>> PKB::getAllModifiesS() { return modifiesTable->getStmtsVarEntries(); }

void PKB::registerModifiesS(string stmtNum, string varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerModifiesS] Used variable is not registered" << endl;
    }
    if (!isAssignStmt(stmtNum) && !isReadStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerModifiesS] Statement is not of type assign/read/if/while" << endl;
    }
    return modifiesTable->setVarModifiedInStmt(move(stmtNum), move(varName));
}

bool PKB::isModifiesP(string procName, string varName) {
    return modifiesTable->isModifiesP(move(procName), move(varName));
}

unordered_set<string> PKB::getModifiesPByVar(string varName) {
    return modifiesTable->getProcsModifyingVar(move(varName));
}

unordered_set<string> PKB::getModifiesByProc(string procName) {
    return modifiesTable->getVarsModifiedInProc(move(procName));
}

vector<pair<string, string>> PKB::getAllModifiesP() { return modifiesTable->getProcVarEntries(); }

void PKB::registerModifiesP(string procName, string varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerModifiesP] Used variable is not registered" << endl;
    }
    if (!(isProcedure(procName))) {
        cout << "Warning: " << "[PKB][registerModifiesP] Procedure is not registered" << endl;
    }
    return modifiesTable->setVarModifiedInProc(move(procName), move(varName));
}
