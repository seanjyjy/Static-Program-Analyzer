#include <utility>
#include <vector>
#include <iostream>
#include "StmtTable.h"
#include "EntityTable.h"
#include "UsesTable.h"
#include "ModifiesTable.h"
#include "FollowsTable.h"
#include "ParentTable.h"
#include "PatternTable.h"
#include "CallsTable.h"

using namespace std;

#include "PKB.h"

PKB::PKB() {
    stmtTable = new StmtTable();
    entityTable = new EntityTable();
    usesTable = new UsesTable();
    modifiesTable = new ModifiesTable();
    followsTable = new FollowsTable();
    parentTable = new ParentTable();
    patternTable = new PatternTable();
    callsTable = new CallsTable();
}

PKB::~PKB() {
    delete stmtTable;
    delete entityTable;
    delete usesTable;
    delete modifiesTable;
    delete followsTable;
    delete parentTable;
    delete patternTable;
    delete callsTable;
}

//======================================== Statements ==================================================

unordered_set<string> PKB::getAssigns() const { return stmtTable->getAllStmtsByType(ASSIGN); }

unordered_set<string> PKB::getWhiles() const { return stmtTable->getAllStmtsByType(WHILE); }

unordered_set<string> PKB::getIfs() const { return stmtTable->getAllStmtsByType(IF); }

unordered_set<string> PKB::getReads() const { return stmtTable->getAllStmtsByType(READ); }

unordered_set<string> PKB::getPrints() const { return stmtTable->getAllStmtsByType(PRINT); }

unordered_set<string> PKB::getCalls() const { return stmtTable->getAllStmtsByType(CALL); }

void PKB::registerAssign(const string& stmtNum) { return stmtTable->setStmt(stmtNum, ASSIGN); }

void PKB::registerWhile(const string& stmtNum) { return stmtTable->setStmt(stmtNum, WHILE); }

void PKB::registerIf(const string& stmtNum) { return stmtTable->setStmt(stmtNum, IF); }

void PKB::registerRead(const string& stmtNum) { return stmtTable->setStmt(stmtNum, READ); }

void PKB::registerPrint(const string& stmtNum) { return stmtTable->setStmt(stmtNum, PRINT); }

void PKB::registerCall(const string& stmtNum) { return stmtTable->setStmt(stmtNum, CALL); }

bool PKB::isAssignStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), ASSIGN); }

bool PKB::isWhileStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), WHILE); }

bool PKB::isIfStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), IF); }

bool PKB::isReadStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), READ); }

bool PKB::isPrintStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), PRINT); }

bool PKB::isCallStmt(string stmtNum) const { return stmtTable->isStmtType(move(stmtNum), CALL); }

int PKB::getStatementCount() const { return stmtTable->getStmtCount(); }

//======================================== Entities ==================================================

unordered_set<string> PKB::getVariables() const { return entityTable->getVariables(); }

unordered_set<string> PKB::getConstants() const { return entityTable->getConstants(); }

unordered_set<string> PKB::getProcedures() const { return entityTable->getProcedures(); }

unordered_set<string> PKB::getStatements() const { return stmtTable->getAllStmts(); }

void PKB::registerVariable(const string& varName) { return entityTable->addVariable(varName); }

void PKB::registerConstant(const string& constVal) { return entityTable->addConstant(constVal); }

void PKB::registerProcedure(const string& procName) { return entityTable->addProcedure(procName); }

bool PKB::isConstant(string constVal) const { return entityTable->isConstant(move(constVal)); }

bool PKB::isProcedure(string procName) const { return entityTable->isProcedure(move(procName)); }

bool PKB::isVariable(string varName) const { return entityTable->isVariable(move(varName)); }

//======================================== Calls ==================================================

void PKB::registerCalls(const string& proc1, const string& proc2) {
    if (!(isProcedure(proc1))) {
        cout << "Warning: " << "[PKB][registerCalls] Caller is not a registered procedure" << endl;
    }
    if (!(isProcedure(proc2))) {
        cout << "Warning: " << "[PKB][registerCalls] Callee is not a registered procedure" << endl;
    }
    callsTable->setCalls(proc1, proc2);
}

void PKB::registerCallsT(const string& proc1, const string& proc2) {
    if (!(isProcedure(proc1))) {
        cout << "Warning: " << "[PKB][registerCallsT] Caller is not a registered procedure" << endl;
    }
    if (!(isProcedure(proc2))) {
        cout << "Warning: " << "[PKB][registerCallsT] Callee is not a registered procedure" << endl;
    }
    callsTable->setCallsT(proc1, proc2);
}

bool PKB::isCalls(string proc1, string proc2) { return callsTable->isCalls(move(proc1), move(proc2)); }

