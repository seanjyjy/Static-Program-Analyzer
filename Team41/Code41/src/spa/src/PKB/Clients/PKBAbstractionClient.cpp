#include "PKBAbstractionClient.h"
#include "PKB/Tables/UsesTable.h"
#include "PKB/Tables/ModifiesTable.h"
#include "PKB/Tables/FollowsTable.h"
#include "PKB/Tables/ParentTable.h"
#include "PKB/Tables/CallsTable.h"

using namespace std;


PKBAbstractionClient::PKBAbstractionClient() {
    usesTable = new UsesTable();
    modifiesTable = new ModifiesTable();
    followsTable = new FollowsTable();
    parentTable = new ParentTable();
    callsTable = new CallsTable();
}

PKBAbstractionClient::~PKBAbstractionClient() {
    delete usesTable;
    delete modifiesTable;
    delete followsTable;
    delete parentTable;
    delete callsTable;
}

//======================================== Follows ==================================================

bool PKBAbstractionClient::isFollows(string stmt1, string stmt2) const { return followsTable->isFollows(move(stmt1), move(stmt2)); }

string PKBAbstractionClient::getStmtFollowing(string stmtNum) const { return followsTable->getStmtFollowing(move(stmtNum)); }

string PKBAbstractionClient::getStmtFollowedBy(string stmtNum) const { return followsTable->getStmtFollowedBy(move(stmtNum)); }

vector<pair<string, string>> PKBAbstractionClient::getAllFollows() const { return followsTable->getFollowEntries(); }

bool PKBAbstractionClient::isFollowsT(string stmt1, string stmt2) const { return followsTable->isFollowsT(move(stmt1), move(stmt2)); }

unordered_set<string> PKBAbstractionClient::getAllStmtsFollowingT(string stmtNum) const {
    return followsTable->getStmtsFollowingT(move(stmtNum));
}

unordered_set<string> PKBAbstractionClient::getAllStmtsFollowedTBy(string stmtNum) const {
    return followsTable->getStmtsFollowedTBy(move(stmtNum));
}

vector<pair<string, string>> PKBAbstractionClient::getAllFollowsT() const { return followsTable->getFollowTEntries(); }

unordered_set<string> PKBAbstractionClient::getAllStmtsFollowingSomeStmt() const { return followsTable->getStmtsFollowingSomeStmt(); }

unordered_set<string> PKBAbstractionClient::getAllStmtsFollowedBySomeStmt() const { return followsTable->getStmtsFollowedBySomeStmt(); }

//======================================== Parent ==================================================

bool PKBAbstractionClient::isParent(string stmt1, string stmt2) const { return parentTable->isParent(move(stmt1), move(stmt2)); }

unordered_set<string> PKBAbstractionClient::getChildStmtsOf(string parentStmt) const {
    return parentTable->getAllChildrenOf(move(parentStmt));
}

string PKBAbstractionClient::getParentOf(string childStmt) const { return parentTable->getParentOf(move(childStmt)); }

vector<pair<string, string>> PKBAbstractionClient::getAllParent() const { return parentTable->getParentEntries(); }

bool PKBAbstractionClient::isParentT(string stmt1, string stmt2) const { return parentTable->isParentT(move(stmt1), std::move(stmt2)); }

unordered_set<string> PKBAbstractionClient::getDescendantStmtsOf(string parentStmt) const {
    return parentTable->getAllDescendantsOf(move(parentStmt));
}

unordered_set<string> PKBAbstractionClient::getAncestorStmtsOf(string childStmt) const {
    return parentTable->getAllAncestorsOf(move(childStmt));
}

vector<pair<string, string>> PKBAbstractionClient::getAllParentT() const { return parentTable->getParentTEntries(); }

unordered_set<string> PKBAbstractionClient::getAllStmtsParentOfSomeStmt() const { return parentTable->getStmtsParentOfSomeStmt(); }

unordered_set<string> PKBAbstractionClient::getAllStmtsChildOfSomeStmt() const { return parentTable->getStmtsChildOfSomeStmt(); }

//======================================== Uses ==================================================

bool PKBAbstractionClient::isUsesS(string stmtNum, string varName) const {
    return usesTable->isUsesS(move(stmtNum), move(varName));
}

