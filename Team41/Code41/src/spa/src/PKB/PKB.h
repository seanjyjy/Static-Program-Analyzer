#pragma once

#include <string>
#include <set>
#include "StmtType.h"

using namespace std;

class StmtTable;  // no need to #include "StmtTable.h" as all I need is pointer
class EntityTable;  // no need to #include "EntityTable.h" as all I need is pointer

class PKB {
public:
    PKB();
    ~PKB();

    StmtTable *stmtTable;
    EntityTable *entityTable;

    set<string> getAssigns();// Gets list of assign statement
    set<string> getWhiles();// Gets list of while statement
    set<string> getIfs();// Gets list of if statement
    set<string> getReads();// Gets list of read statement
    set<string> getPrints();// Gets list of print statement
    set<string> getCalls();// Gets list of call statement
    set<string> getVariables();// Gets list of variables
    set<string> getConstants();// Gets list of constants
    set<string> getProcedures();// Gets list of procedure names

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

    void registerVariable(string varName);// Registers a variable to PKB
    void registerConstant(string constVal);// Registers a constant to PKB
    void registerProcedure(string procName);// Registers a procedure to PKB

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
};