unordered_set<string> PKB::getAllProcCalling(string procName) { return callsTable->getProcsCalling(move(procName)); }

unordered_set<string> PKB::getAllProcCalledBy(string procName) { return callsTable->getProcsCalledBy(move(procName)); }

vector<pair<string, string>> PKB::getAllCalls() { return callsTable->getCallsEntries(); }

bool PKB::isCallsT(string proc1, string proc2) { return callsTable->isCallsT(move(proc1), move(proc2)); }

unordered_set<string> PKB::getAllProcCallingT(string procName) { return callsTable->getProcsCallingT(move(procName)); }

unordered_set<string> PKB::getAllProcCalledTBy(string procName) {
    return callsTable->getProcsCalledTBy(move(procName));
}

vector<pair<string, string>> PKB::getAllCallsT() { return callsTable->getCallsTEntries(); }

//======================================== Follows ==================================================

void PKB::registerFollows(const string& stmt1, const string& stmt2) { return followsTable->setFollows(stmt1, stmt2); }

void PKB::registerFollowsT(const string& stmt1, const string& stmt2) { return followsTable->setFollowsT(stmt1, stmt2); }

bool PKB::isFollows(string stmt1, string stmt2) const { return followsTable->isFollows(move(stmt1), move(stmt2)); }

string PKB::getStmtFollowing(string stmtNum) const { return followsTable->getStmtFollowing(move(stmtNum)); }

string PKB::getStmtFollowedBy(string stmtNum) const { return followsTable->getStmtFollowedBy(move(stmtNum)); }

vector<pair<string, string>> PKB::getAllFollows() const { return followsTable->getFollowEntries(); }

bool PKB::isFollowsT(string stmt1, string stmt2) const { return followsTable->isFollowsT(move(stmt1), move(stmt2)); }

unordered_set<string> PKB::getAllStmtsFollowingT(string stmtNum) const {
    return followsTable->getStmtsFollowingT(move(stmtNum));
}

unordered_set<string> PKB::getAllStmtsFollowedTBy(string stmtNum) const {
    return followsTable->getStmtsFollowedTBy(move(stmtNum));
}

vector<pair<string, string>> PKB::getAllFollowsT() const { return followsTable->getFollowTEntries(); }

unordered_set<string> PKB::getAllStmtsFollowingSomeStmt() const { return followsTable->getStmtsFollowingSomeStmt(); }

unordered_set<string> PKB::getAllStmtsFollowedBySomeStmt() const { return followsTable->getStmtsFollowedBySomeStmt(); }

//======================================== Parent ==================================================

void PKB::registerParent(const string& parentStmt, const string& childStmt) {
    return parentTable->setParent(parentStmt, childStmt);
}

void PKB::registerParentT(const string& parentStmt, const string& childStmt) {
    return parentTable->setParentT(parentStmt, childStmt);
}

bool PKB::isParent(string stmt1, string stmt2) const { return parentTable->isParent(move(stmt1), move(stmt2)); }

unordered_set<string> PKB::getChildStmtsOf(string parentStmt) const {
    return parentTable->getAllChildrenOf(move(parentStmt));
}

string PKB::getParentOf(string childStmt) const { return parentTable->getParentOf(move(childStmt)); }

vector<pair<string, string>> PKB::getAllParent() const { return parentTable->getParentEntries(); }

bool PKB::isParentT(string stmt1, string stmt2) const { return parentTable->isParentT(move(stmt1), std::move(stmt2)); }

unordered_set<string> PKB::getDescendantStmtsOf(string parentStmt) const {
    return parentTable->getAllDescendantsOf(move(parentStmt));
}

unordered_set<string> PKB::getAncestorStmtsOf(string childStmt) const {
    return parentTable->getAllAncestorsOf(move(childStmt));
}

vector<pair<string, string>> PKB::getAllParentT() const { return parentTable->getParentTEntries(); }

unordered_set<string> PKB::getAllStmtsParentOfSomeStmt() const { return parentTable->getStmtsParentOfSomeStmt(); }

unordered_set<string> PKB::getAllStmtsChildOfBySomeStmt() const { return parentTable->getStmtsChildOfSomeStmt(); }

//======================================== Uses ==================================================

bool PKB::isUsesS(string stmtNum, string varName) const {
    return usesTable->isUsesS(move(stmtNum), move(varName));
}

unordered_set<string> PKB::getUsesSByVar(string varName) const { return usesTable->getStmtsUsingVar(move(varName)); }

unordered_set<string> PKB::getUsesByStmt(string stmtNum) const { return usesTable->getVarsUsedInStmt(move(stmtNum)); }

vector<pair<string, string>> PKB::getAllUsesS() const { return usesTable->getStmtsVarEntries(); }

unordered_set<string> PKB::getAllStmtsUsingSomeVar() const { return usesTable->getStmtsUsingSomeVar(); }

