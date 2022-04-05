#pragma once

#include <vector>
#include <unordered_set>
#include "Common/AstNode/TNode.h"

using namespace std;

class UsesTable;

class ModifiesTable;

class FollowsTable;

class CallsTable;

class ParentTable;

/**
 * Used for retrieving abstractions for SIMPLE source code.
 *
 * This PKB facade class provides methods to
 * access the abstraction information stored in various relational table.
 */
class PKBAbstractionClient {
protected:
    UsesTable *usesTable;
    ModifiesTable *modifiesTable;
    FollowsTable *followsTable;
    ParentTable *parentTable;
    CallsTable *callsTable;

public:
    PKBAbstractionClient();

    ~PKBAbstractionClient();

    //======================================== Calls ==================================================

    /**
     * Checks if proc1 calls proc2
     */
    bool isCalls(const string &proc1, const string &proc2) const;

    /**
     * Gets list of procedures that calls proc
     */
    unordered_set<string> getAllProcCalling(const string &procName) const;

    /**
     * Gets list of procedures that is called by proc
     */
    unordered_set<string> getAllProcCalledBy(const string &procName) const;

    /**
     * Gets list of proc1-proc2 pair where proc1 calls proc2
     */
    vector<pair<string, string>> getAllCalls() const;

    /**
     * Checks if proc1 callsT proc2
     */
    bool isCallsT(const string &proc1, const string &proc2) const;

    /**
     * Gets list of procedures that callsT proc
     */
    unordered_set<string> getAllProcCallingT(const string &procName) const;

    /**
     * Gets list of procedures that is calledT by proc
     */
    unordered_set<string> getAllProcCalledTBy(const string &procName) const;

    /**
     * Gets list of proc1-proc2 pair where proc1 callsT proc2
     */
    vector<pair<string, string>> getAllCallsT() const;

    /**
     * Get list of procedures calling some proc
     */
    unordered_set<string> getAllProcsCallingSomeProcs() const;

    /**
     * Get list of procedures called by some proc
     */
    unordered_set<string> getAllProcsCalledBySomeProcs() const;

    //=========================================== Follows ===================================================

    /**
     * Checks if stmt1 follows stmt2
     */
    bool isFollows(const string &stmt1, const string &stmt2) const;

    /**
     * Gets the stmt that follows the specified stmt
     */
    string getStmtDirectlyAfter(const string &stmt) const;

    /**
     * Gets the stmt that are follows by the specified stmt
     */
    string getStmtDirectlyBefore(const string &stmt) const;

    /**
     * Gets list of stmt1-stmt2 pair where stmt1 follows stmt2
     */
    vector<pair<string, string>> getAllFollows() const;

    /**
     * Checks if stmt1 followsT stmt2
     */
    bool isFollowsT(const string &stmt1, const string &stmt2) const;

    /**
     * Gets list of stmts that followsT the specified stmt
     */
    unordered_set<string> getAllStmtsAfter(const string &stmt) const;

    /**
     * Gets list of stmts that is followedT by given stmt
     */
    unordered_set<string> getAllStmtsBefore(const string &stmt) const;

    /**
     * Gets list of stmt1-stmt2 pair where stmt1 followsT stmt2
     */
    vector<pair<string, string>> getAllFollowsT() const;

    /**
     * Get list of stmt following some stmt
     */
    unordered_set<string> getAllStmtsAfterSomeStmt() const;

    /**
     * Get list of stmt followed by some stmt
     */
    unordered_set<string> getAllStmtsBeforeSomeStmt() const;

    //=========================================== Parent ===================================================

    /**
     * Checks if stmt1 is parent of stmt2
     */
    bool isParent(const string &stmt1, const string &stmt2) const;

    /**
     * Gets the stmts that are direct child of parentStmt
     */
    unordered_set<string> getChildStmtsOf(const string &parentStmt) const;

    /**
     * Gets the stmt that is parent of childStmt
     */
    string getParentOf(const string &childStmt) const;

    /**
     * Gets list of parent-child pair where stmt1 is parent of stmt2
     */
    vector<pair<string, string>> getAllParent() const;

