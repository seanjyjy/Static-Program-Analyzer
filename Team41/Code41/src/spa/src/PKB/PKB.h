#pragma once

#include <string>
#include <unordered_set>

using namespace std;

class StmtTable;
class EntityTable;
class UsesTable;
class ModifiesTable;
class FollowsTable;
class CallsTable;
class ParentTable;

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

    //======================================== Statement ==================================================

    unordered_set<string> getAssigns();// Gets list of assign statement
    unordered_set<string> getWhiles();// Gets list of while statement
    unordered_set<string> getIfs();// Gets list of if statement
    unordered_set<string> getReads();// Gets list of read statement
    unordered_set<string> getPrints();// Gets list of print statement
    unordered_set<string> getCalls();// Gets list of call statement

    /**
     * Registers a assign statement to PKB
     * @throws error if statement already has a type
     */
    void registerAssign(string stmtNum);

    /**
     * Registers a while statement to PKB
     * @throws error if statement already has a type
     */
    void registerWhile(string stmtNum);

    /**
     * Registers a if statement to PKB
     * @throws error if statement already has a type
     */
    void registerIf(string stmtNum);

    /**
     * Registers a read statement to PKB
     * @throws error if statement already has a type
     */
    void registerRead(string stmtNum);

    /**
     * Registers a print statement to PKB
     * @throws error if statement already has a type
     */
    void registerPrint(string stmtNum);

    /**
     * Registers a call statement to PKB
     * @throws error if statement already has a type
     */
    void registerCall(string stmtNum);

    bool isAssignStmt(string stmtNum);// Checks if statement is an assignment
    bool isWhileStmt(string stmtNum);// Checks if statement is a while statement
    bool isIfStmt(string stmtNum);// Checks if statement is an if statement
    bool isReadStmt(string stmtNum);// Checks if statement is a read statement
    bool isPrintStmt(string stmtNum);// Checks if statement is a print statement
    bool isCallStmt(string stmtNum);// Checks if statement is a call statement

    /**
     * Gets the number of statements stored in table
     *
     * @return set of statement numbers
     */
    int getStatementCount();

    //======================================== Entities ==================================================

    unordered_set<string> getVariables();// Gets list of variables
    unordered_set<string> getConstants();// Gets list of constants
    unordered_set<string> getProcedures();// Gets list of procedure names

    void registerVariable(string varName);// Registers a variable to PKB
    void registerConstant(string constVal);// Registers a constant to PKB
    void registerProcedure(string procName);// Registers a procedure to PKB

    bool isConstant(string constVal);// Checks if specified variable is registered
    bool isProcedure(string procName);// Checks if specified constant is registered
    bool isVariable(string varName);// Checks if specified procedure is registered

    //=========================================== Follows ===================================================

    /**
     * Registers to PKB that the specified stmt1 follows specified stmt2
     * Recursively update any indirect follow relationship between statements as required
     *
     * @param stmt1 the follower statement
     * @param stmt2 the followed statement
     * @throws key_map_err if stmt1 is already following some other statement
     * @throws val_map_err if stmt2 is already followed by some other statement
     * @throws cyclic_err if the registration creates a cycle in the follow graph
     * @throws self_err if statement attempts to follow itself
     */
    void registerFollows(string stmt1, string stmt2);

    bool isFollows(string stmt1, string stmt2); // Checks if stmt1 follows stmt2
    string getStmtFollowing(string stmtNum);// Gets the stmt that follows the specified stmt
    string getStmtFollowedBy(string stmtNum);// Gets the stmt that are follows by the specified stmt
    vector<pair<string, string>> getAllFollows(); // Gets list of stmt1-stmt2 pair where stmt1 follows stmt2

    bool isFollowsT(string stmt1, string stmt2); // Checks if stmt1 followsT stmt2
    unordered_set<string> getAllStmtsFollowingT(string stmtNum);// Gets list of stmts that followsT the specified stmt
    unordered_set<string> getAllStmtsFollowedTBy(string stmtNum);// Gets list of stmts that is followedT by given stmt
    vector<pair<string, string>> getAllFollowsT(); // Gets list of stmt1-stmt2 pair where stmt1 followsT stmt2

    //=========================================== Parent ===================================================

    /**
     * Registers to PKB that the specified stmt1 is parent of specified stmt2.
     * Recursively update any ancestor-descendant relationship as required
     *
     * @param parentStmt the parent statement
     * @param childStmt the child statement
     * @throws val_map_err if stmt2 is already direct child of some other statement
     * @throws cyclic_err if the registration creates a cycle in the follow graph
     * @throws self_err if statement attempts to parent itself
     */
    void registerParent(string parentStmt, string childStmt);

    bool isParent(string stmt1, string stmt2); // Checks if stmt1 is parent of stmt2
    unordered_set<string> getChildStmtsOf(string parentStmt);// Gets the stmts that are direct child of parentStmt
    string getParentOf(string childStmt);// Gets the stmt that is parent of childStmt
    vector<pair<string, string>> getAllParent(); // Gets list of parent-child pair where stmt1 is parent of stmt2

    bool isParentT(string stmt1, string stmt2); // Checks if stmt1 is parent of stmt2
    unordered_set<string> getDescendantStmtsOf(string parentStmt);// Gets the stmts that are descendants of parentStmt
    unordered_set<string> getAncestorStmtsOf(string childStmt);// Gets the stmt that is ancestors of childStmt
    vector<pair<string, string>> getAllParentT(); // Gets list of parent-child pair where stmt1 is ancestor of stmt2

    //=========================================== Uses ===================================================

    void registerUsesS(string stmtNum, string varName);// Registers to PKB that the specified stmt uses specified var
    bool isUsesS(string stmtNum, string varName);// Checks if specified statement uses specified variable
    unordered_set<string> getUsesSByVar(string varName);// Gets list of statements that uses specified variable
    unordered_set<string> getUsesByStmt(string stmtNum);// Gets list of variables that is used by specified statement
    vector<pair<string, string>> getAllUsesS();// Gets list of stmt-var pair where stmt uses var

    void registerUsesP(string procName, string varName);// Registers to PKB that the specified proc uses specified var
    bool isUsesP(string procName, string varName);// Checks if specified procedure uses specified variable
    unordered_set<string> getUsesPByVar(string varName);// Gets list of procedures that uses specified variable
    unordered_set<string> getUsesByProc(string procName);// Gets list of variables that is used by specified procedure
    vector<pair<string, string>> getAllUsesP();// Gets list of proc-var pair where stmt uses var

    //=========================================== Modifies ===================================================

    void registerModifiesS(string stmtNum, string varName); // Registers to PKB that the stmt modifies specified var
    bool isModifiesS(string stmtNum, string varName);// Checks if specified statement modifies specified variable
    unordered_set<string> getModifiesSByVar(string varName);// Gets list of statements that modifies specified variable
    unordered_set<string> getModifiesByStmt(string stmtNum);// Gets list of variables that is modified by statement
    vector<pair<string, string>> getAllModifiesS();// Gets list of stmt-var pair where stmt modifies var

    void registerModifiesP(string procName, string varName);// Registers to PKB that the proc modifies specified var
    bool isModifiesP(string procName, string varName);// Checks if specified procedure modifies specified variable
    unordered_set<string> getModifiesPByVar(string varName);// Gets list of procedures that modifies specified variable
    unordered_set<string> getModifiesByProc(string procName);// Gets list of variables that is modified by procedure
    vector<pair<string, string>> getAllModifiesP();// Gets list of proc-var pair where stmt modifies var

};