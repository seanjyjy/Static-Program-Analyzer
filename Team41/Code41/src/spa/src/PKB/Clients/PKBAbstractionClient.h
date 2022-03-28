#pragma once

#include <vector>
#include <unordered_set>
#include "Common/TNode.h"

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
    bool isCalls(string proc1, string proc2) const;

    /**
     * Gets list of procedures that calls proc
     */
    unordered_set<string> getAllProcCalling(string procName) const;

    /**
     * Gets list of procedures that is called by proc
     */
    unordered_set<string> getAllProcCalledBy(string procName) const;

    /**
     * Gets list of proc1-proc2 pair where proc1 calls proc2
     */
    vector<pair<string, string>> getAllCalls() const;

    /**
     * Checks if proc1 callsT proc2
     */
    bool isCallsT(string proc1, string proc2) const;

    /**
     * Gets list of procedures that callsT proc
     */
    unordered_set<string> getAllProcCallingT(string procName) const;

    /**
     * Gets list of procedures that is calledT by proc
     */
    unordered_set<string> getAllProcCalledTBy(string procName) const;

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
    bool isFollows(string stmt1, string stmt2) const;

    /**
     * Gets the stmt that follows the specified stmt
     */
    string getStmtDirectlyAfter(string stmt) const;

    /**
     * Gets the stmt that are follows by the specified stmt
     */
    string getStmtDirectlyBefore(string stmt) const;

    /**
     * Gets list of stmt1-stmt2 pair where stmt1 follows stmt2
     */
    vector<pair<string, string>> getAllFollows() const;

    /**
     * Checks if stmt1 followsT stmt2
     */
    bool isFollowsT(string stmt1, string stmt2) const;

    /**
     * Gets list of stmts that followsT the specified stmt
     */
    unordered_set<string> getAllStmtsAfter(string stmt) const;

    /**
     * Gets list of stmts that is followedT by given stmt
     */
    unordered_set<string> getAllStmtsBefore(string stmt) const;

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
    bool isParent(string stmt1, string stmt2) const;

    /**
     * Gets the stmts that are direct child of parentStmt
     */
    unordered_set<string> getChildStmtsOf(string parentStmt) const;

    /**
     * Gets the stmt that is parent of childStmt
     */
    string getParentOf(string childStmt) const;

    /**
     * Gets list of parent-child pair where stmt1 is parent of stmt2
     */
    vector<pair<string, string>> getAllParent() const;

    /**
     * Checks if stmt1 is parentT of stmt2
     */
    bool isParentT(string stmt1, string stmt2) const;

    /**
     * Gets the stmts that are descendants of parentStmt
     */
    unordered_set<string> getDescendantStmtsOf(string parentStmt) const;

    /**
     * Gets the stmts that are ancestors of childStmt
     */
    unordered_set<string> getAncestorStmtsOf(string childStmt) const;

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
    bool isUsesS(string stmtNum, string varName) const;

    /**
     * Gets list of statements that uses specified variable
     */
    unordered_set<string> getUsesSByVar(string varName) const;

    /**
     * Gets list of variables that is used by specified statement
     */
    unordered_set<string> getUsesByStmt(string stmtNum) const;

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
    bool isUsesP(string procName, string varName) const;

    /**
     * Gets list of procedures that uses specified variable
     */
    unordered_set<string> getUsesPByVar(string varName) const;

    /**
     * Gets list of variables that is used by specified procedure
     */
    unordered_set<string> getUsesByProc(string procName) const;

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
    bool isModifiesS(string stmtNum, string varName) const;

    /**
     * Gets list of statements that modifies specified variable
     */
    unordered_set<string> getModifiesSByVar(string varName) const;

    /**
     * Gets list of variables that is modified by statement
     */
    unordered_set<string> getModifiesByStmt(string stmtNum) const;

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
    bool isModifiesP(string procName, string varName) const;

    /**
     * Gets list of procedures that modifies specified variable
     */
    unordered_set<string> getModifiesPByVar(string varName) const;

    /**
     * Gets list of variables that is modified by procedure
     */
    unordered_set<string> getModifiesByProc(string procName) const;

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