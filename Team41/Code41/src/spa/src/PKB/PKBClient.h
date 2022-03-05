#pragma once

#include <vector>
#include "PKBBaseClient.h"
#include "Common/TNode.h"

using namespace std;

class UsesTable;
class ModifiesTable;
class FollowsTable;
class CallsTable;
class ParentTable;
class PatternTable;

/**
 * Used for retrieving abstractions for SIMPLE source code.
 *
 * This PKB facade class provides methods to
 * access the information stored in various relational table.
 */
class PKBClient : public PKBBaseClient {
protected:
    UsesTable *usesTable;
    ModifiesTable *modifiesTable;
    FollowsTable *followsTable;
    ParentTable *parentTable;
    PatternTable *patternTable;
    CallsTable *callsTable;

public:
    PKBClient();

    ~PKBClient();

    //======================================== Calls ==================================================

    bool isCalls(string proc1, string proc2); // Checks if proc1 calls proc2
    unordered_set<string> getAllProcCalling(string procName);// Gets list of procedures that calls proc
    unordered_set<string> getAllProcCalledBy(string procName);// Gets list of procedures that is called by proc
    vector<pair<string, string>> getAllCalls(); // Gets list of proc1-proc2 pair where proc1 calls proc2

    bool isCallsT(string proc1, string proc2); // Checks if proc1 callsT proc2
    unordered_set<string> getAllProcCallingT(string procName);// Gets list of procedures that callsT proc
    unordered_set<string> getAllProcCalledTBy(string procName);// Gets list of procedures that is calledT by proc
    vector<pair<string, string>> getAllCallsT(); // Gets list of proc1-proc2 pair where proc1 callsT proc2

    unordered_set<string> getAllProcsCallingSomeProcs() const; // Get list of procedures calling some proc
    unordered_set<string> getAllProcsCalledBySomeProcs() const; // Get list of procedures called by some proc

    //=========================================== Follows ===================================================

    bool isFollows(string stmt1, string stmt2) const; // Checks if stmt1 follows stmt2
    string getStmtFollowing(string stmtNum) const;// Gets the stmt that follows the specified stmt
    string getStmtFollowedBy(string stmtNum) const;// Gets the stmt that are follows by the specified stmt
    vector<pair<string, string>> getAllFollows() const; // Gets list of stmt1-stmt2 pair where stmt1 follows stmt2

    bool isFollowsT(string stmt1, string stmt2) const; // Checks if stmt1 followsT stmt2
    unordered_set<string> getAllStmtsFollowingT(string stmtNum) const;// Gets list of stmts that followsT the specified stmt
    unordered_set<string> getAllStmtsFollowedTBy(string stmtNum) const;// Gets list of stmts that is followedT by given stmt
    vector<pair<string, string>> getAllFollowsT() const; // Gets list of stmt1-stmt2 pair where stmt1 followsT stmt2

    unordered_set<string> getAllStmtsFollowingSomeStmt() const; // Get list of stmt following some stmt
    unordered_set<string> getAllStmtsFollowedBySomeStmt() const; // Get list of stmt followed by some stmt

    //=========================================== Parent ===================================================

    bool isParent(string stmt1, string stmt2) const; // Checks if stmt1 is parent of stmt2
    unordered_set<string> getChildStmtsOf(string parentStmt) const;// Gets the stmts that are direct child of parentStmt
    string getParentOf(string childStmt) const;// Gets the stmt that is parent of childStmt
    vector<pair<string, string>> getAllParent() const; // Gets list of parent-child pair where stmt1 is parent of stmt2

    bool isParentT(string stmt1, string stmt2) const; // Checks if stmt1 is parentT of stmt2
    unordered_set<string> getDescendantStmtsOf(string parentStmt) const; // Gets the stmts that are descendants of parentStmt
    unordered_set<string> getAncestorStmtsOf(string childStmt) const; // Gets the stmts that are ancestors of childStmt
    vector<pair<string, string>> getAllParentT() const; // Gets list of parent-child pair where stmt1 is ancestor of stmt2

    unordered_set<string> getAllStmtsParentOfSomeStmt() const; // get list of stmt parent of some stmt
    unordered_set<string> getAllStmtsChildOfSomeStmt() const; // get list of stmt parented by some stmt

    //=========================================== Uses ===================================================

    bool isUsesS(string stmtNum, string varName) const;// Checks if specified statement uses specified variable
    unordered_set<string> getUsesSByVar(string varName) const;// Gets list of statements that uses specified variable
    unordered_set<string> getUsesByStmt(string stmtNum) const;// Gets list of variables that is used by specified statement
    vector<pair<string, string>> getAllUsesS() const;// Gets list of stmt-var pair where stmt uses var
    unordered_set<string> getAllStmtsUsingSomeVar() const;// Gets list of stmt where stmt uses some var

    bool isUsesP(string procName, string varName) const;// Checks if specified procedure uses specified variable
    unordered_set<string> getUsesPByVar(string varName) const;// Gets list of procedures that uses specified variable
    unordered_set<string> getUsesByProc(string procName) const;// Gets list of variables that is used by specified procedure
    vector<pair<string, string>> getAllUsesP() const;// Gets list of proc-var pair where stmt uses var
    unordered_set<string> getAllProcsUsingSomeVar() const;// Gets list of proc where proc uses some var

    //=========================================== Modifies ===================================================

    bool isModifiesS(string stmtNum, string varName) const;// Checks if specified statement modifies specified variable
    unordered_set<string> getModifiesSByVar(string varName) const;// Gets list of statements that modifies specified variable
    unordered_set<string> getModifiesByStmt(string stmtNum) const;// Gets list of variables that is modified by statement
    vector<pair<string, string>> getAllModifiesS() const;// Gets list of stmt-var pair where stmt modifies var
    unordered_set<string> getAllStmtsModifyingSomeVar() const;// Gets list of stmt where stmt modifies some var

    bool isModifiesP(string procName, string varName) const;// Checks if specified procedure modifies specified variable
    unordered_set<string> getModifiesPByVar(string varName) const;// Gets list of procedures that modifies specified variable
    unordered_set<string> getModifiesByProc(string procName) const;// Gets list of variables that is modified by procedure
    vector<pair<string, string>> getAllModifiesP() const;// Gets list of proc-var pair where proc modifies var
    unordered_set<string> getAllProcsModifyingSomeVar() const;// Gets list of proc where proc modifies some var

    //=========================================== Pattern ===================================================

    unordered_set<string> getAllStmtsFromFullPattern(TNode *patternAST) const;// Gets list of assignStmt from pattern
    unordered_set<string> getStmtFromFullPatternNVar(TNode *patternAST, string varName) const;// Gets list of assignStmt from pattern and varName
    vector<pair<string, string>> getStmtNVarFromFullPattern(TNode *patternAST) const;// Gets list of assignStmt-varName from pattern

    unordered_set<string> getAllStmtsFromSubPattern(TNode *subPatternAST) const;// Gets list of assignStmt from subpattern
    unordered_set<string> getStmtFromSubPatternNVar(TNode *subPatternAST, const string& varName) const;// Gets list of assignStmt from subpattern and varName
    vector<pair<string, string>> getStmtNVarFromSubPattern(TNode *subPatternAST) const;// Gets list of assignStmt-varName from subpattern
};