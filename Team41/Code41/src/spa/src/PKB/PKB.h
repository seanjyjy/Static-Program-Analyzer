#pragma once

#include <string>
#include <unordered_set>
#include "Common/TNode.h"

using namespace std;

class StmtTable;
class EntityTable;
class UsesTable;
class ModifiesTable;
class FollowsTable;
class CallsTable;
class ParentTable;
class PatternTable;

class PKB {
public:
    PKB();

    ~PKB();

    StmtTable *stmtTable;
    EntityTable *entityTable;
    UsesTable *usesTable;
    ModifiesTable *modifiesTable;
    FollowsTable *followsTable;
    ParentTable *parentTable;
    PatternTable *patternTable;

    //======================================== Statement ==================================================

    unordered_set<string> getAssigns() const;// Gets list of assign statement
    unordered_set<string> getWhiles() const;// Gets list of while statement
    unordered_set<string> getIfs() const;// Gets list of if statement
    unordered_set<string> getReads() const;// Gets list of read statement
    unordered_set<string> getPrints() const;// Gets list of print statement
    unordered_set<string> getCalls() const;// Gets list of call statement

    /**
     * Registers a assign statement to PKB
     * @throws error if statement already has a type
     */
    void registerAssign(const string& stmtNum);

    /**
     * Registers a while statement to PKB
     * @throws error if statement already has a type
     */
    void registerWhile(const string& stmtNum);

    /**
     * Registers a if statement to PKB
     * @throws error if statement already has a type
     */
    void registerIf(const string& stmtNum);

    /**
     * Registers a read statement to PKB
     * @throws error if statement already has a type
     */
    void registerRead(const string& stmtNum);

    /**
     * Registers a print statement to PKB
     * @throws error if statement already has a type
     */
    void registerPrint(const string& stmtNum);

    /**
     * Registers a call statement to PKB
     * @throws error if statement already has a type
     */
    void registerCall(const string& stmtNum);

    bool isAssignStmt(string stmtNum) const;// Checks if statement is an assignment
    bool isWhileStmt(string stmtNum) const;// Checks if statement is a while statement
    bool isIfStmt(string stmtNum) const;// Checks if statement is an if statement
    bool isReadStmt(string stmtNum) const;// Checks if statement is a read statement
    bool isPrintStmt(string stmtNum) const;// Checks if statement is a print statement
    bool isCallStmt(string stmtNum) const;// Checks if statement is a call statement

    /**
     * Gets the number of statements stored in table
     *
     * @return set of statement numbers
     */
    int getStatementCount() const;

    //======================================== Entities ==================================================

    unordered_set<string> getVariables() const;// Gets list of variables
    unordered_set<string> getConstants() const;// Gets list of constants
    unordered_set<string> getProcedures() const;// Gets list of procedure names
    unordered_set<string> getStatements() const;// Gets list of statement numbers

    void registerVariable(const string& varName);// Registers a variable to PKB
    void registerConstant(const string& constVal);// Registers a constant to PKB
    void registerProcedure(const string& procName);// Registers a procedure to PKB

    bool isConstant(string constVal) const;// Checks if specified variable is registered
    bool isProcedure(string procName) const;// Checks if specified constant is registered
    bool isVariable(string varName) const;// Checks if specified procedure is registered

    //=========================================== Follows ===================================================

    /**
     * Registers to PKB that the specified stmt1 follows specified stmt2
     *
     * @param stmt1 the follower statement
     * @param stmt2 the followed statement
     * @throws key_map_err if stmt1 is already following some other statement
     * @throws val_map_err if stmt2 is already followed by some other statement
     */
    void registerFollows(const string& stmt1, const string& stmt2);

    /**
     * Registers to PKB that the specified stmt1 followsT specified stmt2
     *
     * @param stmt1 the follower statement
     * @param stmt2 the followed statement
     */
    void registerFollowsT(const string& stmt1, const string& stmt2);

    bool isFollows(string stmt1, string stmt2) const; // Checks if stmt1 follows stmt2
    string getStmtFollowing(string stmtNum) const;// Gets the stmt that follows the specified stmt
    string getStmtFollowedBy(string stmtNum) const;// Gets the stmt that are follows by the specified stmt
    vector<pair<string, string>> getAllFollows() const; // Gets list of stmt1-stmt2 pair where stmt1 follows stmt2

    bool isFollowsT(string stmt1, string stmt2) const; // Checks if stmt1 followsT stmt2
    unordered_set<string> getAllStmtsFollowingT(string stmtNum) const;// Gets list of stmts that followsT the specified stmt
    unordered_set<string> getAllStmtsFollowedTBy(string stmtNum) const;// Gets list of stmts that is followedT by given stmt
    vector<pair<string, string>> getAllFollowsT() const; // Gets list of stmt1-stmt2 pair where stmt1 followsT stmt2

    unordered_set<string> getAllStmtsFollowingSomeStmt() const; // get list of stmt following some stmt
    unordered_set<string> getAllStmtsFollowedBySomeStmt() const; // get list of stmt followed by some stmt

    //=========================================== Parent ===================================================

