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

bool PKBAbstractionClient::isFollows(const string &stmt1, const string &stmt2) const {
    return followsTable->isFollows(stmt1, stmt2);
}

string PKBAbstractionClient::getStmtDirectlyAfter(const string &stmtNum) const {
    return followsTable->getStmtDirectlyAfter(stmtNum);
}

string PKBAbstractionClient::getStmtDirectlyBefore(const string &stmtNum) const {
    return followsTable->getStmtDirectlyBefore(stmtNum);
}

vector<pair<string, string>> PKBAbstractionClient::getAllFollows() const {
    return followsTable->getFollowEntries();
}

bool PKBAbstractionClient::isFollowsT(const string &stmt1, const string &stmt2) const {
    return followsTable->isFollowsT(stmt1, stmt2);
}

unordered_set<string> PKBAbstractionClient::getAllStmtsAfter(const string &stmtNum) const {
    return followsTable->getStmtsAfter(stmtNum);
}

unordered_set<string> PKBAbstractionClient::getAllStmtsBefore(const string &stmtNum) const {
    return followsTable->getStmtsBefore(stmtNum);
}

vector<pair<string, string>> PKBAbstractionClient::getAllFollowsT() const {
    return followsTable->getFollowTEntries();
}

unordered_set<string> PKBAbstractionClient::getAllStmtsAfterSomeStmt() const {
    return followsTable->getStmtsAfterSomeStmt();
}

unordered_set<string> PKBAbstractionClient::getAllStmtsBeforeSomeStmt() const {
    return followsTable->getStmtsBeforeSomeStmt();
}

//======================================== Parent ==================================================

bool PKBAbstractionClient::isParent(const string &stmt1, const string &stmt2) const {
    return parentTable->isParent(stmt1, stmt2);
}

unordered_set<string> PKBAbstractionClient::getChildStmtsOf(const string &parentStmt) const {
    return parentTable->getAllChildrenOf(parentStmt);
}

string PKBAbstractionClient::getParentOf(const string &childStmt) const {
    return parentTable->getParentOf(childStmt);
}

vector<pair<string, string>> PKBAbstractionClient::getAllParent() const {
    return parentTable->getParentEntries();
}

bool PKBAbstractionClient::isParentT(const string &stmt1, const string &stmt2) const {
    return parentTable->isParentT(stmt1, stmt2);
}

unordered_set<string> PKBAbstractionClient::getDescendantStmtsOf(const string &parentStmt) const {
    return parentTable->getAllDescendantsOf(parentStmt);
}

unordered_set<string> PKBAbstractionClient::getAncestorStmtsOf(const string &childStmt) const {
    return parentTable->getAllAncestorsOf(childStmt);
}

vector<pair<string, string>> PKBAbstractionClient::getAllParentT() const {
    return parentTable->getParentTEntries();
}

unordered_set<string> PKBAbstractionClient::getAllStmtsParentOfSomeStmt() const {
    return parentTable->getStmtsParentOfSomeStmt();
}

unordered_set<string> PKBAbstractionClient::getAllStmtsChildOfSomeStmt() const {
    return parentTable->getStmtsChildOfSomeStmt();
}

//======================================== Uses ==================================================

bool PKBAbstractionClient::isUsesS(const string &stmtNum, const string &varName) const {
    return usesTable->isUsesS(stmtNum, varName);
}

unordered_set<string> PKBAbstractionClient::getUsesSByVar(const string &varName) const {
    return usesTable->getStmtsUsingVar(varName);
}

unordered_set<string> PKBAbstractionClient::getUsesByStmt(const string &stmtNum) const {
    return usesTable->getVarsUsedInStmt(stmtNum);
}

vector<pair<string, string>> PKBAbstractionClient::getAllUsesS() const {
    return usesTable->getStmtsVarEntries();
}

unordered_set<string> PKBAbstractionClient::getAllStmtsUsingSomeVar() const {
    return usesTable->getStmtsUsingSomeVar();
}

unordered_set<string> PKBAbstractionClient::getAllVarUsedBySomeStmt() const {
    return usesTable->getVarsUsedInSomeStmt();
}

bool PKBAbstractionClient::isUsesP(const string &procName, const string &varName) const {
    return usesTable->isUsesP(procName, varName);
}

