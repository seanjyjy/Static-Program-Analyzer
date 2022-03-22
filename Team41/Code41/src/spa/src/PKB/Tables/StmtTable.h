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
    OneToMany<StmtType, string> stmtMapping;
    unordered_map<string, string> attrMapping;
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
     */
    StmtType getStmtType(string stmtNumber);

    /**
     * Gets the attribute associated with the stmt number
     *
     * @param stmtNumber the target statement number in string
     * @return the statement type
     */
    string getStmtAttr(string stmtNumber);

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
     */
    void setStmt(const string &stmtNum, StmtType type);

    /**
     * Set the statement and associated attribute into the table
     *
     * Extension: If a stmt can support multiple attr in the future,
     * store an association object rather than string
     *
     * @param stmtNum the statement number in string
     * @param attr the attribute value associated to stmt
     */
    void setStmtAttr(const string &stmtNum, const string &attr);

    /**
     * Gets the number of statements stored in table
     *
     * @return count of statement numbers
     */
    size_t getStmtCount();

    /**
     * Gets the count of statements based on type
     *
     * @return count of statement numbers of the specific type
     */
    size_t getStmtCount(StmtType type);

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