    /**
     * Registers to PKB that the specified stmt1 is parent of specified stmt2.
     */
    void registerParent(const string& parentStmt, const string& childStmt);

    /**
     * Registers to PKB that the specified stmt1 has ancestor-descendant relation with specified stmt2.
     */
    void registerParentT(const string& parentStmt, const string& childStmt);

    bool isParent(string stmt1, string stmt2) const; // Checks if stmt1 is parent of stmt2
    unordered_set<string> getChildStmtsOf(string parentStmt) const;// Gets the stmts that are direct child of parentStmt
    string getParentOf(string childStmt) const;// Gets the stmt that is parent of childStmt
    vector<pair<string, string>> getAllParent() const; // Gets list of parent-child pair where stmt1 is parent of stmt2

    bool isParentT(string stmt1, string stmt2) const; // Checks if stmt1 is parent of stmt2
    unordered_set<string> getDescendantStmtsOf(string parentStmt) const;// Gets the stmts that are descendants of parentStmt
    unordered_set<string> getAncestorStmtsOf(string childStmt) const;// Gets the stmt that is ancestors of childStmt
    vector<pair<string, string>> getAllParentT() const; // Gets list of parent-child pair where stmt1 is ancestor of stmt2

    unordered_set<string> getAllStmtsParentOfSomeStmt() const; // get list of stmt parent of some stmt
    unordered_set<string> getAllStmtsChildOfBySomeStmt() const; // get list of stmt parented by some stmt

    //=========================================== Uses ===================================================

    void registerUsesS(const string& stmtNum, const string& varName);// Registers to PKB that the specified stmt uses specified var
    bool isUsesS(string stmtNum, string varName) const;// Checks if specified statement uses specified variable
    unordered_set<string> getUsesSByVar(string varName) const;// Gets list of statements that uses specified variable
    unordered_set<string> getUsesByStmt(string stmtNum) const;// Gets list of variables that is used by specified statement
    vector<pair<string, string>> getAllUsesS() const;// Gets list of stmt-var pair where stmt uses var
    unordered_set<string> getAllStmtsUsingSomeVar() const;// Gets list of stmt where stmt uses some var

    void registerUsesP(const string& procName, const string& varName);// Registers to PKB that the specified proc uses specified var
    bool isUsesP(string procName, string varName) const;// Checks if specified procedure uses specified variable
    unordered_set<string> getUsesPByVar(string varName) const;// Gets list of procedures that uses specified variable
    unordered_set<string> getUsesByProc(string procName) const;// Gets list of variables that is used by specified procedure
    vector<pair<string, string>> getAllUsesP() const;// Gets list of proc-var pair where stmt uses var
    unordered_set<string> getAllProcsUsingSomeVar() const;// Gets list of proc where proc uses some var

    //=========================================== Modifies ===================================================

    void registerModifiesS(const string& stmtNum, const string& varName); // Registers to PKB that the stmt modifies specified var
    bool isModifiesS(string stmtNum, string varName) const;// Checks if specified statement modifies specified variable
    unordered_set<string> getModifiesSByVar(string varName) const;// Gets list of statements that modifies specified variable
    unordered_set<string> getModifiesByStmt(string stmtNum) const;// Gets list of variables that is modified by statement
    vector<pair<string, string>> getAllModifiesS() const;// Gets list of stmt-var pair where stmt modifies var
    unordered_set<string> getAllStmtsModifyingSomeVar() const;// Gets list of stmt where stmt modifies some var

    void registerModifiesP(const string& procName, const string& varName);// Registers to PKB that the proc modifies specified var
    bool isModifiesP(string procName, string varName) const;// Checks if specified procedure modifies specified variable
    unordered_set<string> getModifiesPByVar(string varName) const;// Gets list of procedures that modifies specified variable
    unordered_set<string> getModifiesByProc(string procName) const;// Gets list of variables that is modified by procedure
    vector<pair<string, string>> getAllModifiesP() const;// Gets list of proc-var pair where stmt modifies var
    unordered_set<string> getAllProcsModifyingSomeVar() const;// Gets list of proc where proc modifies some var

    //=========================================== Pattern ===================================================

    /**
     * Registers to PKB that the specified assignment stmt has the following pattern
     *
     * @param stmtNum assign statement
     * @param assignAST
     */
    void registerPattern(const string& stmtNum, const string& lhsVariable, TNode *rhsAssignAST);

    unordered_set<string> getAllStmtsFromFullPattern(TNode *patternAST) const;// Gets list of assignStmt from pattern
    unordered_set<string> getStmtFromFullPatternNVar(TNode *patternAST, string varName) const;// Gets list of assignStmt from pattern and varName
    vector<pair<string, string>> getStmtNVarFromFullPattern(TNode *patternAST) const;// Gets list of assignStmt-varName from pattern

    unordered_set<string> getAllStmtsFromSubPattern(TNode *subPatternAST) const;// Gets list of assignStmt from subpattern
    unordered_set<string> getStmtFromSubPatternNVar(TNode *subPatternAST, const string& varName) const;// Gets list of assignStmt from subpattern and varName
    vector<pair<string, string>> getStmtNVarFromSubPattern(TNode *subPatternAST) const;// Gets list of assignStmt-varName from subpattern

};