void PKB::registerUsesS(const string& stmtNum, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerUsesS] Used variable is not registered" << endl;
    }
    if (!isAssignStmt(stmtNum) && !isPrintStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerUsesS] Statement is not of type assign/print/if/while" << endl;
    }
    return usesTable->setVarUsedInStmt(stmtNum, varName);
}

bool PKB::isUsesP(string procName, string varName) const {
    return usesTable->isUsesP(move(procName), move(varName));
}

unordered_set<string> PKB::getUsesPByVar(string varName) const { return usesTable->getProcsUsingVar(move(varName)); }

unordered_set<string> PKB::getUsesByProc(string procName) const { return usesTable->getVarsUsedInProc(move(procName)); }

vector<pair<string, string>> PKB::getAllUsesP() const { return usesTable->getProcVarEntries(); }

unordered_set<string> PKB::getAllProcsUsingSomeVar() const { return usesTable->getProcsUsingSomeVar(); }

void PKB::registerUsesP(const string& procName, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerUsesP] Used variable is not registered" << endl;
    }
    if (!(isProcedure(procName))) {
        cout << "Warning: " << "[PKB][registerUsesP] Procedure is not registered" << endl;
    }
    return usesTable->setVarUsedInProc(procName, varName);
}

//======================================== Modifies ==================================================

bool PKB::isModifiesS(string stmtNum, string varName) const {
    return modifiesTable->isModifiesS(move(stmtNum), move(varName));
}

unordered_set<string> PKB::getModifiesSByVar(string varName) const {
    return modifiesTable->getStmtsModifyingVar(move(varName));
}

unordered_set<string> PKB::getModifiesByStmt(string stmtNum) const {
    return modifiesTable->getVarsModifiedInStmt(move(stmtNum));
}

vector<pair<string, string>> PKB::getAllModifiesS() const { return modifiesTable->getStmtsVarEntries(); }

unordered_set<string> PKB::getAllStmtsModifyingSomeVar() const { return modifiesTable->getStmtsModifyingSomeVar(); }

void PKB::registerModifiesS(const string& stmtNum, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerModifiesS] Used variable is not registered" << endl;
    }
    if (!isAssignStmt(stmtNum) && !isReadStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerModifiesS] Statement is not of type assign/read/if/while" << endl;
    }
    return modifiesTable->setVarModifiedInStmt(stmtNum, varName);
}

bool PKB::isModifiesP(string procName, string varName) const {
    return modifiesTable->isModifiesP(move(procName), move(varName));
}

unordered_set<string> PKB::getModifiesPByVar(string varName) const {
    return modifiesTable->getProcsModifyingVar(move(varName));
}

unordered_set<string> PKB::getModifiesByProc(string procName) const {
    return modifiesTable->getVarsModifiedInProc(move(procName));
}

vector<pair<string, string>> PKB::getAllModifiesP() const { return modifiesTable->getProcVarEntries(); }

unordered_set<string> PKB::getAllProcsModifyingSomeVar() const { return modifiesTable->getProcsModifyingSomeVar(); }

void PKB::registerModifiesP(const string& procName, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerModifiesP] Used variable is not registered" << endl;
    }
    if (!(isProcedure(procName))) {
        cout << "Warning: " << "[PKB][registerModifiesP] Procedure is not registered" << endl;
    }
    return modifiesTable->setVarModifiedInProc(procName, varName);
}

//======================================== Pattern ==================================================

void PKB::registerPattern(const string& stmtNum, const string& lhsVariable, TNode *rhsAssignAST) {
    return patternTable->setPattern(stmtNum, lhsVariable, rhsAssignAST);
}

unordered_set<string> PKB::getAllStmtsFromFullPattern(TNode *patternAST) const {
    return patternTable->getAllStmtsFromFullPattern(patternAST);
}

unordered_set<string> PKB::getStmtFromFullPatternNVar(TNode *patternAST, string varName) const {
    return patternTable->getStmtFromFullPatternNVar(patternAST, std::move(varName));
}

vector<pair<string, string>> PKB::getStmtNVarFromFullPattern(TNode *patternAST) const {
    return patternTable->getStmtNVarFromFullPattern(patternAST);
}

unordered_set<string> PKB::getAllStmtsFromSubPattern(TNode *subPatternAST) const {
    return patternTable->getAllStmtsFromSubPattern(subPatternAST);
}

unordered_set<string> PKB::getStmtFromSubPatternNVar(TNode *subPatternAST, const string& varName) const {
    return patternTable->getStmtFromSubPatternNVar(subPatternAST, varName);
}

vector<pair<string, string>> PKB::getStmtNVarFromSubPattern(TNode *subPatternAST) const {
    return patternTable->getStmtNVarFromSubPattern(subPatternAST);
}