    /**
     * Checks if stmt1 is parentT of stmt2
     */
    bool isParentT(const string &stmt1, const string &stmt2) const;

    /**
     * Gets the stmts that are descendants of parentStmt
     */
    unordered_set<string> getDescendantStmtsOf(const string &parentStmt) const;

    /**
     * Gets the stmts that are ancestors of childStmt
     */
    unordered_set<string> getAncestorStmtsOf(const string &childStmt) const;

    /**
     * Gets list of parent-child pair where stmt1 is ancestor of stmt2
     */
    vector<pair<string, string>> getAllParentT() const;

    /**
     * get list of stmt parent of some stmt
     */
    unordered_set<string> getAllStmtsParentOfSomeStmt() const;

    /**
     * get list of stmt parented by some stmt
     */
    unordered_set<string> getAllStmtsChildOfSomeStmt() const;

    //=========================================== Uses ===================================================

    /**
     * Checks if specified statement uses specified variable
     */
    bool isUsesS(const string &stmtNum, const string &varName) const;

    /**
     * Gets list of statements that uses specified variable
     */
    unordered_set<string> getUsesSByVar(const string &varName) const;

    /**
     * Gets list of variables that is used by specified statement
     */
    unordered_set<string> getUsesByStmt(const string &stmtNum) const;

    /**
     * Gets list of stmt-var pair where stmt uses var
     */
    vector<pair<string, string>> getAllUsesS() const;

    /**
     * Gets list of stmt where stmt uses some var
     */
    unordered_set<string> getAllStmtsUsingSomeVar() const;

    /**
     * Gets list of variable used by some stmt
     */
    unordered_set<string> getAllVarUsedBySomeStmt() const;

    /**
     * Checks if specified procedure uses specified variable
     */
    bool isUsesP(const string &procName, const string &varName) const;

    /**
     * Gets list of procedures that uses specified variable
     */
    unordered_set<string> getUsesPByVar(const string &varName) const;

    /**
     * Gets list of variables that is used by specified procedure
     */
    unordered_set<string> getUsesByProc(const string &procName) const;

    /**
     * Gets list of proc-var pair where stmt uses var
     */
    vector<pair<string, string>> getAllUsesP() const;

    /**
     * Gets list of proc where proc uses some var
     */
    unordered_set<string> getAllProcsUsingSomeVar() const;

    /**
     * Gets list of variable used by some proc
     */
    unordered_set<string> getAllVarUsedBySomeProc() const;

    //=========================================== Modifies ===================================================

    /**
     * Checks if specified statement modifies specified variable
     */
    bool isModifiesS(const string &stmtNum, const string &varName) const;

    /**
     * Gets list of statements that modifies specified variable
     */
    unordered_set<string> getModifiesSByVar(const string &varName) const;

    /**
     * Gets list of variables that is modified by statement
     */
    unordered_set<string> getModifiesByStmt(const string &stmtNum) const;

    /**
     * Gets list of stmt-var pair where stmt modifies var
     */
    vector<pair<string, string>> getAllModifiesS() const;

    /**
     * Gets list of stmt where stmt modifies some var
     */
    unordered_set<string> getAllStmtsModifyingSomeVar() const;

    /**
     * Gets list of variable modified by some stmt
     */
    unordered_set<string> getAllVarModifiedBySomeStmt() const;

    /**
     * Checks if specified procedure modifies specified variable
     */
    bool isModifiesP(const string &procName, const string &varName) const;

    /**
     * Gets list of procedures that modifies specified variable
     */
    unordered_set<string> getModifiesPByVar(const string &varName) const;

    /**
     * Gets list of variables that is modified by procedure
     */
    unordered_set<string> getModifiesByProc(const string &procName) const;

    /**
     * Gets list of proc-var pair where proc modifies var
     */
    vector<pair<string, string>> getAllModifiesP() const;

    /**
     * Gets list of proc where proc modifies some var
     */
    unordered_set<string> getAllProcsModifyingSomeVar() const;

    /**
     * Gets list of variable modified by some proc
     */
    unordered_set<string> getAllVarModifiedBySomeProc() const;
};