#pragma once

#include <string>
#include <unordered_set>
#include "Common/TNode.h"
#include "PKBClient.h"

using namespace std;

/**
 * Used for storing and retrieving abstractions for SIMPLE source code.
 *
 * The PKBManager facade class provides methods to
 * update or access the various relational table.
 */
class PKBManager: public PKBClient {
public:

    //======================================== Statement ==================================================

    /**
     * Registers a assign statement to PKBManager
     * @throws error if statement already has a type
     */
    void registerAssign(const string& stmtNum);

    /**
     * Registers a while statement to PKBManager
     * @throws error if statement already has a type
     */
    void registerWhile(const string& stmtNum);

    /**
     * Registers a if statement to PKBManager
     * @throws error if statement already has a type
     */
    void registerIf(const string& stmtNum);

    /**
     * Registers a read statement to PKBManager
     * @throws error if statement already has a type
     */
    void registerRead(const string& stmtNum);

    /**
     * Registers a print statement to PKBManager
     * @throws error if statement already has a type
     */
    void registerPrint(const string& stmtNum);

    /**
     * Registers a call statement to PKBManager
     * @throws error if statement already has a type
     */
    void registerCall(const string& stmtNum);

    //======================================== Entities ==================================================

    void registerVariable(const string& varName);// Registers a variable to PKBManager
    void registerConstant(const string& constVal);// Registers a constant to PKBManager
    void registerProcedure(const string& procName);// Registers a procedure to PKBManager

    //=========================================== Calls ===================================================

    /**
     * Registers to PKB that proc1 calls proc2
     *
     * @param proc1 the procedure where it is called from
     * @param proc2 the called procedure
     * @throws cyclic_err if the registration creates a cycle in the call graph
     * @throws self_err if the procedure attempts to call itself
     */
    void registerCalls(const string& proc1, const string& proc2);

    /**
     * Registers to PKB that proc1 callsT proc2
     *
     * @param proc1 the procedure where it is called from
     * @param proc2 the called procedure
     * @throws cyclic_err if the registration creates a cycle in the call graph
     * @throws self_err if the procedure attempts to call itself
     */
    void registerCallsT(const string& proc1, const string& proc2);

    //=========================================== Follows ===================================================

    /**
     * Registers to PKBManager that the specified stmt1 follows specified stmt2
     *
     * @param stmt1 the follower statement
     * @param stmt2 the followed statement
     * @throws key_map_err if stmt1 is already following some other statement
     * @throws val_map_err if stmt2 is already followed by some other statement
     */
    void registerFollows(const string& stmt1, const string& stmt2);

    /**
     * Registers to PKBManager that the specified stmt1 followsT specified stmt2
     *
     * @param stmt1 the follower statement
     * @param stmt2 the followed statement
     */
    void registerFollowsT(const string& stmt1, const string& stmt2);

    //=========================================== Parent ===================================================

    /**
     * Registers to PKBManager that the specified stmt1 is parent of specified stmt2.
     */
    void registerParent(const string& parentStmt, const string& childStmt);

    /**
     * Registers to PKBManager that the specified stmt1 has ancestor-descendant relation with specified stmt2.
     */
    void registerParentT(const string& parentStmt, const string& childStmt);

    //=========================================== Uses ===================================================

    void registerUsesS(const string& stmtNum, const string& varName);// Registers to PKBManager that the specified stmt uses specified var

    void registerUsesP(const string& procName, const string& varName);// Registers to PKBManager that the specified proc uses specified var

    //=========================================== Modifies ===================================================

    void registerModifiesS(const string& stmtNum, const string& varName); // Registers to PKBManager that the stmt modifies specified var

    void registerModifiesP(const string& procName, const string& varName);// Registers to PKBManager that the proc modifies specified var

    //=========================================== Pattern ===================================================

    /**
     * Registers to PKBManager that the specified assignment stmt, modifies the variable and has the following expression pattern
     *
     * @param stmtNum assign statement
     * @param lhsVariable the modified variable in the statement
     * @param assignAST the Abstract Syntax Tree for the RHS expression of the statement
     */
    void registerPattern(const string& stmtNum, const string& lhsVariable, TNode *rhsAssignAST);
};