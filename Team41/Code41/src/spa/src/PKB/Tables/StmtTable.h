#pragma once

#include "PKB/Relations/OneToMany.h"

using namespace std;

/**
 * The types of SIMPLE statements
 */
enum StmtType {
    UNKNOWN_STMT,
    ASSIGN,
    WHILE,
    IF,
    READ,
    PRINT,
    CALL
};


/**
 * Store list of statement used in the SIMPLE program and their respective types
 * - uses One-Many relation to map statement type to statements and vice versa
 */
class StmtTable {
private:
    OneToMany<StmtType, string> mapping;
public:
    StmtTable();

    /**
     * Gets set of statement that belongs to the specified type
     *
     * @param type the target SIMPLE statement type
     * @return the set of numeric string representing the stmt numbers
     */
    unordered_set<string> getAllStmtsByType(StmtType type);

    /**
     * Gets all the statement type of the specified statement number
     *
     * @param stmtNumber the target statement number in string
     * @return the statement type
     * @throws err if stmtNumber does not exist in table
     */
    StmtType getStmtType(string stmtNumber);

    /**
     * Checks if the statement number match the specified type
     *
     * @param stmtNumber the statement number in string
     * @param type the queried statement type
     * @return true if matches, false otherwise
     */
    bool isStmtType(string stmtNumber, StmtType type);

    /**
     * Set the statement and type into the table
     *
     * @param stmtNum the statement number in string
     * @param type the type of the statement
     * @throws error if type of stmtNumber is already set
     */
    void setStmt(const string& stmtNum, StmtType type);

    /**
     * Gets the number of statements stored in table
     *
     * @return set of statement numbers
     */
    size_t getStmtCount();

    /**
     * Gets all statements added to the table
     *
     * @return set of statement numbers
     */
    unordered_set<string> getAllStmts();

    /**
     * Gets all statement types added to the table
     *
     * @return set of statement types
     */
    unordered_set<StmtType> getAllTypes();
};