unordered_set<string> PKBAbstractionClient::getUsesPByVar(const string &varName) const {
    return usesTable->getProcsUsingVar(varName);
}

unordered_set<string> PKBAbstractionClient::getUsesByProc(const string &procName) const {
    return usesTable->getVarsUsedInProc(procName);
}

vector<pair<string, string>> PKBAbstractionClient::getAllUsesP() const {
    return usesTable->getProcVarEntries();
}

unordered_set<string> PKBAbstractionClient::getAllProcsUsingSomeVar() const {
    return usesTable->getProcsUsingSomeVar();
}

unordered_set<string> PKBAbstractionClient::getAllVarUsedBySomeProc() const {
    return usesTable->getVarsUsedInSomeProc();
}

//======================================== Modifies ==================================================

bool PKBAbstractionClient::isModifiesS(const string &stmtNum, const string &varName) const {
    return modifiesTable->isModifiesS(stmtNum, varName);
}

unordered_set<string> PKBAbstractionClient::getModifiesSByVar(const string &varName) const {
    return modifiesTable->getStmtsModifyingVar(varName);
}

unordered_set<string> PKBAbstractionClient::getModifiesByStmt(const string &stmtNum) const {
    return modifiesTable->getVarsModifiedInStmt(stmtNum);
}

vector<pair<string, string>> PKBAbstractionClient::getAllModifiesS() const {
    return modifiesTable->getStmtsVarEntries();
}

unordered_set<string> PKBAbstractionClient::getAllStmtsModifyingSomeVar() const {
    return modifiesTable->getStmtsModifyingSomeVar();
}

unordered_set<string> PKBAbstractionClient::getAllVarModifiedBySomeStmt() const {
    return modifiesTable->getVarsModifiedInSomeStmt();
}

bool PKBAbstractionClient::isModifiesP(const string &procName, const string &varName) const {
    return modifiesTable->isModifiesP(procName, varName);
}

unordered_set<string> PKBAbstractionClient::getModifiesPByVar(const string &varName) const {
    return modifiesTable->getProcsModifyingVar(varName);
}

unordered_set<string> PKBAbstractionClient::getModifiesByProc(const string &procName) const {
    return modifiesTable->getVarsModifiedInProc(procName);
}

vector<pair<string, string>> PKBAbstractionClient::getAllModifiesP() const {
    return modifiesTable->getProcVarEntries();
}

unordered_set<string> PKBAbstractionClient::getAllProcsModifyingSomeVar() const {
    return modifiesTable->getProcsModifyingSomeVar();
}

unordered_set<string> PKBAbstractionClient::getAllVarModifiedBySomeProc() const {
    return modifiesTable->getVarsModifiedInSomeProc();
}

//======================================== Calls ==================================================

bool PKBAbstractionClient::isCalls(const string &proc1, const string &proc2) const {
    return callsTable->isCalls(proc1, proc2);
}

unordered_set<string> PKBAbstractionClient::getAllProcCalling(const string &procName) const {
    return callsTable->getProcsCalling(procName);
}

unordered_set<string> PKBAbstractionClient::getAllProcCalledBy(const string &procName) const {
    return callsTable->getProcsCalledBy(procName);
}

vector<pair<string, string>> PKBAbstractionClient::getAllCalls() const {
    return callsTable->getCallsEntries();
}

bool PKBAbstractionClient::isCallsT(const string &proc1, const string &proc2) const {
    return callsTable->isCallsT(proc1, proc2);
}

unordered_set<string> PKBAbstractionClient::getAllProcCallingT(const string &procName) const {
    return callsTable->getProcsCallingT(procName);
}

unordered_set<string> PKBAbstractionClient::getAllProcCalledTBy(const string &procName) const {
    return callsTable->getProcsCalledTBy(procName);
}

vector<pair<string, string>> PKBAbstractionClient::getAllCallsT() const {
    return callsTable->getCallsTEntries();
}

unordered_set<string> PKBAbstractionClient::getAllProcsCallingSomeProcs() const {
    return callsTable->getProcsCallingSomeProc();
}

unordered_set<string> PKBAbstractionClient::getAllProcsCalledBySomeProcs() const {
    return callsTable->getProcsCalledBySomeProc();
}