unordered_set<string> PKBAbstractionClient::getUsesSByVar(string varName) const { return usesTable->getStmtsUsingVar(move(varName)); }

unordered_set<string> PKBAbstractionClient::getUsesByStmt(string stmtNum) const { return usesTable->getVarsUsedInStmt(move(stmtNum)); }

vector<pair<string, string>> PKBAbstractionClient::getAllUsesS() const { return usesTable->getStmtsVarEntries(); }

unordered_set<string> PKBAbstractionClient::getAllStmtsUsingSomeVar() const { return usesTable->getStmtsUsingSomeVar(); }

bool PKBAbstractionClient::isUsesP(string procName, string varName) const {
    return usesTable->isUsesP(move(procName), move(varName));
}

unordered_set<string> PKBAbstractionClient::getUsesPByVar(string varName) const { return usesTable->getProcsUsingVar(move(varName)); }

unordered_set<string> PKBAbstractionClient::getUsesByProc(string procName) const { return usesTable->getVarsUsedInProc(move(procName)); }

vector<pair<string, string>> PKBAbstractionClient::getAllUsesP() const { return usesTable->getProcVarEntries(); }

unordered_set<string> PKBAbstractionClient::getAllProcsUsingSomeVar() const { return usesTable->getProcsUsingSomeVar(); }

//======================================== Modifies ==================================================

bool PKBAbstractionClient::isModifiesS(string stmtNum, string varName) const {
    return modifiesTable->isModifiesS(move(stmtNum), move(varName));
}

unordered_set<string> PKBAbstractionClient::getModifiesSByVar(string varName) const {
    return modifiesTable->getStmtsModifyingVar(move(varName));
}

unordered_set<string> PKBAbstractionClient::getModifiesByStmt(string stmtNum) const {
    return modifiesTable->getVarsModifiedInStmt(move(stmtNum));
}

vector<pair<string, string>> PKBAbstractionClient::getAllModifiesS() const { return modifiesTable->getStmtsVarEntries(); }

unordered_set<string> PKBAbstractionClient::getAllStmtsModifyingSomeVar() const { return modifiesTable->getStmtsModifyingSomeVar(); }

bool PKBAbstractionClient::isModifiesP(string procName, string varName) const {
    return modifiesTable->isModifiesP(move(procName), move(varName));
}

unordered_set<string> PKBAbstractionClient::getModifiesPByVar(string varName) const {
    return modifiesTable->getProcsModifyingVar(move(varName));
}

unordered_set<string> PKBAbstractionClient::getModifiesByProc(string procName) const {
    return modifiesTable->getVarsModifiedInProc(move(procName));
}

vector<pair<string, string>> PKBAbstractionClient::getAllModifiesP() const { return modifiesTable->getProcVarEntries(); }

unordered_set<string> PKBAbstractionClient::getAllProcsModifyingSomeVar() const { return modifiesTable->getProcsModifyingSomeVar(); }

//======================================== Calls ==================================================

bool PKBAbstractionClient::isCalls(string proc1, string proc2) { return callsTable->isCalls(move(proc1), move(proc2)); }

unordered_set<string> PKBAbstractionClient::getAllProcCalling(string procName) { return callsTable->getProcsCalling(move(procName)); }

unordered_set<string> PKBAbstractionClient::getAllProcCalledBy(string procName) { return callsTable->getProcsCalledBy(move(procName)); }

vector<pair<string, string>> PKBAbstractionClient::getAllCalls() { return callsTable->getCallsEntries(); }

bool PKBAbstractionClient::isCallsT(string proc1, string proc2) { return callsTable->isCallsT(move(proc1), move(proc2)); }

unordered_set<string> PKBAbstractionClient::getAllProcCallingT(string procName) { return callsTable->getProcsCallingT(move(procName)); }

unordered_set<string> PKBAbstractionClient::getAllProcCalledTBy(string procName) {
    return callsTable->getProcsCalledTBy(move(procName));
}

vector<pair<string, string>> PKBAbstractionClient::getAllCallsT() { return callsTable->getCallsTEntries(); }

unordered_set<string> PKBAbstractionClient::getAllProcsCallingSomeProcs() const { return callsTable->getProcsCallingSomeProc(); }

unordered_set<string> PKBAbstractionClient::getAllProcsCalledBySomeProcs() const { return callsTable->getProcsCalledBySomeProc(); }
