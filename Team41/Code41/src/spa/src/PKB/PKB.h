#pragma once

#include <string>
#include <set>

using namespace std;

class StmtTable;  // no need to #include "StmtTable.h" as all I need is pointer
class EntityTable;  // no need to #include "EntityTable.h" as all I need is pointer
class UsesTable;  // no need to #include "UsesTable.h" as all I need is pointer
class ModifiesTable;  // no need to #include "ModifiesTable.h" as all I need is pointer

class PKB {
public:
    PKB();

    ~PKB();

    StmtTable *stmtTable;
    EntityTable *entityTable;
    UsesTable *usesTable;
    ModifiesTable *modifiesTable;

    //======================================== Statement ==================================================

    set<string> getAssigns();// Gets list of assign statement
    set<string> getWhiles();// Gets list of while statement
    set<string> getIfs();// Gets list of if statement
    set<string> getReads();// Gets list of read statement
    set<string> getPrints();// Gets list of print statement
    set<string> getCalls();// Gets list of call statement

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

    set<string> getVariables();// Gets list of variables
    set<string> getConstants();// Gets list of constants
    set<string> getProcedures();// Gets list of procedure names

    void registerVariable(string varName);// Registers a variable to PKB
    void registerConstant(string constVal);// Registers a constant to PKB
    void registerProcedure(string procName);// Registers a procedure to PKB

    //=========================================== Uses ===================================================

    void registerUsesS(string stmtNum, string varName);// Registers to PKB that the specified stmt uses specified var
    bool isUsesS(string stmtNum, string varName);// Checks if specified statement uses specified variable
    set<string> getUsesSByVar(string varName);// Gets list of statements that uses specified variable
    set<string> getUsesByStmt(string stmtNum);// Gets list of variables that is used by specified statement
    set<pair<string, string>> getAllUsesS();// Gets list of stmt-var pair where stmt uses var

    void registerUsesP(string procName, string varName);// Registers to PKB that the specified proc uses specified var
    bool isUsesP(string procName, string varName);// Checks if specified procedure uses specified variable
    set<string> getUsesPByVar(string varName);// Gets list of procedures that uses specified variable
    set<string> getUsesByProc(string procName);// Gets list of variables that is used by specified procedure
    set<pair<string, string>> getAllUsesP();// Gets list of proc-var pair where stmt uses var

    //=========================================== Modifies ===================================================

    void
    registerModifiesS(string stmtNum, string varName);// Registers to PKB that the specified stmt modifies specified var
    bool isModifiesS(string stmtNum, string varName);// Checks if specified statement modifies specified variable
    set<string> getModifiesSByVar(string varName);// Gets list of statements that modifies specified variable
    set<string> getModifiesByStmt(string stmtNum);// Gets list of variables that is modified by specified statement
    set<pair<string, string>> getAllModifiesS();// Gets list of stmt-var pair where stmt modifies var

    void registerModifiesP(string procName,
                           string varName);// Registers to PKB that the specified proc modifies specified var
    bool isModifiesP(string procName, string varName);// Checks if specified procedure modifies specified variable
    set<string> getModifiesPByVar(string varName);// Gets list of procedures that modifies specified variable
    set<string> getModifiesByProc(string procName);// Gets list of variables that is modified by specified procedure
    set<pair<string, string>> getAllModifiesP();// Gets list of proc-var pair where stmt modifies var
};