#include "PKBClient.h"
#include "PKB/Tables/UsesTable.h"
#include "PKB/Tables/ModifiesTable.h"
#include "PKB/Tables/FollowsTable.h"
#include "PKB/Tables/ParentTable.h"
#include "PKB/Tables/PatternTable.h"

using namespace std;


PKBClient::PKBClient() {
    usesTable = new UsesTable();
    modifiesTable = new ModifiesTable();
    followsTable = new FollowsTable();
    parentTable = new ParentTable();
    patternTable = new PatternTable();
}

PKBClient::~PKBClient() {
    delete usesTable;
    delete modifiesTable;
    delete followsTable;
    delete parentTable;
    delete patternTable;
}

//======================================== Follows ==================================================

bool PKBClient::isFollows(string stmt1, string stmt2) const { return followsTable->isFollows(move(stmt1), move(stmt2)); }

string PKBClient::getStmtFollowing(string stmtNum) const { return followsTable->getStmtFollowing(move(stmtNum)); }

string PKBClient::getStmtFollowedBy(string stmtNum) const { return followsTable->getStmtFollowedBy(move(stmtNum)); }

vector<pair<string, string>> PKBClient::getAllFollows() const { return followsTable->getFollowEntries(); }

bool PKBClient::isFollowsT(string stmt1, string stmt2) const { return followsTable->isFollowsT(move(stmt1), move(stmt2)); }

unordered_set<string> PKBClient::getAllStmtsFollowingT(string stmtNum) const {
    return followsTable->getStmtsFollowingT(move(stmtNum));
}

unordered_set<string> PKBClient::getAllStmtsFollowedTBy(string stmtNum) const {
    return followsTable->getStmtsFollowedTBy(move(stmtNum));
}

vector<pair<string, string>> PKBClient::getAllFollowsT() const { return followsTable->getFollowTEntries(); }

unordered_set<string> PKBClient::getAllStmtsFollowingSomeStmt() const { return followsTable->getStmtsFollowingSomeStmt(); }

unordered_set<string> PKBClient::getAllStmtsFollowedBySomeStmt() const { return followsTable->getStmtsFollowedBySomeStmt(); }

//======================================== Parent ==================================================

bool PKBClient::isParent(string stmt1, string stmt2) const { return parentTable->isParent(move(stmt1), move(stmt2)); }

unordered_set<string> PKBClient::getChildStmtsOf(string parentStmt) const {
    return parentTable->getAllChildrenOf(move(parentStmt));
}

string PKBClient::getParentOf(string childStmt) const { return parentTable->getParentOf(move(childStmt)); }

vector<pair<string, string>> PKBClient::getAllParent() const { return parentTable->getParentEntries(); }

bool PKBClient::isParentT(string stmt1, string stmt2) const { return parentTable->isParentT(move(stmt1), std::move(stmt2)); }

unordered_set<string> PKBClient::getDescendantStmtsOf(string parentStmt) const {
    return parentTable->getAllDescendantsOf(move(parentStmt));
}

unordered_set<string> PKBClient::getAncestorStmtsOf(string childStmt) const {
    return parentTable->getAllAncestorsOf(move(childStmt));
}

vector<pair<string, string>> PKBClient::getAllParentT() const { return parentTable->getParentTEntries(); }

unordered_set<string> PKBClient::getAllStmtsParentOfSomeStmt() const { return parentTable->getStmtsParentOfSomeStmt(); }

unordered_set<string> PKBClient::getAllStmtsChildOfBySomeStmt() const { return parentTable->getStmtsChildOfSomeStmt(); }

//======================================== Uses ==================================================

bool PKBClient::isUsesS(string stmtNum, string varName) const {
    return usesTable->isUsesS(move(stmtNum), move(varName));
}

unordered_set<string> PKBClient::getUsesSByVar(string varName) const { return usesTable->getStmtsUsingVar(move(varName)); }

unordered_set<string> PKBClient::getUsesByStmt(string stmtNum) const { return usesTable->getVarsUsedInStmt(move(stmtNum)); }

vector<pair<string, string>> PKBClient::getAllUsesS() const { return usesTable->getStmtsVarEntries(); }

unordered_set<string> PKBClient::getAllStmtsUsingSomeVar() const { return usesTable->getStmtsUsingSomeVar(); }

bool PKBClient::isUsesP(string procName, string varName) const {
    return usesTable->isUsesP(move(procName), move(varName));
}

unordered_set<string> PKBClient::getUsesPByVar(string varName) const { return usesTable->getProcsUsingVar(move(varName)); }

unordered_set<string> PKBClient::getUsesByProc(string procName) const { return usesTable->getVarsUsedInProc(move(procName)); }

vector<pair<string, string>> PKBClient::getAllUsesP() const { return usesTable->getProcVarEntries(); }

unordered_set<string> PKBClient::getAllProcsUsingSomeVar() const { return usesTable->getProcsUsingSomeVar(); }

//======================================== Modifies ==================================================

bool PKBClient::isModifiesS(string stmtNum, string varName) const {
    return modifiesTable->isModifiesS(move(stmtNum), move(varName));
}

unordered_set<string> PKBClient::getModifiesSByVar(string varName) const {
    return modifiesTable->getStmtsModifyingVar(move(varName));
}

unordered_set<string> PKBClient::getModifiesByStmt(string stmtNum) const {
    return modifiesTable->getVarsModifiedInStmt(move(stmtNum));
}

vector<pair<string, string>> PKBClient::getAllModifiesS() const { return modifiesTable->getStmtsVarEntries(); }

unordered_set<string> PKBClient::getAllStmtsModifyingSomeVar() const { return modifiesTable->getStmtsModifyingSomeVar(); }

bool PKBClient::isModifiesP(string procName, string varName) const {
    return modifiesTable->isModifiesP(move(procName), move(varName));
}

unordered_set<string> PKBClient::getModifiesPByVar(string varName) const {
    return modifiesTable->getProcsModifyingVar(move(varName));
}

unordered_set<string> PKBClient::getModifiesByProc(string procName) const {
    return modifiesTable->getVarsModifiedInProc(move(procName));
}

vector<pair<string, string>> PKBClient::getAllModifiesP() const { return modifiesTable->getProcVarEntries(); }

unordered_set<string> PKBClient::getAllProcsModifyingSomeVar() const { return modifiesTable->getProcsModifyingSomeVar(); }

//======================================== Pattern ==================================================

unordered_set<string> PKBClient::getAllStmtsFromFullPattern(TNode *patternAST) const {
    return patternTable->getAllStmtsFromFullPattern(patternAST);
}

unordered_set<string> PKBClient::getStmtFromFullPatternNVar(TNode *patternAST, string varName) const {
    return patternTable->getStmtFromFullPatternNVar(patternAST, std::move(varName));
}

vector<pair<string, string>> PKBClient::getStmtNVarFromFullPattern(TNode *patternAST) const {
    return patternTable->getStmtNVarFromFullPattern(patternAST);
}

unordered_set<string> PKBClient::getAllStmtsFromSubPattern(TNode *subPatternAST) const {
    return patternTable->getAllStmtsFromSubPattern(subPatternAST);
}

unordered_set<string> PKBClient::getStmtFromSubPatternNVar(TNode *subPatternAST, const string& varName) const {
    return patternTable->getStmtFromSubPatternNVar(subPatternAST, varName);
}

vector<pair<string, string>> PKBClient::getStmtNVarFromSubPattern(TNode *subPatternAST) const {
    return patternTable->getStmtNVarFromSubPattern(subPatternAST);
}

